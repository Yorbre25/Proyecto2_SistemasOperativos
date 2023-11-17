#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "Library/commulib.h"


int main(){
    int serialPort = initSerialComm();
    char word[] = "Arduino\n";
    sendCommand(serialPort, word);

    return 0;
}