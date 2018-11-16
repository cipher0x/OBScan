#ifndef ELM327OBD_HPP
#define ELM327OBD_HPP

#include "ELM327.hpp"
#include "PID.hpp"

#include <string>
#include <vector>
#include <sstream>


typedef uint32_t quadoct;

class ELM327OBD
{
public:
    ELM327OBD();
    explicit ELM327OBD(ELM327 &elmin);
    ~ELM327OBD();
    int setELM327(ELM327 &elmin);
    int setMode(int mode);
    PID getPID(int pid);
    int setModeOneMask(quadoct pidMask);
    quadoct getModeOneMask();
    vector<quadoct> getPIDVec();
    int setReqModeOnePIDSMap(quadoct pidMap);

private:
    ELM327 *elm = nullptr;
    int GlobalMode = 0;

};


#endif
