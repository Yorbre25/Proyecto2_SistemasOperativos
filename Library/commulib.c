#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "commulib.h"

#define ARDUINO_PORT "/dev/ttyACM0"  // Serial port device file
#define MOST_FRECUENT_WORD_FILE "Library/most_frecuent_word.txt"


int serialport_flush(int fd)
{
    sleep(2); //Por algún motivo no funciona si no se espera un poco
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
    options.c_cflag &= ~PARENB;  
    options.c_cflag &= ~CSTOPB;  
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;       

    tcsetattr(serialPort, TCSANOW, &options);
    return serialPort;
}

int sendCommand(int serialPort, const char* command) {
    int len = strlen(command);
    int n = write(serialPort, command, len);
    usleep( 2 * 100000 );  // wait 1 msec try again
    if( n!=len ) {
        perror("serialport_write: couldn't write whole string\n");
        return -1;
    }
    return 0;
}

int sendMostFrecuentWord(int serialPort){
    FILE *filePointer;

    filePointer = fopen(MOST_FRECUENT_WORD_FILE, "r");

    if (filePointer == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Buffer to store the line
    char line[256]; 


    if (fgets(line, sizeof(line), filePointer) != NULL) {
        printf("First line: %s\n", line);
        char num[1] = {'1'};
        sendCommand(serialPort, num);
    } else {
        printf("The file is empty\n");
    }
}

int initSerialComm(){
    int serialPort = openSerialPort();
    serialport_flush(serialPort);
    if (serialPort == -1) {
        return 1;
    }
    return serialPort;
}

