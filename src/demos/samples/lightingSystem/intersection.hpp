// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: intersection.hpp
// Author: crdrisko
// Date: 07/01/2024-10:21:10
// Description:

#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <config.h>

#include <cmath>

#include "dryphys/vector3d.hpp"

struct Point2D
{
    float x {};
    float y {};

    DryPhys::real cross(const Point2D& rhs) { return x * rhs.y - y * rhs.x; }
};

inline DryPhys::Vector3D operator-(const Point2D& a, const Point2D& b)
{
    return (DryPhys::Vector3D(a.x - b.x, a.y - b.y, 0));
}

struct Intersect
{
    bool result;
    Point2D pos;
};

inline Intersect lineIntersection(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d)
{
    DryPhys::Vector3D r = b - a;
    DryPhys::Vector3D s = d - c;

    DryPhys::real rxs = r.cross(s)[2];

    DryPhys::Vector3D cma = c - a;

    DryPhys::real t = cma.cross(s)[2] / rxs;
    DryPhys::real u = cma.cross(r)[2] / rxs;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
        return Intersect {true, Point2D {a.x + t * r[0], a.y + t * r[1]}};
    else
        return Intersect {false, Point2D {}};
}

#endif
