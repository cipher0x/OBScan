#ifndef ELM327_HPP
#define ELM327_HPP

#include "ttyCom.hpp"
#include "cfgproc.hpp"
#include <string>

class ELM327
{
public:
    ELM327();
    explicit ELM327(cfgproc &cfg);
    void elmConnect(cfgproc &cfg);
    ~ELM327();
    int connectECM();
    string getELMVersion(){return ELMVersion;}
    string ELMReadLine();
    void ELMWriteLine(string strIn);
    
private:
    string CRLF = "\r\n";
    string PROTOCOL;
    ttyCom tty;
    string ELMVersion="";
    
    int init(cfgproc cf);
    int parseConfig(cfgproc cfg);
    
};

#endif
