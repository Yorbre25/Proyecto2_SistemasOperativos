#ifndef LIBRARY_COMMULIB_H
#define LIBRARY_COMMULIB_H


int initSerialComm();
int sendMostFrecuentWord(int serialPort);
int sendCommand(int serialPort, const char* command);

#endif 