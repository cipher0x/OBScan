#ifndef TTYCOM_HPP
#define TTYCOM_HPP

#include "cfgproc.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <libserial/SerialPort.h>

using namespace std;
using namespace LibSerial ;

#define PORT_END_OF_LINE '\r'
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
    SerialPort serial_port ;
};


#endif
