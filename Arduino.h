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

#include <default_gui_model.h>
#include <qstring.h>
#include <ostream>
#include <fstream>
#include <stdio.h> // Standard input/output definitions
#include <stdlib.h>
#include <string.h> // String function definitions
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "arduino_serial.h"

#define buf_max 256

class Arduino : public DefaultGUIModel
{

public:

    Arduino(void);
    virtual ~Arduino(void);

    virtual void execute(void);

protected:

    virtual void update(DefaultGUIModel::update_flags_t);

private:

    void OpenFile(QString);

    QStringList::Iterator it;
    QString FileName;
    QMessageBox FileOpenMessageBox;
    QFileDialog FileOpenDialog;
    std::fstream File;

    char FileOK;
    char Done;


	//char const serialport[buf_max];
	char buf[buf_max];
	int rc,n;

	int fd;// = -1;
	int baudrate;// = 9600; // default
	char quiet;//=0;
	char eolchar;// = '\n';
	int timeout;// = 5000;
	int in0, in1, in2, in3, in4, in5;
	int Arduino_time;
	double Time;

    double i;
};
