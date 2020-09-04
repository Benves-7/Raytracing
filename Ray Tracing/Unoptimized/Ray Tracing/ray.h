//
// Created by benves-7 on 9/10/19.
//

#ifndef RAY_H
#define RAY_H

#include "MathLib.h"

class ray{
public:
    ray(){}
    ray(const vec3& a, const vec3& b) { A= a; B = b; }

    vec3 origin() const         { return A; }
    vec3 direction() const      { return B; }
    vec3 point_at_parameter(float t) const { return A + B*t; }

    vec3 A, B;
};

#endif
