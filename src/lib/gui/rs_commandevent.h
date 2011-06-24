/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by 
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/


#ifndef RS_COMMANDEVENT_H
#define RS_COMMANDEVENT_H

/**
 * Command Events.
 */
class RS_CommandEvent {
public:
    /**
	 * Creates a new command event that is not yet accepted.
	 *
	 * @param cmd the command that was triggered.
	 */
    RS_CommandEvent(const QString& cmd) {
        this->cmd = cmd;
		accepted = false;
    }
 
    /**
	 * @return the command that was triggered, usually by
	 * the user.
	 */
    QString getCommand() {
        return cmd;
    }

    /**
	 * Sets the event state to accepted.
	 */
	void accept() {
		accepted = true;
	}

    /**
	 * @return Whether the event was already accepted or not.
	 */
	bool isAccepted() {
		return accepted;
	}
 
protected:
    QString cmd;
	bool accepted;
};

#endif
