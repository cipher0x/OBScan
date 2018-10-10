#include <math.h>
#include <sstream>
#include <string>
#include "cfgproc.hpp"
#include "ttyCom.hpp"
#include "ELM327.hpp"
#include "ELM327OBD.hpp"

#include "PID.hpp"

#include <bitset>

uint8_t reverseByte(uint8_t b);

int main(int argc, char *argv[])
{
    //loades config from tty.conf file
    cfgproc cfg;
    ELM327 elm(cfg);


    PID ReadPID;
    ELM327OBD elmobd(elm);
    elmobd.setMode(1);

    //Scan all PIDs
    cout<<"PID,OPDATA,Reported Nib,Reported Bit,Status"<<endl;

    std::string reportedPIDs = "";
    std::string nib = "";
    std::string pidVal="";
    std::stringstream ss;
    std::stringstream line;
    std::string lineOut = "";
    uint32_t intByte = 0;
    uint8_t byte = 0;
    uint8_t bit = 0;

//*********PIDs 0>32***********************************************************
    for(int i = 0; i <= 0x60; i++)
    {
        ReadPID = elmobd.getPID(i);
        line<<std::hex<<i<<","<<ReadPID.getRawPidVaule()<<",";



        if(i >=0 & i <= 0x20)//first 32 PID report
        {

          if(i==0)//gets first reported PIDs 0x01 to 0x20
          {

              reportedPIDs = ReadPID.getRawPidVaule();
              reportedPIDs = reportedPIDs.substr(6,12);//get just Data
              std::remove(reportedPIDs.begin(), reportedPIDs.end(), ' ' );//removes whitespace

          }//if

          nib = reportedPIDs.substr(std::floor((i-1)/4),1);
          if(i==0)//if on supported PIDs
          {
            nib="";
            bit = 0;
            line <<nib<<",";
            line<<std::hex<<bit<<",";
          }
          else//if not supported PIDs
          {
            line <<nib<<",";

            //convert hex string to int
            ss << std::hex<<nib;
            ss >> intByte;
            byte = intByte;

            byte = reverseByte(byte);//reverse bit order
            byte = byte >> 4;

            //convert to bitset object
            std::bitset<4> bitSetByte (byte);
            cout <<bitSetByte<<endl;


            //reset stringstream
            ss.str(std::string());
            ss.clear();

            //get current PID reported bit
            bit = bitSetByte[(i-1)%4];
            line<<std::hex<<(int)bit<<",";
          }

          if(!bit & (ReadPID.getRawPidVaule() != "NO DATA"))//if reporting no data when data is available
          {
            line <<"Not Reported";
          }
          else
          {
            line <<"Reported";
          }

        }//if0 to 0x19



        if(i >=0x20 & i <= 0x40)//second 32 PID report
        {

          if(i==0x20)//gets first reported PIDs 0x01 to 0x20
          {

              reportedPIDs = ReadPID.getRawPidVaule();
              reportedPIDs = reportedPIDs.substr(6,12);//get just Data
              std::remove(reportedPIDs.begin(), reportedPIDs.end(), ' ' );//removes whitespace

          }//if

          nib = reportedPIDs.substr(std::floor(((i-0x20)-1)/4),1);
          if(i==0x20)//if on supported PIDs
          {
            nib="";
            bit = 0;
            line <<nib<<",";
            line<<std::hex<<bit<<",";
          }
          else//if not supported PIDs
          {
            line <<nib<<",";

            //convert hex string to int
            ss << std::hex<<nib;
            ss >> intByte;
            byte = intByte;

            byte = reverseByte(byte);//reverse bit order
            byte = byte >> 4;

            //convert to bitset object
            std::bitset<4> bitSetByte (byte);
            cout <<bitSetByte<<endl;


            //reset stringstream
            ss.str(std::string());
            ss.clear();

            //get current PID reported bit
            bit = bitSetByte[((i-0x20)-1)%4];
            line<<std::hex<<(int)bit<<",";
          }

          if(i!= 0x20)
          {
            if(!bit & (ReadPID.getRawPidVaule() != "NO DATA"))//if reporting no data when data is available
            {
              line <<"Not Reported";
            }
            else
            {
              line <<"Reported";
            }
          }//of not on 0x20
          else//remove extra , on reporting PID
          {
            line.str(line.str().erase(line.str().size()-3));
          }

        }//if 0x20 to 0x40



        if(i >=0x40 & i <= 0x60)//third 32 PID report
        {

          if(i==0x40)//gets first reported PIDs
          {

              reportedPIDs = ReadPID.getRawPidVaule();
              reportedPIDs = reportedPIDs.substr(6,12);//get just Data
              std::remove(reportedPIDs.begin(), reportedPIDs.end(), ' ' );//removes whitespace

          }//if

          nib = reportedPIDs.substr(std::floor(((i-0x40)-1)/4),1);
          if(i==0x40)//if on supported PIDs
          {
            nib="";
            bit = 0;
            line <<nib<<",";
            line<<std::hex<<bit<<",";
          }
          else//if not supported PIDs
          {
            line <<nib<<",";

            //convert hex string to int
            ss << std::hex<<nib;
            ss >> intByte;
            byte = intByte;

            byte = reverseByte(byte);//reverse bit order
            byte = byte >> 4;

            //convert to bitset object
            std::bitset<4> bitSetByte (byte);
            cout <<bitSetByte<<endl;


            //reset stringstream
            ss.str(std::string());
            ss.clear();

            //get current PID reported bit
            bit = bitSetByte[((i-0x40)-1)%4];
            line<<std::hex<<(int)bit<<",";
          }

          if(i!= 0x40)
          {
            if(!bit & (ReadPID.getRawPidVaule() != "NO DATA"))//if reporting no data when data is available
            {
              line <<"Not Reported";
            }
            else
            {
              line <<"Reported";
            }
          }//of not on 0x20
          else//remove extra , on reporting PID
          {
            line.str(line.str().erase(line.str().size()-3));
          }

        }//if 0x40 to 0x60


    cout << line.str() << endl;
    //reset stringstream
    line.str(std::string());
    line.clear();

    }//for


    return 0;
}//main



uint8_t reverseByte(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
