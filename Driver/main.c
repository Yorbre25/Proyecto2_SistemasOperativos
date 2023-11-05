#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define ARDUINO_PORT "/dev/ttyACM0"  // Serial port device file


int serialport_flush(int fd)
{
    sleep(2); //required to make flush work, for some reason
    return tcflush(fd, TCIOFLUSH);
}

int openSerialPort() {

    int serialPort = open(ARDUINO_PORT, O_RDWR | O_NONBLOCK);
    if (serialPort == -1) {
        perror("Error opening serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(serialPort, &options);

    cfsetispeed(&options, B9600);  // Set baud rate
    cfsetospeed(&options, B9600);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;   // No parity
    options.c_cflag &= ~CSTOPB;   // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;       // 8 data bits

    tcsetattr(serialPort, TCSANOW, &options);
    return serialPort;
}

int sendCommandToArduino(int serialPort, const char* command) {
    int len = strlen(command);
    int n = write(serialPort, command, len);
    usleep( 2 * 100000 );  // wait 1 msec try again
    if( n!=len ) {
        perror("serialport_write: couldn't write whole string\n");
        return -1;
    }
    return 0;
}

int initSerialComm(){
    int serialPort = openSerialPort();
    serialport_flush(serialPort);
    if (serialPort == -1) {
        return 1;
    }
    return serialPort;
}

int main(){
    int serialPort = initSerialComm();
    char num[1] = {'1'};
    sendCommandToArduino(serialPort, num);
    return 0;
}