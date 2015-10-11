/*
	 Copyright (C) 2011 Georgia Institute of Technology

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

/*
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
 */

#include <arduino.h>
#include <dirent.h>

std::list<std::string> getComList();

extern "C" Plugin::Object *createRTXIPlugin(void) 
{
	return new Arduino();
}

static DefaultGUIModel::variable_t vars[] = 
{
	{"Baud Rate", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER,},
	{"ArduinoSerial0","Int", DefaultGUIModel::OUTPUT,},
	{"ArduinoSerial1","Int", DefaultGUIModel::OUTPUT,},
	{"ArduinoSerial2","Int", DefaultGUIModel::OUTPUT,},
	{"ArduinoSerial3","Int", DefaultGUIModel::OUTPUT,},
	{"ArduinoSerial4","Int", DefaultGUIModel::OUTPUT,},
	{"ArduinoSerial5","Int", DefaultGUIModel::OUTPUT,},
};

static size_t num_vars = sizeof(vars)/sizeof(DefaultGUIModel::variable_t);

Arduino::Arduino(void) : DefaultGUIModel("Arduino",::vars,::num_vars)
{
	createGUI(vars, num_vars);
	customizeGUI();
	update(INIT);
	refresh();
	resizeMe();
}

Arduino::~Arduino(void) {}

void Arduino::execute(void)
{
	if(fd != -1)
	{
		serialport_read_until(fd, buf, ',', buf_max, timeout);
		Arduino_time = atoi(buf);
		if( !quiet ) 
		{	
			printf("time:");
			printf("%s\t", buf);
		}

		serialport_read_until(fd, buf, ',', buf_max, timeout);
		in0=atoi(buf);
		if( !quiet ) 
		{	
			printf("X:");
			printf("%s\t", buf);
		}

		serialport_read_until(fd, buf, ',', buf_max, timeout);
		in1=atoi(buf);
		if( !quiet ) 
		{	
			printf("Y:");
			printf("%s\t", buf);
		}

		serialport_read_until(fd, buf, ',', buf_max, timeout);
		in2=atoi(buf);
		if( !quiet ) 
		{	
			printf("Z:");
			printf("%s\n", buf);
		}

		serialport_read_until(fd, buf, ',', buf_max, timeout);
		in3=atoi(buf);
		if( !quiet ) 
		{	
			printf("in3:");
			printf("%s\n", buf);
		}

		serialport_read_until(fd, buf, ',', buf_max, timeout);
		in4=atoi(buf);
		if( !quiet ) 
		{	
			printf("in4:");
			printf("%s\n", buf);
		}

		serialport_read_until(fd, buf, ';', buf_max, timeout);
		in5=atoi(buf);
		if( !quiet ) 
		{	
			printf("in5:");
			printf("%s\n", buf);
		}
		serialport_read_until(fd, buf, '\n', buf_max, timeout);
		output(0)=((double)in0-128.0)/256.0;
		output(1)=((double)in1-128.0)/256.0;
		output(2)=((double)in2-128.0)/256.0;
		output(3)=((double)in3-128.0)/256.0;
		output(4)=((double)in4-128.0)/256.0;
		output(5)=((double)in5-128.0)/256.0;
	}
}

void Arduino::update(DefaultGUIModel::update_flags_t flag)
{
	switch(flag) {
		case INIT:
			fd = -1;
			baudrate = 9600;
			quiet = 1;
			eolchar = '\n';
			timeout = 5000;
			serialport_flush(fd);
			memset(buf,0,buf_max); 
			break;

		case MODIFY:
			fd = serialport_init("/dev/ttyACM0", baudrate);
			if(fd == -1)
				printf("couldn't open port\n");
			if(!quiet)
				printf("opened port /dev/ttyUSB0\n");
			break;

		case PAUSE:
			break;

		default:
			break;
	}
}

void Arduino::customizeGUI(void)
{
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	
	QGroupBox *modeBox = new QGroupBox("Port:");
	QVBoxLayout *modeBoxLayout = new QVBoxLayout(modeBox);
	waveShape = new QComboBox;
	modeBoxLayout->addWidget(waveShape);

	// Retrive list of available serial ports
	std::list<std::string> listOfPorts = getComList();
	for(std::list<std::string>::iterator it = listOfPorts.begin(); it != listOfPorts.end(); it++)
		waveShape->insertItem(i, QString::fromUtf8(std::string(*it).c_str()));

	// Connect button
	QPushButton *connectButton = new QPushButton("Connect");
	customlayout->addWidget(connectButton);
	QObject::connect(connectButton, SIGNAL(clicked()), this, SLOT(connectArduino()));
	modeBoxLayout->addWidget(connectButton);

	customlayout->addWidget(modeBox, 0,0);
	setLayout(customlayout);
}

void Arduino::connectArduino()
{
	printf("Connect...\n");
}

std::list<std::string> getComList() {
	int n;
	struct dirent **namelist;
	const char* sysdir = "/sys/class/tty/";
	std::string devicedir = sysdir;
	std::list<std::string> comList;

	// Scan through /sys/class/tty - it contains all tty-devices in the system
	n = scandir(sysdir, &namelist, NULL, NULL);
	if (n < 0)
		perror("scandir");
	else {
		while (n--) {
			if (strcmp(namelist[n]->d_name,"..") && strcmp(namelist[n]->d_name,".")) {
				comList.push_back(devicedir + namelist[n]->d_name);
			}
		}
		free(namelist);
	}

	// Return the lsit of detected comports
	return comList;
}
