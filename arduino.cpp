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

std::list<std::string> getComList();

extern "C" Plugin::Object *createRTXIPlugin(void) 
{
	return new Arduino();
}

static DefaultGUIModel::variable_t vars[] = 
{
	{"Baud Rate", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER,},
	{"Timeout", "Connection Timeout", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER,},
	{"Sampling Rate", "Hz", DefaultGUIModel::PARAMETER, },
	{"Data","Int", DefaultGUIModel::OUTPUT,},
};

static size_t num_vars = sizeof(vars)/sizeof(DefaultGUIModel::variable_t);

Arduino::Arduino(void) : DefaultGUIModel("Arduino",::vars,::num_vars)
{
	createGUI(vars, num_vars);
	update(INIT);
	customizeGUI();
	refresh();
	QTimer::singleShot(0, this, SLOT(resizeMe()));

	timer = new QTimer();
	timer->setTimerType(Qt::CoarseTimer);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(acquireData()));
}

Arduino::~Arduino(void) { }

/*
 * No data acquisition takes place in execute due to the 
 * non-RT nature of serial port connections
 */
void Arduino::execute(void)
{
}

/*
 * Skeleton function for basic acquisition of data from connected Arduino
 * Users should customize parsing of incoming data based upon their Arduino's
 * specific configuration and serial settings.
 */
void Arduino::acquireData(void)
{
	if(fd != -1)
	{
		serialport_read_until(fd, buf, ',', buf_max, timeout);

		serialport_read_until(fd, buf, ';', buf_max, timeout);

		serialport_read_until(fd, buf, '\n', buf_max, timeout);

		output(0) = ((double)in0-128.0)/256.0;
	}
}

void Arduino::update(DefaultGUIModel::update_flags_t flag)
{
	switch(flag) {
		case INIT:
			fd = -1;
			fs = 10;
			baudrate = 9600;
			timeout = 5000;
			eolchar = '\n';
			memset(buf,0,buf_max); 
			setParameter("Baud Rate", baudrate);
			setParameter("Timeout", timeout);
			setParameter("Sampling Rate", fs);
			break;

		case MODIFY:
			baudrate = getParameter("Baud Rate").toInt();
			timeout = getParameter("Timeout").toInt();
			fs = getParameter("Sampling Rate").toDouble();
			break;

		case PAUSE:
			break;

		case UNPAUSE:
			timer->start(1.0/fs);
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
	portList = new QComboBox;
	modeBoxLayout->addWidget(portList);

	// Retrive list of available serial ports
	std::list<std::string> listOfPorts = getComList();
	size_t i = 0;
	for(std::list<std::string>::iterator it = listOfPorts.begin(); it != listOfPorts.end(); it++)
	{
		portList->insertItem(i, QString::fromUtf8(std::string(*it).c_str()));
		i++;
	}

	// Connect button
	QPushButton *connectButton = new QPushButton("Connect");
	customlayout->addWidget(connectButton);
	QObject::connect(connectButton, SIGNAL(clicked()), this, SLOT(connectArduino()));
	modeBoxLayout->addWidget(connectButton);

	// Status bar
	QGroupBox *status_group = new QGroupBox;
	QHBoxLayout *status_layout = new QHBoxLayout;
	status_group->setLayout(status_layout);
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	statusBar->showMessage(tr("Not connected to device."));
	status_layout->addWidget(statusBar, 2, 0);

	customlayout->addWidget(modeBox, 0,0);
	customlayout->addWidget(status_group, 2, 0);
	setLayout(customlayout);
}

void Arduino::connectArduino()
{
	if(fd = serialport_init(portList->currentText().toStdString().c_str(), baudrate))
	{
		statusBar->showMessage(tr("Connected."));
		serialport_flush(fd);
	}
	else
	{
		statusBar->showMessage(tr("Connection failed. Check port configuration."));
	}
}

std::list<std::string> getComList() {
	int n;
	struct dirent **namelist;
	const char* sysdir = "/dev/";
	std::string devicedir = sysdir;
	std::list<std::string> comList;

	// Scan through /sys/class/tty - it contains all tty-devices in the system
	n = scandir(sysdir, &namelist, NULL, NULL);
	if (n < 0)
		perror("scandir");
	else {
		while (n--) {
			if (strstr(namelist[n]->d_name,"tty"))
				comList.push_back(devicedir + namelist[n]->d_name);
		}
		free(namelist);
	}

	// Return the lsit of detected comports
	return comList;
}
