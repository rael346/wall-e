#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serial.h"
#include "state.h"
#include "utils.h"

#define MSG_BUF_SIZE 128

volatile sig_atomic_t signal_received = 0;
void sigint_handler(int signal) { signal_received = signal; }

void handleMsg(char msg[MSG_BUF_SIZE + 1]) {
  char buf[MSG_BUF_SIZE + 1 - 2] = {};
  strncpy(buf, msg + 2, strlen(msg));

  switch (msg[0]) {
    case 'e':
      Logger(DEBUG, "(pico error) %s", buf);
      break;
    case 'i':
      Logger(DEBUG, "(pico info) %s", buf);
      SetDist(atoi(buf));
      break;
    case 'd':
      Logger(DEBUG, "(pico debug) %s", buf);
      break;
  }

  enum State currState = GetCurrState();
  enum State prevState = GetPrevState();

  if (prevState == FORWARD && currState == STOP) {
    Logger(DEBUG, "STOP before a wall");
    WriteMsg("0 0 0 0");
    SetCurrState(SPIN);
  }

  if (prevState == STOP && currState == SPIN) {
    Logger(DEBUG, "SPIN to a new direction");
    WriteMsg("1 1 2 1");
    SetCurrState(SPIN);
    usleep(500 * 1000);
  }

  if (prevState == SPIN && currState == STOP) {
    Logger(DEBUG, "STOP before a new direction");
    WriteMsg("0 0 0 0");
    SetCurrState(FORWARD);
  }

  if (prevState == STOP && currState == FORWARD) {
    Logger(DEBUG, "FORWARD to the new direction");
    WriteMsg("1 1 1 1");
    SetCurrState(FORWARD);
  }
}

int main() {
  ConfigSerial();
  SetLogLevel(DEBUG);

  WriteMsg("1 1 1 1");
  char msgBuf[MSG_BUF_SIZE + 1] = {};
  int idx = 0;
  while (!signal_received) {
    char c;
    ssize_t numBytes = ReadChar(&c);
    if (numBytes == 0) {
      continue;
    }

    if (c == '\n') {
      /* Logger(DEBUG, "received %s", msgBuf); */
      handleMsg(msgBuf);

      idx = 0;
      memset(msgBuf, '\0', MSG_BUF_SIZE);
      continue;
    }

    if (idx >= MSG_BUF_SIZE) {
      Logger(DEBUG, "received %s", msgBuf);
      Logger(ERROR, "input too large, missing newline character");
      continue;
    }

    msgBuf[idx++] = c;
  }

  CloseSerial();
  return 0;
}
