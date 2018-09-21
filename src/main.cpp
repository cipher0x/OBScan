#include "cfgproc.hpp"
#include "ttyCom.hpp"
#include "ELM327.hpp"
#include "ELM327OBD.hpp"

#include "PID.hpp"


int main(int argc, char *argv[])
{
    //loades config from tty.conf file
    cfgproc cfg;
    ELM327 elm(cfg);

    PID ReadPID;
    ELM327OBD elmobd(elm);
    elmobd.setMode(1);

    //Scan all PIDs
    cout<<"PID,OPDATA"<<endl;
    for(int i = 1; i <= 0xff; i++)
    {
        ReadPID = elmobd.getPID(i);
        cout<<std::hex<<i<<","<<ReadPID.getRawPidVaule()<<endl;
    }//for
    
    return 0;
}//main
