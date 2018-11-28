#include "ELM327OBD.hpp"


/*
 * Empty Constructor
 *
 */
ELM327OBD::ELM327OBD()
{
}



/*
 * Constructor takes an ELM327 intisilized object
 *
 */
ELM327OBD::ELM327OBD(ELM327 &elmin)
{
    setELM327(elmin);
}



/*
 * Default Destructor
 *
 */
ELM327OBD::~ELM327OBD()
{
}


/*
 * Sets up the local ELM327 object
 *
 */
int ELM327OBD::setELM327(ELM327 &elmin)
{
    elm = &elmin;
}



 /*
  * Sets the Requesed OBDII Mode
  *
  */
int ELM327OBD::setMode(int mode)
{
    GlobalMode = mode;
}



/*
 * Returns the value of the requested pid
 *
 */
PID ELM327OBD::getPID(int pid)
{
    PID rtPID(GlobalMode, pid,elm);
    rtPID.run();
    cout<<"c"<<endl;
    return rtPID;

}//getPID()



/*
 * Set up PID Mask for returning multiple PID's
 * with:
 * ELM327OBD::vector<quadoct> getPIDVec()
 *
 */
int ELM327OBD::setModeOneMask(quadoct pidMask)
{
}




quadoct ELM327OBD::getModeOneMask()
{
}




vector<quadoct> ELM327OBD::getPIDVec()
{
}




int ELM327OBD::setReqModeOnePIDSMap(quadoct pidMap)
{
}
