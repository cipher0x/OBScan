#include "PID.hpp"




/*
 * ****************************************************************
 * ****************************************************************
 * ****************************************************************
 * PID CLASS Begin*************************************************
 * ****************************************************************
 * ****************************************************************
 * ****************************************************************
 */
PID::PID()
{
}



/*
 * Constructor sets up PID Object
 *
 */
PID::PID(int Mode, int PID, ELM327 *elmin)
{
    setMode(Mode);
    setPID(PID);
    elm = elmin;
}




PID::~PID()
{
    //if(PID1 != NULL) delete PID1; //dealocate memeory if needed

    //handle errors*************************************************
}



/*
 * Sets reqested OBDII access Mode
 *
 */
void PID::setMode(int mode)
{
    Mode = mode;
}



/*
 * Sets requested OBDII PID
 *
 */
void PID::setPID(int pid)
{
    pidNum = pid;
}



/*
 * Returns Current set OBDII Mode
 *
 */
int PID::getMode()
{
    return Mode;
}



/*
 * Returns current Set OBDII PID
 *
 */
int PID::getPIDNum()
{
    return pidNum;
}


/*
 * Returns PID Error State
 *
 */
 int PID::errorState()
 {
     return NoError;
 }


 /*
  * Communcates with OBDII Bus and sets up sub PID Objects
  *
  */
int PID::run()
{
      switch(Mode)
    {
        case 1:
            if(PID1 != NULL) delete PID1; //dealocate memeory if needed
            PID1 = new PIDMode1();
            PID1->start(this);

            //check for error condition
            if(NoError)
            {
                ;
            }
            else
            {
                cout <<"ERROR in getPID()"<<endl;
            }

            break;

        default:
            cout <<"ERROR in getPID()"<<endl;
            NoError = 0;
    }//switch
}//run()




/*
 * Attemtps to Communicate with OBDII Bus and reqest set PID and Mode
 *
 */
string PID::getPID()
{
    switch(Mode)
    {
        case 1:
            return PID1->finalPIDValue;
            break;
    }//switch




}



/*
 * Returns description of currently set Mode:PID
 *
 */
string PID::getPidDescription()
{
    switch(Mode)
    {
        //check for error condition
        if(!NoError)
        {
            return nullptr;
        }//if error

        case 1:
            return PID1->pidDescription;
            break;

        default:
            NoError = 0;
    }//Mode swtich
}//getPidDescription



/*
 * Returns Raw PID Vaule returned on BUS
 *
 */
string PID::getRawPidVaule()
{
    //check for error condition
    if(!NoError)
    {
        return nullptr;
    }//if error

    switch(Mode)
    {
        case 1:
            return PID1->rawPidValue;
            break;

        default:
            NoError = 0;
    }//Mode swtich
}



/*
 * Returns Max possible PID Value of current Mode::PID
 *
 */
string PID::getMaxPidVaule()
{
    //check for error condition
    if(!NoError)
    {
        return nullptr;
    }//if error

     switch(Mode)
    {
        case 1:
            return PID1->maxValue;
            break;

        default:
            NoError = 0;
    }//Mode swtich
}



/*
 * Returns Min possible IPD Value of current Mode::PID
 *
 */
string PID::getMinPidVaule()
{
    //check for error condition
    if(!NoError)
    {
        return nullptr;
    }//if error

    switch(Mode)
    {
        case 1:
            return PID1->minValue;
            break;

        default:
            NoError = 0;
    }//Mode swtich
}


/*
 *Returns Unit type of PID
 *
 */
string PID::getUnits()
{
    //check for error condition
    if(!NoError)
    {
        return nullptr;
    }//if error

    switch(Mode)
    {
        case 1:
            return PID1->units;
            break;

        default:
            NoError = 0;
    }//Mode swtich
}


/*
 * Converts an int to hex string zero padded to one byte ex. 0x01;
 *
 */
string PID::intByteToHexstr(int in )
{
  stringstream stream;
  stream << setfill('0') << setw(sizeof(char)*2)
         << hex << in;
  return stream.str();
}



/*
 * Converts an int to dec string
 *
 */
string PID::intToDecStr(int in )
{
    stringstream stream;
    stream << in;
    return stream.str();
}



/*
 * Converts a Hex string into type int
 *
 */
int PID::HexStrToInt(string in)
{
    stringstream stream;
    int rt;

    stream << in;
    stream >> hex >> rt;

    return rt;
}//HexStrToInt



/*
 * Convert Double to Dec String
 *
 */
string PID::doubleToStr(double in)
{
    stringstream stream;
    stream << in;
    return stream.str();
}


/*
 * ****************************************************************
 * ****************************************************************
 * ****************************************************************
 * PID CLASS END  *************************************************
 * ****************************************************************
 * ****************************************************************
 * ****************************************************************
 */



/*
 * ****************************************************************
 * ****************************************************************
 * ****************************************************************
 * PIDMode1 CLASS Begin********************************************
 * ****************************************************************
 * ****************************************************************
 * ****************************************************************
 */



/*
 * Takes call PID object as parm and requests Mode1:PID from vehicle
 *
 */
int PIDMode1::start(PID* const PIDIn)
{
    PIDObj = const_cast<PID*>(PIDIn);//removes const from this

    string cmd = "";
    string tmp ="";
    //adds current mode to command to be sent to vehicle
    switch(PIDObj->Mode)
    {
        case 1:
            cmd+="01";
            break;
        case 2:
            cmd+="02";
            break;
        case 3:
            cmd+="03";
            break;
        case 4:
            cmd+="04";
            break;
        case 5:
            cmd+="05";
            break;
        case 6:
            cmd+="06";
            break;
        case 7:
            cmd+="07";
            break;
        case 8:
            cmd+="08";
            break;
        case 9:
            cmd+="09";
            break;
        default:
            cmd+="00";
            PIDObj->NoError=0;
            return 0;
    }//Mode set switch

    //adds current PID to comamnd to be sent to vehicle
    if(PIDObj->pidNum <= 255)
    {
        cmd+=PIDObj->intByteToHexstr(PIDObj->pidNum);
    }
    else
    {
        PIDObj->NoError=0;
        return 0;
    }

    //for test
    //cout <<"OUT: "<< cmd<<endl;//testing*******************************************************************************************

    //writes command to OBDII BUS
    if(PIDObj->elm != nullptr)
    {
        cmd+="\r\n";//sends command

        PIDObj->elm->ELMWriteLine(cmd);
        tmp = PIDObj->elm->ELMReadLine();//read echo
        //cout <<"IN: "<<tmp<<endl;
        tmp = PIDObj->elm->ELMReadLine();//read data
        //cout <<"IN: "<<tmp<<endl;

        PIDObj->elm->ELMReadLine();//clear stream
        //cout <<"IN: "<<tmp<<endl;

    }
    else
    {
        PIDObj->NoError=0;
        return 0;
    }

    //save PID String
    rawPidValue = tmp;

    //process PID Value
    PIDProcessor();
}//pid start


/*
 * converts rawPidValue into finalPIDValue
 * Sets the Description, Unit, Min, and Max values of current PID
 *
 *
 * Need to Ignore first 2 bytes in rawPidValue***************************
 * 4MODE::PID::VALUE BITTS
 *
 */
void PIDMode1::PIDProcessor()
{
    string strtmp = "";
    string Byte_1 = "";
    string Byte_2 = "";

    //if General Module Error
    if(rawPidValue.substr(0,2)==PIDObj->GeneralRqErr)
    {
        finalPIDValue="NA";
        return;
    }//if



    switch(PIDObj->pidNum)
    {
       case 0x00://Bit encoded supported PIDs 0x01 -> 0x20
           strtmp = rawPidValue.substr(3,strtmp.length());//ignore first 2 bytes
           strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=rawPidValue;
           pidDescription="PIDs supported 0x01 -> 0x20";
           units="";
           maxValue="";
           maxValue="";
           break;

        case 0x01:////Bit encoded Monitor Status
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=rawPidValue;
           //cout<<"Final PID: "<<finalPIDValue<<endl;
           pidDescription="MIL Status and DTCs count";
           units="";
           maxValue="";
           minValue="";
           break;

       case 0x02://Freez DTC
           //cout <<"PID2: "<< rawPidValue<<endl;
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=rawPidValue;
           pidDescription="Freeze DTC";
           units="";
           maxValue="";
           minValue="";
           break;


      /*
       * 0x03 Bit Encoded Fuel System Status
       * 2 Bytes
       *
       * Calculation:
       * 1    Open loop due to insufficient engine temperature
       * 2    Closed loop, using oxygen sensor feedback to determine fuel mix
       * 4    Open loop due to engine load OR fuel cut due to deceleration
       * 8    Open loop due to system failure
       * 16   Closed loop, using at least one oxygen sensor but there is a fault in the feedback system
       *
       * Second Byte Describes System #2 if installed
       */
       case 0x03://NEED TO ERR CHECK***********************************************************************************************************************
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           //Byte_1 = erase(remove_if(Byte_1.begin(), Byte_1.end(), ::isspace), Byte_1.end());
           //cout << "PID 3 B1: "<<Byte_1<<endl;
           Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           //Byte_2 = erase(remove_if(Byte_2.begin(), Byte_2.end(), ::isspace), Byte_2.end());
        //cout << "PID 3 B2: "<<Byte_2<<endl;
           finalPIDValue=PIDObj->intToDecStr(PIDObj->HexStrToInt(Byte_1));//only supports #1 Fule System
           pidDescription="Fule loop status";
           units="Bit Encoded Value";
           maxValue="16";
           minValue="1";
           break;

       /*
        * 0x04 Calculated engine load
        * 1 Byte
        * Calculation: (100/255)rawPidValue
        */
        case 0x04:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->intToDecStr((int)((100/255)*(PIDObj->HexStrToInt(rawPidValue))));
           pidDescription="Calculated engine load";
           units="%";
           maxValue="100";
           minValue="0";
           break;

      /*
       * 0x05 Engine coolant temperature
       * 1 Byte
       * Calculation: rawPidValue-40
       */
       case 0x05:
           Byte_1 = rawPidValue.substr(6,2);//ignore first 2 bytes
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)-40);
           pidDescription="Engine coolant temperature";
           units="C";
           maxValue="215";
           minValue="-40";
           break;

      /*
       * 0x06 Short term fuel trim Bank 1
       * 1 Byte
       * Calculation: ((100/128)*rawPidValue)-100
       */
       case 0x06:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->doubleToStr((100/128)*(PIDObj->HexStrToInt(strtmp)-100));
           pidDescription="Short term fuel trim—Bank 1";
           units="%";
           maxValue="99.2";
           minValue="-100";
           break;

      /*
       * 0x07 Long term fuel trim Bank 1
       * 1 Byte
       * Calculation: ((100/128)*rawPidValue)-100
       */
       case 0x07:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->doubleToStr((100/128)*(PIDObj->HexStrToInt(strtmp)-100));
           pidDescription="Long term fuel trim—Bank 1";
           units="%";
           maxValue="99.2";
           minValue="-100";
           break;

      /*
       * 0x08 Short term fuel trim Bank 2
       * 1 Byte
       * Calculation: ((100/128)*rawPidValue)-100
       */
       case 0x08:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->doubleToStr((100/128)*(PIDObj->HexStrToInt(strtmp)-100));
           pidDescription="Long term fuel trim—Bank 2";
           units="%";
           maxValue="99.2";
           minValue="-100";
           break;

      /*
       * 0x09 Long term fuel trim Bank 2
       * 1 Byte
       * Calculation: ((100/128)*rawPidValue)-100
       */
       case 0x09:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->doubleToStr((100/128)*(PIDObj->HexStrToInt(strtmp)-100));
           pidDescription="Long term fuel trim—Bank 2";
           units="%";
           maxValue="99.2";
           minValue="-100";
           break;

      /*
       * 0x0A Fule Pressure
       * 1 Byte
       * Calculation: 3*rawPidValue
       */
       case 0x0a:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->intToDecStr(3*PIDObj->HexStrToInt(strtmp));
           pidDescription="Fuel pressure ";
           units="kPa";
           maxValue="765";
           minValue="0";
           break;

      /*
       * 0x0b Intake manifold absolute pressure
       * 1 Byte
       * Calculation: rawPidValue
       */
       case 0x0b:
           strtmp = rawPidValue.substr(6,2);//ignore first 2 bytes
           //strtmp.erase(remove_if(strtmp.begin(), strtmp.end(), ::isspace), strtmp.end());
           finalPIDValue=PIDObj->intToDecStr(PIDObj->HexStrToInt(strtmp));
           pidDescription="Intake manifold absolute pressure";
           units="kPa";
           maxValue="255";
           minValue="0";
           break;

      /*
       * 0x0c Engine RPM
       * 2 Bytes
       * Calculation: (256*Byte_1+Byte_2)/4
       */
       case 0x0c:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           //Byte_1 = erase(remove_if(Byte_1.begin(), Byte_1.end(), ::isspace), Byte_1.end());
           //cout << "PID 3 B1: "<<Byte_1<<endl;
           Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           //Byte_2 = erase(remove_if(Byte_2.begin(), Byte_2.end(), ::isspace), Byte_2.end());
        //cout << "PID 3 B2: "<<Byte_2<<endl;
           finalPIDValue=PIDObj->doubleToStr((256*(PIDObj->HexStrToInt(Byte_1)+PIDObj->HexStrToInt(Byte_2))/4));
           pidDescription="Engine RPM";
           units="rpm";
           maxValue="16383.75";
           minValue="0";
           break;

      /*
       * 0x0d Vehicle speed
       * 1 Byte
       * Calculation: Byte_1
       */
       case 0x0d:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           finalPIDValue=PIDObj->intToDecStr(PIDObj->HexStrToInt(Byte_1));
           pidDescription="Vehicle speed";
           units="km/h";
           maxValue="255";
           minValue="0";
           break;

      /*
       * 0x0e Timing advance
       * 1 Byte
       * Calculation: (Byte_1 / 2) - 64
       */
       case 0x0e:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           finalPIDValue=PIDObj->doubleToStr((PIDObj->HexStrToInt(Byte_1)/2)-64);
           pidDescription="Timing advance";
           units="Degrees before TDC";
           maxValue="63.5";
           minValue="-64";
           break;

      /*
       * 0x0f Intake air temperature
       * 1 Byte
       * Calculation: Byte_1 - 40
       */
       case 0x0f:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           finalPIDValue=PIDObj->intToDecStr(PIDObj->HexStrToInt(Byte_1) - 40);
           pidDescription="Intake air temperature";
           units="C";
           maxValue="215";
           minValue="-40";
           break;

       /*
        * 0x10 MAF air flow rate
        * 2 Bytes
        * Calculation: (256*Byte_1+Byte_2)/100
        */
       case 0x10:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(((256*PIDObj->HexStrToInt(Byte_1))+PIDObj->HexStrToInt(Byte_2)/100));
           pidDescription="MAF air flow rate";
           units="g/s";
           maxValue="655.35";
           minValue="0";
           break;

       /*
        * 0X11 Throttle position
        * 1 Byte
        * Calculation: (100/255)*BYTE_1
        */
       case 0x11:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           finalPIDValue=PIDObj->doubleToStr((100/255)*PIDObj->HexStrToInt(Byte_1));
           pidDescription="Throttle position";
           units="%";
           maxValue="100";
           minValue="0";
           break;

       /*
        * 0x12 Commanded secondary air status
        * 1 byte
        * Calculation:
        *
        * 1 Upsteram
        * 2 Downstream of catalytic converter
        * 4 From the ouside atmosphere or off
        * 8 Pump commanded on for diagnostics
        */
       case 0x12:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           finalPIDValue=PIDObj->intToDecStr(PIDObj->HexStrToInt(Byte_1));
           pidDescription="Commanded secondary air status";
           units="";
           maxValue="8";
           minValue="1";
           break;

       /*
        * 0x13 Oxygen sensors present (in 2 banks)
        * 1 Byte
        * Calculation:
        * [A0..A3] == Bank 1, Sensors 1-4. [A4..A7] == Bank 2...
        *
        */
       case 0x13:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           finalPIDValue=PIDObj->intToDecStr(PIDObj->HexStrToInt(Byte_1));
           pidDescription="Oxygen sensor present";
           units="[A0..A3] == Bank 1, Sensors 1-4. [A4..A7] == Bank 2...";
           maxValue="";
           minValue="";
           break;

       /* 0x14 Oxygen Sensor 1
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x14:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 1";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x15 Oxygen Sensor 2
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x15:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 2";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x16 Oxygen Sensor 3
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x16:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 3";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x17 Oxygen Sensor 4
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x17:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 4";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x18 Oxygen Sensor 5
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x18:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 5";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x19 Oxygen Sensor 6
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x19:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 6";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x1a Oxygen Sensor 7
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x1a:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 7";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x1b Oxygen Sensor 8
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x1b:
           Byte_1 = rawPidValue.substr(6,2);//Get First Data Byte
           Byte_2 = Byte_2 = rawPidValue.substr(8,2);//Get Second Data Byte
           finalPIDValue=PIDObj->doubleToStr(PIDObj->HexStrToInt(Byte_1)/200);
           pidDescription="Oxygen Sensor 8";
           units="volts";
           maxValue="1.275";
           minValue="0";
           break;

       /* 0x1c OBD standards
        * 2 Bytes
        * Calcuatlion: Voltage = Byte_1/200
        * Byte_1 = Voltage
        * Byte_2 = Short term fuel trim
        */
       case 0x1c:
           finalPIDValue=rawPidValue;
           pidDescription="OBD standards";
           units="";
           maxValue="";
           minValue="";
           break;

       /* 0x1d Present oxygen sensors
        * 1 Byte
        * Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2]
        *
        */
       case 0x1d:
           finalPIDValue=rawPidValue;
           pidDescription="Present Oxygen Sensors";
           units="Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2]";
           maxValue="";
           minValue="";
           break;

       /* 0x1e Auxiliary input states
        * 1 Byte
        * Bit Encoded: A0 == Power Take Off (PTO) status (1 == active)[A1..A7] not used
        *
        */
       case 0x1e:
           finalPIDValue=rawPidValue;
           pidDescription="Auxiliary input states";
           units="Bit Encoded: A0 == Power Take Off (PTO) status (1 == active)[A1..A7] not used";
           maxValue="";
           minValue="";
           break;

       /* 0x1f Engine run time since start
        * 2 Bytes
        * Calcuatlion: RunTime = 256*Byte_1+Byte_2
        *
        */
       case 0x1f:
           finalPIDValue=PIDObj->intToDecStr((256*PIDObj->HexStrToInt(Byte_1)+PIDObj->HexStrToInt(Byte_2)));
           pidDescription="Engine run time since start";
           units="seconds";
           maxValue="65535";
           minValue="0";
           break;

//        case 0x20:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x21:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x22:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x23:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x24:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x25:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x26:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x27:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x28:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x29:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x2a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x2b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x2c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x2d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x2e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x2f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x30:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x31:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x32:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x33:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x34:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x35:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x36:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x37:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x38:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x39:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x3a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x3b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x3c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x3d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x3e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x3f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x40:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x41:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x42:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x43:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x44:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x45:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x46:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x47:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x48:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x49:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x4a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x4b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x4c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x4d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x4e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x4f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x50:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x51:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x52:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x53:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x54:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x55:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x56:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x57:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x58:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x59:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x5a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x5b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x5c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x5d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x5e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x5f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x60:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x61:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x62:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x63:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x64:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x65:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x66:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x67:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x68:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x69:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x6a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x6b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x6c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x6d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x6e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x6f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x70:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x71:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x72:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x73:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x74:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x75:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x76:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x77:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x78:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x79:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x7a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x7b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x7c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x7d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x7e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x7f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x80:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x81:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x82:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x83:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x84:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x85:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x86:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x87:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x88:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x89:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x8a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x8b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x8c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x8d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x8e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x8f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x90:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x91:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x92:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x93:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x94:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x95:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x96:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x97:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x98:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x99:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x9a:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x9b:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x9c:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x9d:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x9e:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0x9f:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa0:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa1:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa2:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa3:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa4:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa5:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa6:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa7:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa8:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xa9:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xaa:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xab:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xac:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xad:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xae:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xaf:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb0:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb1:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb2:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb3:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb4:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb5:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb6:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb7:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb8:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xb9:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xba:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xbb:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xbc:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xbd:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xbe:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xbf:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xc0:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xc1:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xc2:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xc3:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;
//
//        case 0xc4:
//            finalPIDValue=;
//            pidDescription=;
//            units=;
//            maxValue=;
//            minValue=;
//            break;

    }//switch

}//void PIDMode1::PIDProcessor()
