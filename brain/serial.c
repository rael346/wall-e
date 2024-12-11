#include "serial.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"

int serialPort = -1;

// config the serial port behavior (since the defaults are not good)
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
void ConfigSerial() {
  serialPort = open("/dev/ttyACM0", O_RDWR);

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
}

void WriteMsg(const char msg[]) {
  // The first +1 is for \r
  // The second +1 is for \0
  char buf[strlen(msg) + 1 + 1];
  sprintf(buf, "%s\r", msg);
  Logger(DEBUG, "write %s", buf);
  AssertErr(write(serialPort, buf, strlen(buf)));
}

ssize_t ReadChar(char* c) {
  int numBytes = read(serialPort, c, 1);
  AssertErr(numBytes);
  return numBytes;
}

void CloseSerial() { close(serialPort); }
