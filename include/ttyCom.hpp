#ifndef TTYCOM_HPP
#define TTYCOM_HPP

#include "cfgproc.hpp"
#include <iostream>
#include <string>
#include <thread> 
#include <SerialStream.h>
#include <SerialPort.h>

using namespace std;
using namespace LibSerial ;

class ttyCom
{
public:
    ttyCom();
    explicit ttyCom(cfgproc &ttyConfig);
    void loadConfig(cfgproc &ttyConfig);
    ~ttyCom();
    int write(string &out);
    string read();
    
    
private:
    int parseConfig(cfgproc ttyConfig);
    SerialStream serial_stream ;
};


#endif
