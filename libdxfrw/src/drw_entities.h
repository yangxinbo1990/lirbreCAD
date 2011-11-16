/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#ifndef DRW_ENTITIES_H
#define DRW_ENTITIES_H


#include <string>

class dxfReader;

using std::string;

namespace DRW {
    //! Vertical alignments.
    enum VAlign {
        VAlignTop,      /*!< Top. */
        VAlignMiddle,   /*!< Middle */
        VAlignBottom    /*!< Bottom */
    };

    //! Horizontal alignments.
    enum HAlign {
        HAlignLeft,     /*!< Left */
        HAlignCenter,   /*!< Centered */
        HAlignRight     /*!< Right */
    };


   //! Entity's type.
    enum ETYPE {
        POINT,
        LINE,
        CIRCLE,
        ARC,
        CONSTRUCTIONLINE,
        ELLIPSE,
        IMAGE,
        OVERLAYBOX,
        SOLID,
        TEXT,
        INSERT,
        POLYLINE,
        SPLINE,
        HATCH,
        DIMLEADER,
        DIMALIGNED,
        DIMLINEAR,
        DIMRADIAL,
        DIMDIAMETRIC,
        DIMANGULAR,
        UNKNOWN
    };

    enum LWEIGHT {
        L0=0,
        L1,
        L2,
        L3,
        L4,
        L5,
        L6,
        L7
    };

}

//! Base class for entities
/*!
*  Base class for entities
*  @author Rallaz
*/
class DRW_Entity {
public:
    //initializes default values
    DRW_Entity() {
        lineType = "BYLAYER";
        color = 256; // default BYLAYER (256)
        ltypeScale = 1.0;
        visible = true;
        layer = "0";
    }

protected:
    void parseCode(int code, dxfReader *reader);

public:
    enum DRW::ETYPE eType;     /*!< enum: entity type, code 0 */
    string handle;             /*!< entity identifier, code 5 */
    string handleBlock;        /*!< Soft-pointer ID/handle to owner BLOCK_RECORD object, code 330 */
    string layer;              /*!< layer name, code 8 */
    string lineType;           /*!< line type, code 6 */
    int color;                 /*!< entity color, code 62 */
    //RLZ: TODO as integer or enum??
    int lWeight;               /*!< entity lineweight, code 370 */
//    enum DRW::LWEIGHT lWeight; /*!< entity lineweight, code 370 */
    double ltypeScale;         /*!< linetype scale, code 48 */
    bool visible;              /*!< entity visibility, code 60 */
    int color24;               /*!< 24-bit color, code 420 */
    string colorName;          /*!< color name, code 430 */
};


//! Class to handle point entity
/*!
*  Class to handle point entity
*  @author Rallaz
*/
class DRW_Point : public DRW_Entity {
public:
    DRW_Point() {
        eType = DRW::POINT;
        z = ex = ey = 0;
        ez = 1;
        thickness = 0;
    }

    void parseCode(int code, dxfReader *reader);

public:
    double x;                 /*!< x coordinate, code 10 */
    double y;                 /*!< y coordinate, code 20 */
    double z;                 /*!< z coordinate, code 30 */
    double thickness;         /*!< thickness, code 39 */
    double ex;                /*!< x extrusion coordinate, code 210 */
    double ey;                /*!< y extrusion coordinate, code 220 */
    double ez;                /*!< z extrusion coordinate, code 230 */
};

//! Class to handle line entity
/*!
*  Class to handle line entity
*  @author Rallaz
*/
class DRW_Line : public DRW_Point {
public:
    DRW_Line() {
        eType = DRW::LINE;
        bz = 0;
    }

    void parseCode(int code, dxfReader *reader);

public:
    double bx;                 /*!< x coordinate, code 11 */
    double by;                 /*!< y coordinate, code 21 */
    double bz;                 /*!< z coordinate, code 31 */
};

//! Class to handle circle entity
/*!
*  Class to handle circle entity
*  @author Rallaz
*/
class DRW_Circle : public DRW_Point {
public:
    DRW_Circle() {
        eType = DRW::CIRCLE;
    }

    void parseCode(int code, dxfReader *reader);

public:
    double radious;                 /*!< x coordinate, code 11 */
};

//! Class to handle arc entity
/*!
*  Class to handle arc entity
*  @author Rallaz
*/
class DRW_Arc : public DRW_Circle {
public:
    DRW_Arc() {
        eType = DRW::ARC;
    }

    void parseCode(int code, dxfReader *reader);

public:
    double staangle;               /*!< x coordinate, code 50 */
    double endangle;               /*!< x coordinate, code 51 */
};



#endif

// EOF

