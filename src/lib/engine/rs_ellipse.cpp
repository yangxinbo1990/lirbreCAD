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


#include "rs_ellipse.h"

#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "rs_information.h"
#include "rs_linetypepattern.h"

/**
 * Constructor.
 */
RS_Ellipse::RS_Ellipse(RS_EntityContainer* parent,
                       const RS_EllipseData& d)
    :RS_AtomicEntity(parent), data(d) {

    //calculateEndpoints();
    calculateBorders();
}


/**
 * Recalculates the endpoints using the angles and the radius.
 */
/*
void RS_Ellipse::calculateEndpoints() {
   double angle = data.majorP.angle();
   double radius1 = getMajorRadius();
   double radius2 = getMinorRadius();

   startpoint.set(data.center.x + cos(data.angle1) * radius1,
                  data.center.y + sin(data.angle1) * radius2);
   startpoint.rotate(data.center, angle);
   endpoint.set(data.center.x + cos(data.angle2) * radius1,
                data.center.y + sin(data.angle2) * radius2);
   endpoint.rotate(data.center, angle);
}
*/


/**
 * Calculates the boundary box of this ellipse.
 *
 * @todo Fix that - the algorithm used is really bad / slow.
 */
void RS_Ellipse::calculateBorders() {
    RS_DEBUG->print("RS_Ellipse::calculateBorders");

    double radius1 = getMajorRadius();
    double radius2 = getMinorRadius();
    double angle = getAngle();
    //double a1 = ((!isReversed()) ? data.angle1 : data.angle2);
    //double a2 = ((!isReversed()) ? data.angle2 : data.angle1);
    RS_Vector startpoint = getStartpoint();
    RS_Vector endpoint = getEndpoint();

    double minX = std::min(startpoint.x, endpoint.x);
    double minY = std::min(startpoint.y, endpoint.y);
    double maxX = std::max(startpoint.x, endpoint.x);
    double maxY = std::max(startpoint.y, endpoint.y);

    RS_Vector vp;
    // kind of a brute force. TODO: exact calculation
//    double a = a1;

//    do {
//        vp.set(data.center.x + radius1 * cos(a),
//               data.center.y + radius2 * sin(a));
//        vp.rotate(data.center, angle);
//
//        minX = std::min(minX, vp.x);
//        minY = std::min(minY, vp.y);
//        maxX = std::max(maxX, vp.x);
//        maxY = std::max(maxY, vp.y);
//
//        a += 0.03;
//    } while (RS_Math::isAngleBetween(RS_Math::correctAngle(a), a1, a2, false) &&
//             a<4*M_PI);
//    std::cout<<"a1="<<a1<<"\ta2="<<a2<<std::endl<<"Old algorithm:\nminX="<<minX<<"\tmaxX="<<maxX<<"\nminY="<<minY<<"\tmaxY="<<maxY<<std::endl;

    // Exact algorithm, based on rotation:
    // ( r1*cos(a), r2*sin(a)) rotated by angle to
    // (r1*cos(a)*cos(angle)-r2*sin(a)*sin(angle),r1*cos(a)*sin(angle)+r2*sin(a)*cos(angle))
    // both coordinates can be further reorganized to the form rr*cos(a+ theta),
    // with rr and theta angle defined by the coordinates given above
    double amin,amax;
//      x range
    vp.set(radius1*cos(angle),radius2*sin(angle));

    amin=RS_Math::correctAngle(getAngle1()+vp.angle()); // to the range of 0 to 2*M_PI
    amax=RS_Math::correctAngle(getAngle2()+vp.angle()); // to the range of 0 to 2*M_PI
	if( RS_Math::isAngleBetween(M_PI,amin,amax,isReversed()) || fabs(amax-amin) < RS_TOLERANCE){ 
    //if( (amin<=M_PI && delta_a >= M_PI - amin) || (amin > M_PI && delta_a >= 3*M_PI - amin)) {
        minX= data.center.x-vp.magnitude();
	}
//    else
//       minX=data.center.x +vp.magnitude()*std::min(cos(amin),cos(amin+delta_a));
	if( RS_Math::isAngleBetween(2.*M_PI,amin,amax,isReversed()) || fabs(amax-amin) < RS_TOLERANCE){ 
    //if( delta_a >= 2*M_PI - amin ) {
        maxX= data.center.x+vp.magnitude();
	}
//    else
//       maxX= data.center.x+vp.magnitude()*std::max(cos(amin),cos(amin+delta_a));
//      y range
    vp.set(radius1*sin(angle),-1*radius2*cos(angle));
    amin=RS_Math::correctAngle(getAngle1()+vp.angle()); // to the range of 0 to 2*M_PI
    amax=RS_Math::correctAngle(getAngle2()+vp.angle()); // to the range of 0 to 2*M_PI
	if( RS_Math::isAngleBetween(M_PI,amin,amax,isReversed()) || fabs(amax-amin) < RS_TOLERANCE){ 
    //if( (amin<=M_PI &&delta_a >= M_PI - amin) || (amin > M_PI && delta_a >= 3*M_PI - amin)) {
        minY= data.center.y-vp.magnitude();
	}
//    else
//        minY=data.center.y +vp.magnitude()*std::min(cos(amin),cos(amin+delta_a));
	if( RS_Math::isAngleBetween(2.*M_PI,amin,amax,isReversed()) || fabs(amax-amin) < RS_TOLERANCE){ 
    //if( delta_a >= 2*M_PI - amin ) {
        maxY= data.center.y+vp.magnitude();
	}
//    else
//        maxY= data.center.y+vp.magnitude()*std::max(cos(amin),cos(amin+delta_a));
//std::cout<<"New algorithm:\nminX="<<minX<<"\tmaxX="<<maxX<<"\nminY="<<minY<<"\tmaxY="<<maxY<<std::endl;

    minV.set(minX, minY);
    maxV.set(maxX, maxY);
    RS_DEBUG->print("RS_Ellipse::calculateBorders: OK");
}



RS_VectorSolutions RS_Ellipse::getRefPoints() {
    RS_VectorSolutions ret(getStartpoint(), getEndpoint(), data.center);
    return ret;
}



RS_Vector RS_Ellipse::getNearestEndpoint(const RS_Vector& coord, double* dist) {
    double dist1, dist2;
    RS_Vector nearerPoint;
    RS_Vector startpoint = getStartpoint();
    RS_Vector endpoint = getEndpoint();

    dist1 = startpoint.distanceTo(coord);
    dist2 = endpoint.distanceTo(coord);

    if (dist2<dist1) {
        if (dist!=NULL) {
            *dist = dist2;
        }
        nearerPoint = endpoint;
    } else {
        if (dist!=NULL) {
            *dist = dist1;
        }
        nearerPoint = startpoint;
    }

    return nearerPoint;
}


//implemented using an analytical aglorithm
RS_Vector RS_Ellipse::getNearestPointOnEntity(const RS_Vector& coord,
        bool onEntity, double* dist, RS_Entity** entity)
{

    RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity");

    RS_Vector ret(false);

    if (entity!=NULL) {
        *entity = this;
    }
    ret.set(coord.x,coord.y);
    ret.move(-getCenter());
    ret.rotate(-getAngle());
    double x=ret.x,y=ret.y;
    double a=getMajorRadius();
    double b=getMinorRadius();
    //std::cout<<"(a= "<<a<<" b= "<<b<<" x= "<<x<<" y= "<<y<<" )\n";
    //std::cout<<"finding minimum for ("<<x<<"-"<<a<<"*cos(t))^2+("<<y<<"-"<<b<<"*sin(t))^2\n";
    double twoa2b2=2*(a*a-b*b);
    double twoax=2*a*x;
    double twoby=2*b*y;
    double a0=twoa2b2*twoa2b2;
    double ce[4];
    double roots[4];
    unsigned int counts=0;
    //need to handle a=b
    if(a0 > RS_TOLERANCE*RS_TOLERANCE ) { // a != b , ellipse
        ce[0]=-2.*twoax/twoa2b2;
        ce[1]= (twoax*twoax+twoby*twoby)/a0-1.;
        ce[2]= - ce[0];
        ce[3]= -twoax*twoax/a0;
        //std::cout<<"1::find cosine, variable c, solve(c^4 +("<<ce[0]<<")*c^3+("<<ce[1]<<")*c^2+("<<ce[2]<<")*c+("<<ce[3]<<")=0,c)\n";
        counts=RS_Math::quarticSolver(ce,roots);
    } else {//a=b, quadratic equation for circle
        counts=2;
        a0=twoby/twoax;
        roots[0]=sqrt(1./(1.+a0*a0));
        roots[1]=-roots[0];
    }
    if(!counts) {
        //this should not happen
    std::cout<<"(a= "<<a<<" b= "<<b<<" x= "<<x<<" y= "<<y<<" )\n";
    std::cout<<"finding minimum for ("<<x<<"-"<<a<<"*cos(t))^2+("<<y<<"-"<<b<<"*sin(t))^2\n";
        std::cout<<"2::find cosine, variable c, solve(c^4 +("<<ce[0]<<")*c^3+("<<ce[1]<<")*c^2+("<<ce[2]<<")*c+("<<ce[3]<<")=0,c)\n";
	std::cout<<ce[0]<<' '<<ce[1]<<' '<<ce[2]<<' '<<ce[3]<<std::endl;
        std::cerr<<"RS_Math::RS_Ellipse::getNearestPointOnEntity() finds no root from quartic, this should not happen\n";
    }

    RS_Vector vp2(false);
    double dDistance(RS_MAXDOUBLE);
    //double ea;
    for(unsigned int i=0; i<counts; i++) {
        if ( fabs(roots[i])>1.) continue;
        double s=twoby*roots[i]/(twoax-twoa2b2*roots[i]); //sine
        if (fabs(s) > 1. ) continue;
        double d2=twoa2b2*(1-2*roots[i]*roots[i])+twoax*roots[i]+twoby*s;
        if (d2<0) continue; // fartherest
        RS_Vector vp3;
        vp3.set(a*roots[i],b*s);
        double d=vp3.distanceTo(ret);
        //std::cout<<"Checking: cos= "<<roots[i]<<" sin= "<<s<<" angle= "<<atan2(roots[i],s)<<" minimum= "<<d<<std::endl;
        if( vp2.valid && d>dDistance) {
		continue;
		}
        vp2=vp3;
        dDistance=d;
//			ea=atan2(roots[i],s);
    }
    if( ! vp2.valid ) {
        //this should not happen
	std::cout<<ce[0]<<' '<<ce[1]<<' '<<ce[2]<<' '<<ce[3]<<std::endl;
        std::cerr<<"RS_Math::RS_Ellipse::getNearestPointOnEntity() finds no minimum, this should not happen\n";
    }
    if (dist!=NULL) {
        if (ret.valid) {
            *dist = dDistance;
        } else {
            *dist = RS_MAXDOUBLE;
        }
    }
    vp2.rotate(getAngle());
    vp2.move(getCenter());
    ret=vp2;
    if (onEntity) {
        if (!RS_Math::isAngleBetween(getEllipseAngle(ret), getAngle1(), getAngle2(), data.reversed)) {
            ret = RS_Vector(false);
        }
    }

//std::cout<<"New algorithm\nMinimum dist="<<dmin<<std::endl;
//std::cout<<"Nearest point: "<<vp2<<std::endl;


//
//
//    double ang = getAngle();
//
//    RS_Vector normalized = (coord - data.center).rotate(-ang);
//
//    double dU = normalized.x;
//    double dV = normalized.y;
//    double dA = getMajorRadius();
//    double dB = getMinorRadius();
//    double dEpsilon = 1.0e-8;
//    int iMax = 32;
//    int riIFinal = 0;
//    double rdX = 0.0;
//    double rdY = 0.0;
//    double dDistance;
//    bool swap = false;
//    bool majorSwap = false;
//
//    if (dA<dB) {
//        double dum = dA;
//        dA = dB;
//        dB = dum;
//        dum = dU;
//        dU = dV;
//        dV = dum;
//        majorSwap = true;
//    }
//
//    if (dV<0.0) {
//        dV*=-1.0;
//        swap = true;
//    }
//
//    // initial guess
//    double dT = dB*(dV - dB);
//
//    // Newton s method
//    int i;
//    for (i = 0; i < iMax; i++) {
//        RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: i: %d", i);
//        double dTpASqr = dT + dA*dA;
//        double dTpBSqr = dT + dB*dB;
//        double dInvTpASqr = 1.0/dTpASqr;
//        double dInvTpBSqr = 1.0/dTpBSqr;
//        double dXDivA = dA*dU*dInvTpASqr;
//        double dYDivB = dB*dV*dInvTpBSqr;
//        double dXDivASqr = dXDivA*dXDivA;
//        double dYDivBSqr = dYDivB*dYDivB;
//        double dF = dXDivASqr + dYDivBSqr - 1.0;
//        RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: dF: %f", dF);
//        if ( fabs(dF) < dEpsilon ) {
//            // F(t0) is close enough to zero, terminate the iteration:
//            rdX = dXDivA*dA;
//            rdY = dYDivB*dB;
//            riIFinal = i;
//            RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: rdX,rdY 1: %f,%f", rdX, rdY);
//            break;
//        }
//        double dFDer = 2.0*(dXDivASqr*dInvTpASqr + dYDivBSqr*dInvTpBSqr);
//        double dRatio = dF/dFDer;
//        RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: dRatio: %f", dRatio);
//        if ( fabs(dRatio) < dEpsilon ) {
//            // t1-t0 is close enough to zero, terminate the iteration:
//            rdX = dXDivA*dA;
//            rdY = dYDivB*dB;
//            riIFinal = i;
//            RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: rdX,rdY 2: %f,%f", rdX, rdY);
//            break;
//        }
//        dT += dRatio;
//    }
//    if ( i == iMax ) {
//        // failed to converge:
//        RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: failed");
//        dDistance = RS_MAXDOUBLE;
//    }
//    else {
//        double dDelta0 = rdX - dU;
//        double dDelta1 = rdY - dV;
//        dDistance = sqrt(dDelta0*dDelta0 + dDelta1*dDelta1);
//        ret = RS_Vector(rdX, rdY);
//        RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: rdX,rdY 2: %f,%f", rdX, rdY);
//        RS_DEBUG->print("RS_Ellipse::getNearestPointOnEntity: ret: %f,%f", ret.x, ret.y);
//    }
//    std::cout<<"Old algorithm: dist="<<dDistance<<" "<<ret<<std::endl;
//    if (dist!=NULL) {
//        if (ret.valid) {
//            *dist = dDistance;
//        } else {
//            *dist = RS_MAXDOUBLE;
//        }
//    }
//
//    if (ret.valid) {
//        if (swap) {
//            ret.y*=-1.0;
//        }
//        if (majorSwap) {
//            double dum = ret.x;
//            ret.x = ret.y;
//            ret.y = dum;
//        }
//        ret = (ret.rotate(ang) + data.center);
//
//        if (onEntity) {
//            double a1 = data.center.angleTo(getStartpoint());
//            double a2 = data.center.angleTo(getEndpoint());
//            double a = data.center.angleTo(ret);
//            if (!RS_Math::isAngleBetween(a, a1, a2, data.reversed)) {
//                ret = RS_Vector(false);
//            }
//        }
//    }
//    std::cout<<"Old algorithm\ndist="<<dDistance<<std::endl;
//    if(ret.valid) std::cout<<"coord="<<ret<<std::endl;

    return ret;
}




/**
 * @param tolerance Tolerance.
 *
 * @retval true if the given point is on this entity.
 * @retval false otherwise
 */
bool RS_Ellipse::isPointOnEntity(const RS_Vector& coord,
                                 double tolerance) {
    double dist = getDistanceToPoint(coord, NULL, RS2::ResolveNone);
    return (dist<=tolerance);
}



RS_Vector RS_Ellipse::getNearestCenter(const RS_Vector& coord,
                                       double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}



/**
 * a naive implementation of middle point
 * to accurately locate the middle point from arc length is possible by using elliptic integral to find the total arc length, then, using elliptic function to find the half length point
 */
RS_Vector RS_Ellipse::getNearestMiddle(const RS_Vector& coord,
                                       double* dist) {
    if ( RS_Math::isSameDirection(data.angle1,data.angle2, RS_TOLERANCE_ANGLE) ) { // no middle point for whole ellipse
        if (dist!=NULL) {
            *dist = RS_MAXDOUBLE;
        }
        return RS_Vector(false);

    }
    double amin,amax;
    RS_Vector vp;
    vp.setPolar(1.0,data.angle1);
    vp.scale(RS_Vector(1.0,data.ratio));
    amin=vp.angle();
    vp.setPolar(1.0,data.angle2);
    vp.scale(RS_Vector(1.0,data.ratio));
    amax=vp.angle();
    amin=0.5*(amin+amax);
    if (data.reversed ^ (amin > amax ) ) amin += M_PI; // condition to adjust one end by 2*M_PI, therefore, the middle point by M_PI
    vp.set(getMajorRadius()*cos(amin),getMinorRadius()*sin(amin));
    vp.rotate(getAngle());
    vp.move(data.center);
    if (dist!=NULL) {
        *dist = coord.distanceTo(vp);
    }
    RS_DEBUG->print("RS_Ellipse::getNearestMiddle: angle1=%g, angle2=%g, middle=%g\n",data.angle1,data.angle2,amin);
    return vp;
}



RS_Vector RS_Ellipse::getNearestDist(double /*distance*/,
                                     const RS_Vector& /*coord*/,
                                     double* dist) {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



double RS_Ellipse::getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity,
                                      RS2::ResolveLevel, double /*solidDist*/) {
    double dist = RS_MAXDOUBLE;
    getNearestPointOnEntity(coord, true, &dist, entity);

    // RVT 6 Jan 2011 : Add selection by center point
    float dToCenter=data.center.distanceTo(coord);

    if (dist<dToCenter) {
        return dist;
    } else {
        return dToCenter;
    }

}



void RS_Ellipse::move(RS_Vector offset) {
    data.center.move(offset);
    //calculateEndpoints();
    calculateBorders();
}



void RS_Ellipse::rotate(RS_Vector center, double angle) {
    data.center.rotate(center, angle);
    data.majorP.rotate(angle);
    //calculateEndpoints();
    calculateBorders();
}



void RS_Ellipse::moveStartpoint(const RS_Vector& pos) {
    data.angle1 = getEllipseAngle(pos);
    //data.angle1 = data.center.angleTo(pos);
    //calculateEndpoints();
    calculateBorders();
}



void RS_Ellipse::moveEndpoint(const RS_Vector& pos) {
    data.angle2 = getEllipseAngle(pos);
    //data.angle2 = data.center.angleTo(pos);
    //calculateEndpoints();
    calculateBorders();
}


RS2::Ending RS_Ellipse::getTrimPoint(const RS_Vector& coord,
                                     const RS_Vector& trimPoint) {

    //double angEl = getEllipseAngle(trimPoint);
    double angM = getEllipseAngle(coord);
    if (RS_Math::getAngleDifference(angM, data.angle1) > RS_Math::getAngleDifference(data.angle2,angM)) {
        return RS2::EndingStart;
    } else {
        return RS2::EndingEnd;
    }
//
//
//    if (RS_Math::getAngleDifference(angM, angEl)>M_PI) {
//        //if (data.reversed) {
//        //	return RS2::EndingEnd;
//        //}
//        //else {
//        return RS2::EndingStart;
//        //}
//    }
//    else {
//        //if (data.reversed) {
//        //	return RS2::EndingStart;
//        //}
//        //else {
//        return RS2::EndingEnd;
//        //}
//    }
}

double RS_Ellipse::getEllipseAngle(const RS_Vector& pos) {
    RS_Vector m = pos-data.center;
    m.rotate(-data.majorP.angle());
    m.scale(RS_Vector(data.ratio, 1.0));
    return m.angle();
}



void RS_Ellipse::scale(RS_Vector center, RS_Vector factor) {
    data.center.scale(center, factor);
    data.majorP.scale(factor);
    //calculateEndpoints();
    calculateBorders();
}


/**
 * @todo deal with angles correctly
 */
void RS_Ellipse::mirror(RS_Vector axisPoint1, RS_Vector axisPoint2) {
    RS_Vector mp = data.center + data.majorP;

    data.center.mirror(axisPoint1, axisPoint2);
    mp.mirror(axisPoint1, axisPoint2);

    data.majorP = mp - data.center;

    double a = axisPoint1.angleTo(axisPoint2);

    RS_Vector vec;
    vec.setPolar(1.0, data.angle1);
    vec.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle1 = vec.angle() - 2*a;

    vec.setPolar(1.0, data.angle2);
    vec.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle2 = vec.angle() - 2*a;

    data.reversed = (!data.reversed);

    //calculateEndpoints();
    calculateBorders();
}



void RS_Ellipse::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    RS_Vector startpoint = getStartpoint();
    RS_Vector endpoint = getEndpoint();

    if (ref.distanceTo(startpoint)<1.0e-4) {
        moveStartpoint(startpoint+offset);
    }
    if (ref.distanceTo(endpoint)<1.0e-4) {
        moveEndpoint(endpoint+offset);
    }
}


void RS_Ellipse::draw(RS_Painter* painter, RS_GraphicView* view, double /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }


    if (getPen().getLineType()==RS2::SolidLine ||
            isSelected() ||
            view->getDrawingMode()==RS2::ModePreview) {

        painter->drawEllipse(view->toGui(getCenter()),
                             getMajorRadius() * view->getFactor().x,
                             getMinorRadius() * view->getFactor().x,
                             getAngle(),
                             getAngle1(), getAngle2(),
                             isReversed());
    } else {
        double styleFactor = getStyleFactor(view);
        if (styleFactor<0.0) {
            painter->drawEllipse(view->toGui(getCenter()),
                                 getMajorRadius() * view->getFactor().x,
                                 getMinorRadius() * view->getFactor().x,
                                 getAngle(),
                                 getAngle1(), getAngle2(),
                                 isReversed());
            return;
        }

        // Pattern:
        RS_LineTypePattern* pat;
        if (isSelected()) {
            pat = &patternSelected;
        } else {
            pat = view->getPattern(getPen().getLineType());
        }

        if (pat==NULL) {
            return;
        }

        // Pen to draw pattern is always solid:
        RS_Pen pen = painter->getPen();
        pen.setLineType(RS2::SolidLine);
        painter->setPen(pen);

        double* da;     // array of distances in x.
        int i;          // index counter

        double length = getAngleLength();

        // create pattern:
        da = new double[pat->num];

        double tot=0.0;
        i=0;
        bool done = false;
        double curA = getAngle1();
        double curR;
        RS_Vector cp = view->toGui(getCenter());
        double r1 = getMajorRadius() * view->getFactor().x;
        double r2 = getMinorRadius() * view->getFactor().x;

        do {
            curR = sqrt(RS_Math::pow(getMinorRadius()*cos(curA), 2.0)
                        + RS_Math::pow(getMajorRadius()*sin(curA), 2.0));

            if (curR>1.0e-6) {
                da[i] = fabs(pat->pattern[i] * styleFactor) / curR;
                if (pat->pattern[i] * styleFactor > 0.0) {

                    if (tot+fabs(da[i])<length) {
                        painter->drawEllipse(cp,
                                             r1, r2,
                                             getAngle(),
                                             curA,
                                             curA + da[i],
                                             false);
                    } else {
                        painter->drawEllipse(cp,
                                             r1, r2,
                                             getAngle(),
                                             curA,
                                             getAngle2(),
                                             false);
                    }
                }
            }
            curA+=da[i];
            tot+=fabs(da[i]);
            done=tot>length;

            i++;
            if (i>=pat->num) {
                i=0;
            }
        } while(!done);

        delete[] da;
    }
}



/**
 * Dumps the point's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_Ellipse& a) {
    os << " Ellipse: " << a.data << "\n";
    return os;
}

