/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2014 Dongxu Li (dongxuli2011@gmail.com)
** Copyright (C) 2014 Pavel Krejcir (pavel@pamsoft.cz)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**********************************************************************/


#ifndef LC_Parabola_H
#define LC_Parabola_H

#include <array>
#include <vector>
#include "lc_splinepoints.h"

class RS_Line;
struct RS_LineData;

/**
 * Holds the data that defines a line.
 * Few notes about implementation:
 * When drawing, the spline is defined via splinePoints collection.
 * However, since we want to allow trimming/cutting the spline,
 * we cannot guarantee that the shape would stay unchanged after
 * a part of the spline would be cut off. This would espetially be
 * obvious after cutting closed splines. So we introduce the "cut"
 * state. After that, all splinePoints will be deleted except start
 * and end points, and the controlPoints become the reference points
 * of that shape. It will be further possible to modify the spline,
 * but the control points will serve as handles then.
 */
struct LC_ParabolaData
{
    /**
    * Default constructor. Leaves the data object uninitialized.
    */
    static std::vector<LC_ParabolaData> From4Points(const std::vector<RS_Vector>& points);
    static LC_ParabolaData FromEndPointsTangents(
            const std::array<RS_Vector, 2>& endPoints,
            const std::array<RS_Vector, 2>& endTangents);
    LC_ParabolaData() = default;
    LC_ParabolaData(std::array<RS_Vector, 3> controlPoints);
    RS_LineData GetAxis() const;
    RS_LineData GetDirectrix() const;
    RS_Vector GetFocus() const;
    LC_ParabolaData& move(const RS_Vector& displacement);
    LC_ParabolaData& rotate(const RS_Vector& rotation);
    LC_ParabolaData& rotate(const RS_Vector& center, const RS_Vector& rotation);
    LC_ParabolaData& rotate(const RS_Vector& center, double angle);
    LC_ParabolaData& scale(double factor);
    LC_ParabolaData& scale(const RS_Vector& factor);
    LC_ParabolaData scaled(const RS_Vector& factor) const;
    LC_ParabolaData scaled(const RS_Vector& center, const RS_Vector& factor) const;
    LC_ParabolaData& mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    LC_ParabolaData mirrored(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) const;

    /** \brief return the equation of the entity
    a quadratic contains coefficients for quadratic:
    m0 x^2 + m1 xy + m2 y^2 + m3 x + m4 y + m5 =0

    for linear:
    m0 x + m1 y + m2 =0
    **/
    LC_Quadratic getQuadratic() const;

    // The three control points
    std::array<RS_Vector, 3> controlPoints;
    //
    RS_Vector focus;
    // a vector from the vertex to focus
    RS_Vector axis;
    RS_Vector vertex;
    bool valid = false;
private:
    void CalculatePrimitives();
};

std::ostream& operator << (std::ostream& os, const LC_ParabolaData& ld);

/**
 * Class for a parabola entity.
 *
 * @author Dongxu Li
 */
class LC_Parabola : public LC_SplinePoints // RS_EntityContainer
{
private:
    // /**
    //  * @brief mapDataToGui - map Data to Gui space for painter, which is ignorant about the view
    //  * @return LC_ParabolaData - in the Gui coordinates
    //  */
    // void UpdateControlPoints();
    // void UpdateQuadExtent(const RS_Vector& x1, const RS_Vector& c1, const RS_Vector& x2);
    // int GetNearestQuad(const RS_Vector& coord, double* dist, double* dt) const;
    // RS_Vector GetSplinePointAtDist(double dDist, int iStartSeg, double dStartT,
    //                                int *piSeg, double *pdt) const;
    // int GetQuadPoints(int iSeg, RS_Vector *pvStart, RS_Vector *pvControl,
    //                   RS_Vector *pvEnd) const;

    // bool offsetCut(const RS_Vector& coord, const double& distance);
    // bool offsetSpline(const RS_Vector& coord, const double& distance);
    // std::vector<RS_Entity*> offsetTwoSidesSpline(const double& distance) const;
    // std::vector<RS_Entity*> offsetTwoSidesCut(const double& distance) const;
    LC_ParabolaData data;

public:
    LC_Parabola(RS_EntityContainer* parent, const LC_ParabolaData& d);
    //RS_Entity* clone() const override;

    RS_Entity* clone() const override;

    /**	@return RS2::EntitySpline */
    RS2::EntityType rtti() const override;

    /** @return false */
    bool isEdge() const override
    {
        return true;
    }

    /** @return Copy of data that defines the spline. */
    LC_ParabolaData const& getData() const
    {
        return data;
    }
    LC_ParabolaData& getData()
    {
        return data;
    }

    /** \brief return the equation of the entity
    a quadratic contains coefficients for quadratic:
    m0 x^2 + m1 xy + m2 y^2 + m3 x + m4 y + m5 =0

    for linear:
    m0 x + m1 y + m2 =0
    **/
    LC_Quadratic getQuadratic() const override;

    RS_Vector getTangentDirection(const RS_Vector& point)const override;
    //find the tangential points seeing from given point
    RS_VectorSolutions getTangentPoint(const RS_Vector& point) const override;

    RS2::Ending getTrimPoint(const RS_Vector& trimCoord,
                             const RS_Vector& trimPoint) override;
    RS_Vector prepareTrim(const RS_Vector& trimCoord,
                          const RS_VectorSolutions& trimSol) override;

    /** Sets the startpoint */
    //void setStartpoint(RS_Vector s) {
    //    data.startpoint = s;
    //    calculateBorders();
    //}
    /** Sets the endpoint */
    //void setEndpoint(RS_Vector e) {
    //    data.endpoint = e;
    //    calculateBorders();
    //}

    double getDirection1() const override;
    double getDirection2() const override;

    void moveStartpoint(const RS_Vector& pos) override;
    void moveEndpoint(const RS_Vector& pos) override;
    //RS2::Ending getTrimPoint(const RS_Vector& coord,
    //          const RS_Vector& trimPoint);
    //void reverse() override;
    /** @return the center point of the line. */
    //RS_Vector getMiddlePoint() {
    //    return (data.startpoint + data.endpoint)/2.0;
    //}
    //bool hasEndpointsWithinWindow(RS_Vector v1, RS_Vector v2) override;

    /**
    * @return The length of the line.
    */
    // double getLength() const override;

    /**
    * @return The angle of the line (from start to endpoint).
    */
    //double getAngle1() {
    //    return data.startpoint.angleTo(data.endpoint);
    //}

    /**
    * @return The angle of the line (from end to startpoint).
    */
    //double getAngle2() {
    //    return data.endpoint.angleTo(data.startpoint);
    //}

    // RS_VectorSolutions getTangentPoint(const RS_Vector& point) const override;
    // RS_Vector getTangentDirection(const RS_Vector& point) const override;

    // RS_Vector getNearestEndpoint(const RS_Vector& coord,
    //                              double* dist = nullptr) const override;
    /**
     * @brief getNearestPointOnEntity
     * @param coord
     * @param onEntity, unused, because current implementation finds the nearest point on the spline
     * @param dist
     * @param entity
     * @return
     */
    // RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
    //                                   bool onEntity = true, double* dist = nullptr, RS_Entity** entity = nullptr) const override;
    // //	RS_Vector getNearestCenter(const RS_Vector& coord,
    // //		double* dist = nullptr) const;
    // RS_Vector getNearestMiddle(const RS_Vector& coord,
    //                            double* dist = nullptr, int middlePoints = 1) const override;
    // RS_Vector getNearestDist(double distance,
    //                          const RS_Vector& coord, double* dist = nullptr) const override;
    // //RS_Vector getNearestRef(const RS_Vector& coord,
    // //                                 double* dist = nullptr);
    // double getDistanceToPoint(const RS_Vector& coord,
    //                           RS_Entity** entity = nullptr, RS2::ResolveLevel level = RS2::ResolveNone,
    //                           double solidDist = RS_MAXDOUBLE) const override;

    // void move(const RS_Vector& offset) override;
    // void rotate(const RS_Vector& center, const double& angle) override;
    // void rotate(const RS_Vector& center, const RS_Vector& angleVector) override;
    // void scale(const RS_Vector& center, const RS_Vector& factor) override;
    // void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) override;

    // void moveRef(const RS_Vector& ref, const RS_Vector& offset) override;
    // void revertDirection() override;

    // void draw(RS_Painter* painter, RS_GraphicView* view, double& patternOffset) override;
    // std::vector<RS_Vector> const& getPoints() const;
    // std::vector<RS_Vector> const& getControlPoints() const;
    // std::vector<RS_Vector> getStrokePoints() const;

    // friend std::ostream& operator << (std::ostream& os, const LC_SplinePoints& l);

    // void calculateBorders() override;

    // bool offset(const RS_Vector& coord, const double& distance) override;
    // std::vector<RS_Entity*> offsetTwoSides(const double& distance) const override;
    // void draw(RS_Painter* painter, RS_GraphicView* view, double& patternOffset) override;

private:
    // rotate a point around the parabola vertex so, the parabola is y= ax^2 + bx + c, with a > 0 after the
    // same rotation
    RS_Vector rotateToQuadratic(RS_Vector vp) const;
};

#endif

