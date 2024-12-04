#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"

#define MSG_BUF_SIZE 128

// config the serial port behavior (since the defaults are not good)
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
int configSerialPort() {
  int serialPort = open("/dev/ttyACM0", O_RDWR);

  struct termios tty;
  AssertErr(tcgetattr(serialPort, &tty));
  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;
  tty.c_cc[VTIME] = 10;
  tty.c_cc[VMIN] = 0;
  cfsetspeed(&tty, B115200);
  AssertErr(tcsetattr(serialPort, TCSANOW, &tty));

  return serialPort;
}

void handleMsg(char msg[MSG_BUF_SIZE + 1]) {
  char buf[MSG_BUF_SIZE + 1] = {};
  strncpy(buf, msg, strlen(msg));

  switch (buf[0]) {
    case 'e':
      Logger(ERROR, "(pico) %s", buf + 2);
      break;
    case 'i':
      Logger(INFO, "(pico) %s", buf + 2);
      break;
    case 'd':
      Logger(DEBUG, "(pico) %s", buf + 2);
      break;
  }
}

int main() {
  int serialPort = configSerialPort();
  SetLogLevel(DEBUG);

  char msgBuf[MSG_BUF_SIZE + 1] = {};
  int idx = 0;
  while (true) {
    char c;
    int numBytes = read(serialPort, &c, 1);
    if (numBytes < 0) {
      Logger(ERROR, "Error reading serial port, %s", strerror(errno));
      continue;
    }

    if (numBytes == 0) {
      continue;
    }

    if (c == '\n') {
      Logger(DEBUG, "Received msg %s", msgBuf);
      handleMsg(msgBuf);

      idx = 0;
      memset(msgBuf, '\0', MSG_BUF_SIZE);
      continue;
    }

    if (idx >= MSG_BUF_SIZE) {
      Logger(DEBUG, "Received msg %s", msgBuf);
      Logger(ERROR, "input too large, missing newline character");
      continue;
    }

    msgBuf[idx++] = c;
  }

  close(serialPort);
  return 0;
}
