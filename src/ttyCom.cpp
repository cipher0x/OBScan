
#include "ttyCom.hpp"



ttyCom::ttyCom()
{
}



ttyCom::ttyCom(cfgproc &ttyConfig)
{
    //loads configureation into serail steram object from ttyConfig object
    loadConfig(ttyConfig);
}




ttyCom::~ttyCom()
{
}



void ttyCom::loadConfig(cfgproc &ttyConfig)
{
    //loads configureation into serail steram object from ttyConfig object
    parseConfig(ttyConfig);
}



/*
 * Write string to tty
 */
int ttyCom::write(string &out)
{
    serial_port.Write(out);

    return 0;
}



/*
 * Read one line from tty
 */
string ttyCom::read()
{
    string bldr="";
    char in;
    int hexVal;

    //serial_port >> in;
    if(serial_port.IsOpen())
    {
      usleep(1);
      try
      {
        serial_port.ReadLine(bldr,'\r',15000);
      }
      catch(LibSerial::ReadTimeout)
      {
        bldr="TIME OUT";
      }


      hexVal = (unsigned int)in;
    }

    return bldr;
}


/*
 * Loads serial stream settings from the cfgProc object
 *
 * Need to complete this section to configure all options based on the cfgproc config object
 * Currently only configures tty for default communication to ELM327
 *
 */
int ttyCom::parseConfig(cfgproc ttyConfig)
{
    /*
    serial_port.Open( "/dev/ttyS0" ) ;
    serial_port.SetBaudRate(SerialStreamBuf::BAUD_115200);
    serial_port.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    serial_port.SetNumOfStopBits(1) ;

    //correct error with in_avail()==0
    serial_port.sync_with_stdio(false);
    */

    string tmp = "";

    //set device file
    serial_port.Open(ttyConfig.getDevice(),std::ios::in|std::ios::out);
    if ( ! serial_port.IsOpen() )
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Could not open serial port device file error: "<< ttyConfig.getDevice()
                  << std::endl ;
        exit(1) ;
    }//set device fail

    //set baud SetBaudRate
    tmp = ttyConfig.getBaud();
    if(tmp!="115200")//currently only suppored BAUD Rate************************************************************
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Unsuppored BAUD Rate: ." << tmp
                  << std::endl ;
        exit(1) ;
    }//BAUD

    serial_port.SetBaudRate( BaudRate::BAUD_115200 ) ;
    if ( ! serial_port.IsOpen() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }

    //set data bits
    tmp = ttyConfig.getData_Bits();
    if(tmp != "8")//only suport 8 bits*******************************************************************************
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Unsuppored Data Bits: ." << tmp
                  << std::endl ;
        exit(1) ;
    }//data bits

     serial_port.SetCharacterSize( CharacterSize::CHAR_SIZE_8 ) ;
    if ( ! serial_port.IsOpen() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }//data bits

    //Disable Parity
    serial_port.SetParity( Parity::PARITY_NONE ) ;
    if ( ! serial_port.IsOpen() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }

    //Set Number of stop bits
    serial_port.SetStopBits( StopBits::STOP_BITS_1 ) ;
    if ( ! serial_port.IsOpen() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }


    //Disable Hardware flow control
    serial_port.SetFlowControl( FlowControl::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.IsOpen() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }
}//parseConfig
