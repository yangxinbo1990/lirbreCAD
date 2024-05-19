/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software 
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
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

#ifndef RS_ACTIONMODIFYBEVEL_H
#define RS_ACTIONMODIFYBEVEL_H

#include<memory>

#include "rs_previewactioninterface.h"

class RS_Entity;

/**
 * This action class can handle user events to bevel corners.
 *
 * @author Andrew Mustun
 */
class RS_ActionModifyBevel : public RS_PreviewActionInterface {
    Q_OBJECT
    /**
     * Action States.
     */
    enum Status {
        SetEntity1,      /**< Choosing the 1st entity. */
        SetEntity2,      /**< Choosing the 2nd entity. */
        SetLength1,      /**< Setting length 1 in command line. */
        SetLength2       /**< Setting length 2 in command line. */
    };

public:
    RS_ActionModifyBevel(RS_EntityContainer& container,
                         RS_GraphicView& graphicView);
    ~RS_ActionModifyBevel() override;

    void init(int status) override;
    void trigger() override;
    void finish(bool updateTB = false) override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void commandEvent(RS_CommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void hideOptions() override;
    void showOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

    void setLength1(double l1);

    double getLength1() const;

    void setLength2(double l2);

    double getLength2() const;

    void setTrim(bool t);

    bool isTrimOn() const;

private:
    // update highlight status
    void unhighlightEntity();

    RS_Entity* entity1 = nullptr;
    RS_Entity* entity2 = nullptr;
    struct Points;
    std::unique_ptr<Points> pPoints;
    /** Last status before entering angle. */
    Status lastStatus = SetEntity1;
};
#endif
