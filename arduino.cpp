/*
=========================================================================
Module Name: Arduino

Module Version: v1.0

Date: 2011-10-07

Created by: Theoden Netoff

Description: This module records and .evt file.

Analysis File: none

References: none

Notes:

Changelog:
	-v1.0-Initial version of the module.

=========================================================================
*/

#include <arduino.h>

extern "C" Plugin::Object *createRTXIPlugin(void) 
{
    return new Arduino();
}

static DefaultGUIModel::variable_t vars[] = 
{ 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//INPUTS

//  {"Cell 0 State","0 - 4",DefaultGUIModel::INPUT,},

//OUTPUTS
    
    {"ArduinoSerial0","Int",DefaultGUIModel::OUTPUT,},
    {"ArduinoSerial1","Int",DefaultGUIModel::OUTPUT,},
    {"ArduinoSerial2","Int",DefaultGUIModel::OUTPUT,},
    {"ArduinoSerial3","Int",DefaultGUIModel::OUTPUT,},
    {"ArduinoSerial4","Int",DefaultGUIModel::OUTPUT,},
    {"ArduinoSerial5","Int",DefaultGUIModel::OUTPUT,},
//PARAMETERS

//  {"FileName","Output File Name",DefaultGUIModel::PARAMETER,},

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
};

static size_t num_vars = sizeof(vars)/sizeof(DefaultGUIModel::variable_t);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Arduino::Arduino(void)
  : DefaultGUIModel("Arduino",::vars,::num_vars), FileName("Please input filename.")
{
    createGUI(vars, num_vars);
    update(INIT);
    Time = 0;
    FileOK=0;

	fd = -1;
	baudrate = 57600; // default
	quiet=1;
	eolchar = '\n';
	timeout = 5000;
	//strcpy(serialport, '\\dev\\ttyUSB0');
	//serialport='\dev\ttyUSB0';
	fd = serialport_init("/dev/ttyUSB0", baudrate);
	if( fd==-1 ) printf("couldn't open port\n");
	if(!quiet) printf("opened port /dev/ttyUSB0\n");
	serialport_flush(fd);

    refresh();
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
}

Arduino::~Arduino(void) {}
/*
void Arduino::OpenFile(QString FName)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{
  if(FileName=="Please check filename!" or FileName=="Please input filename.")
  {
  printf("Please check filename!\n");
  return;
  }

  File.open((const char *)FName, std::fstream::out | std::fstream::binary);
     if(File.is_open())
     {
     printf("Open new file successful!\n");
     Time=0;
     FileOK=1;
     }
     else
     {
     printf("Please check filename!\n");
     FileName = "Please check filename!";
     setParameter("FileName",FileName);
     FileOK=0;
     }
     return;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     
} 
*/ 

void Arduino::execute(void)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
{
	if( fd == -1 ) printf("serial port not opened");
	memset(buf,0,buf_max); 
	eolchar = '\n';
	serialport_read_until(fd, buf, ',', buf_max, timeout);
	Arduino_time = atoi(buf);
	if( !quiet ) 
	{	printf("time:");
		printf("%s\t", buf);
	}
	serialport_read_until(fd, buf, ',', buf_max, timeout);
	in0=atoi(buf);
	if( !quiet ) 
	{	printf("X:");
		printf("%s\t", buf);
	}
	serialport_read_until(fd, buf, ',', buf_max, timeout);
	in1=atoi(buf);
	if( !quiet ) 
	{	printf("Y:");
		printf("%s\t", buf);
	}
	serialport_read_until(fd, buf, ',', buf_max, timeout);
	in2=atoi(buf);
	if( !quiet ) 
	{	printf("Z:");
		printf("%s\n", buf);
	}
	serialport_read_until(fd, buf, ',', buf_max, timeout);
	in3=atoi(buf);
	if( !quiet ) 
	{	printf("in3:");
		printf("%s\n", buf);
	}
	serialport_read_until(fd, buf, ',', buf_max, timeout);
	in4=atoi(buf);
	if( !quiet ) 
	{	printf("in4:");
		printf("%s\n", buf);
	}
	serialport_read_until(fd, buf, ';', buf_max, timeout);
	in5=atoi(buf);
	if( !quiet ) 
	{	printf("in5:");
		printf("%s\n", buf);
	}
	serialport_read_until(fd, buf, '\n', buf_max, timeout);
	output(0)=((double)in0-128.0)/256.0;
	output(1)=((double)in1-128.0)/256.0;
	output(2)=((double)in2-128.0)/256.0;
	output(3)=((double)in3-128.0)/256.0;
	output(4)=((double)in4-128.0)/256.0;
	output(5)=((double)in5-128.0)/256.0;
//  if(FileOK==1)
//  {
//  Time+= RT::System::getInstance()->getPeriod() * 1e-9;
//  for(i=0; i<4; i++)
//    {
//      if(input((int)i)==1)
//	{
//	  printf("Time=%f, Cell=%f\n",Time,i);
//	  File.write(reinterpret_cast<char *>(&Time), sizeof(double));
//	  File.write(reinterpret_cast<char *>(&i), sizeof(double));/
//	}
//     }
//   }
  return;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
}

void Arduino::update(DefaultGUIModel::update_flags_t flag)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
{
    switch(flag) {
      case INIT:
//	setParameter("FileName",FileName);
	break;

      case MODIFY:
//	FileName = getParameter("FileName");
//	OpenFile(FileName);
	break;

      case PAUSE:
/*
	if(getActive())
	{
	  printf("UnPausing Arduino.\n");
	}
      
        if((File.is_open())&&getActive()==false)
	{
	Done=FileOpenMessageBox.question(0,tr("Recording Ended?"), tr("You have paused the recording\nWould you like to continue with this file or close it?"),tr("&Contiue"),tr("&Close"), QString::null, 0, 1 );
	  if(Done==1)
	    {
	      File.close();
	      FileName="Please input filename.";
	      setParameter("FileName",FileName);
	      Time = 0;
            FileOK = 0;
	    }
	}
*/
     break;
    
    default:
      break;
    }
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     
}
