#ifndef PID_HPP
#define PID_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "ELM327.hpp"

using namespace std;


/*
 * Forward declarations for all Mode classes
 *
 */
class PIDMode1;




/*
 * Master PID Interface Class for all Modes
 *
 */

class PID
{
    friend class PIDMode1;
public:
    PID();
    PID(int Mode, int PID, ELM327 *elmin);
    ~PID();
    void setMode(int mode);
    void setPID(int pid);
    int getMode();
    int getPIDNum();
    int errorState();
    int run();
    string getPID();
    string getPidDescription();
    string getRawPidVaule();
    string getMaxPidVaule();
    string getMinPidVaule();
    string getUnits();


private:
    string GeneralRqErr="7F";
    int NoError=1;//zero on error
    ELM327 *elm = nullptr;
    int Mode;
    int pidNum;
    string PIDvalue;

    //diffrent mode objects
    PIDMode1 *PID1 = NULL;

    /*
     * Data type converstion methods
     *
     */
    string intByteToHexstr(int in);
    string intToDecStr(int in );
    int HexStrToInt(string &in);
    string doubleToStr(double in);
};


class PIDMode1
{
    friend class PID;
public:
    
    int start(PID* const PIDIn);

private:

    string pidDescription="";
    string units="";
    string rawPidValue="";
    string finalPIDValue="";
    string maxValue="";
    string minValue="";
    uint32_t getMode1PID();
    PID *PIDObj;
    void PIDProcessor();
};



#endif
