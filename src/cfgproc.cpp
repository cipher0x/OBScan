#include "cfgproc.hpp"


cfgproc::cfgproc()
{
    loadCFG();
}


cfgproc::~cfgproc()
{
}

int cfgproc::loadCFG()
{
    string tmp;
    ifstream cfgFile;
    cfgFile.open ("/etc/OBScan.conf");

    //read file while open and not at end
    while((!cfgFile.eof()) && (cfgFile.is_open()))
    {
        tmp="";
        cfgFile >> tmp;

        //parse config file and load config VARS
        if(tmp != "")//if line is not empty
        {

            if(tmp.find("DEVICE=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                DEVICE = tmp.substr(pos);
            }//if Device
            else if(tmp.find("DEVICE_TYPE=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                DEVICE_TYPE = tmp.substr(pos);
            }//if Device_Type
            else if(tmp.find("PARITY=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                PARITY = tmp.substr(pos);
            }//if PARITY
            else if(tmp.find("BAUD=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                BAUD = tmp.substr(pos);
            }//if BAUD
            else if(tmp.find("DATA_BITS=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                DATA_BITS = tmp.substr(pos);
            }//if DATA_BITS
            else if(tmp.find("STOP_BITS=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                STOP_BITS = tmp.substr(pos);
            }//if STOP_BITS
            else if(tmp.find("ERROR_CORRECT=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                ERROR_CORRECT = tmp.substr(pos);
            }//if ERROR_CORRECT
            else if(tmp.find("PROTOCOL=") != string::npos)
            {
                size_t pos = tmp.find("=");
                pos++;

                PROTOCOL = tmp.substr(pos);
            }

        }//if line not empty
    }//while
}//loadCFG()
