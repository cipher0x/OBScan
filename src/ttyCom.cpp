
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
    serial_stream.unsetf( std::ios_base::skipws );
    serial_stream << out;
    
    return 0;
}



/*
 * Read one line from tty
 */
string ttyCom::read()
{
    serial_stream.sync_with_stdio(false);
    
    string bldr="";
    char in;
    
    //configure iostream to not scip whitespace
    serial_stream.unsetf( std::ios_base::skipws );
    
    int cnt = 0;
    int maxCount = 1000;//timeout
    while((serial_stream.rdbuf()->in_avail() == 0))
    {
        usleep(10);
        cnt++;
    }//while
    
    //cout<<"count: "<<cnt<<endl;
   if((maxCount == cnt)) return string("");
    
    while(serial_stream.IsOpen())
    {
        serial_stream >> in;
        bldr += string(1,in);
        //cout<<in<<endl;
        
        if(in=='\r' || in=='\n' || in=='\0')
        {
            break;
        }
        
    }//while
    
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
    serial_stream.Open( "/dev/ttyS0" ) ;
    serial_stream.SetBaudRate(SerialStreamBuf::BAUD_115200);
    serial_stream.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
    serial_stream.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    serial_stream.SetNumOfStopBits(1) ;

    //correct error with in_avail()==0
    serial_stream.sync_with_stdio(false);
    */
    
    string tmp = "";
    
    //set device file
    serial_stream.Open(ttyConfig.getDevice());
    if ( ! serial_stream.good() ) 
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
    
    serial_stream.SetBaudRate( SerialStreamBuf::BAUD_115200 ) ;
    if ( ! serial_stream.good() ) 
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
    
     serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_stream.good() ) 
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }//data bits
    
    //Disable Parity
    serial_stream.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_stream.good() ) 
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    
    //Set Number of stop bits
    serial_stream.SetNumOfStopBits( 1 ) ;
    if ( ! serial_stream.good() ) 
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    
    
    //Disable Hardware flow control
    serial_stream.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_stream.good() ) 
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }
}//parseConfig
