/*
	 Copyright (C) 2011 Georgia Institute of Technology

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

#include <default_gui_model.h>
#include <qstring.h>
#include <ostream>
#include <fstream>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
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

		//char const serialport[buf_max];
		char buf[buf_max];
		int rc,n;
		int fd;
		int baudrate;
		char quiet;
		char eolchar;
		int timeout;
		int in0, in1, in2, in3, in4, in5;
		int Arduino_time;
		double Time;
		double i;
};
