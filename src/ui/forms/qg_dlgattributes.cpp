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
#include "qg_dlgattributes.h"

/*#include <qvariant.h>
#include "rs_graphic.h"
#include "rs_layer.h"
#include "qg_widgetpen.h"
#include "qg_layerbox.h"*/

/*
 *  Constructs a QG_DlgAttributes as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
QG_DlgAttributes::QG_DlgAttributes(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_DlgAttributes::~QG_DlgAttributes()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_DlgAttributes::languageChange()
{
    retranslateUi(this);
}

void QG_DlgAttributes::setData(RS_AttributesData* data, RS_LayerList& layerList) {
    this->data = data;

    //pen = line->getPen();
    wPen->setPen(data->pen, true, true, "Pen");

    //RS_Graphic* graphic = line->getGraphic();
    //if (graphic!=NULL) {
        cbLayer->init(layerList, false, true);
    //}
    //cbLayer->setLayer(data->layer);
    //RS_Layer* lay = line->getLayer(false);
    //if (lay!=NULL) {
    //    cbLayer->setLayer(*lay);
    //}
}

void QG_DlgAttributes::updateData() {
    data->pen = wPen->getPen();
    data->layer = cbLayer->currentText();

    data->changeColor = !wPen->isColorUnchanged();
    data->changeLineType = !wPen->isLineTypeUnchanged();
    data->changeWidth = !wPen->isWidthUnchanged();

    data->changeLayer = !cbLayer->isUnchanged();
}

