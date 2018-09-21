 
#ifndef CFGPROC_H
#define CFGPROC_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class cfgproc
{
public:
    string getDevice(){return DEVICE;};
    string getDevice_Type(){return DEVICE_TYPE;};
    string getParity(){return PARITY;};
    string getBaud(){return BAUD;};
    string getData_Bits(){return DATA_BITS;};
    string getStop_Bits(){return STOP_BITS;};
    string getError_Correct(){return ERROR_CORRECT;};
    string getProtocol(){return PROTOCOL;};
    cfgproc();
    ~cfgproc();
    int reloadCFG();
    
    
private:
    string DEVICE;
    string DEVICE_TYPE;
    string PARITY;
    string BAUD;
    string DATA_BITS;
    string STOP_BITS;
    string ERROR_CORRECT;
    string PROTOCOL;
    int loadCFG();
    
};

#endif
