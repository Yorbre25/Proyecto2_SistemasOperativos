#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "Library/commulib.h"


int main(){
    int serialPort = initSerialComm();
    char num[4] = {'hola'};
    sendCommand(serialPort, num);
    readResp(serialPort);

    // sendMostFrecuentWord(serialPort);
    return 0;
}