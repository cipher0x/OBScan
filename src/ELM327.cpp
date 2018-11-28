
#include "ELM327.hpp"

ELM327::ELM327()
{
}




ELM327::ELM327(cfgproc &cfg)
{
    elmConnect(cfg);
}




ELM327::~ELM327()
{
}



void ELM327::elmConnect(cfgproc &cfg)
{
    parseConfig(cfg);
    init(cfg);
}




int ELM327::init(cfgproc cfg)
{
    tty.loadConfig(cfg);
}


/*
 * Connect to ECM
 * returns zero on error
 */
int ELM327::connectECM()
{
    string ELMIN = "";

    ELMWriteLine("ATZ"+CRLF);//reset ELM state

    //clear new new lines and reads in return value
    ELMIN = ELMReadLine();//read echo
    ELMIN = ELMReadLine();//read new line
    ELMIN = ELMReadLine();//read new line
    ELMIN = ELMReadLine();//read ELM version

    //save ELM327 chip version
    ELMVersion = ELMIN;

    //check for valid state
    //if not in valid state return 0
    if(!(ELMVersion.find("ELM") != string::npos))
    {
        return 0;
    }//if not in valided state

    ELMWriteLine("ATSP"+ PROTOCOL+CRLF);
    ELMIN = ELMReadLine();//read echo chars
    ELMIN = ELMReadLine();//read new line
    ELMIN = ELMReadLine();//read commands error value



    //if not in valid state return 0
    if(!(ELMIN.find("OK") != string::npos))
    {
        return 0;
    }//if not in valided state
    cout << ELMIN <<endl;
    return 1;
}//elmConnect


/*
 * Read Line from ELM327 Device
 *
 */
string ELM327::ELMReadLine()
{
    string rt;
    rt = tty.read();
    cout<<"GREEN"<<endl;
    return rt;
}



/*
 * Write Line to ELM327 Device
 *
 */
void ELM327::ELMWriteLine(string strIn)
{
    tty.write(strIn);
}



int ELM327::parseConfig(cfgproc cfg)
{
    if((stoi(cfg.getProtocol()) >= 0) &&(stoi(cfg.getProtocol()) <= 9)) //if in valid confg range
    {
        PROTOCOL = cfg.getProtocol();
    }//if
    else//invalid prototocl range
    {
         std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Invalid protocol in config file: "<<stoi(cfg.getProtocol())
                  << std::endl;

                  exit(1);
    }//else
}
