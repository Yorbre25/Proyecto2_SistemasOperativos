#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "Library/commulib.h"


int main(){
    int serialPort = initSerialComm();
    // char num[1] = {'1'};
    // sendCommand(serialPort, num);

    sendMostFrecuentWord(serialPort);
    return 0;
}