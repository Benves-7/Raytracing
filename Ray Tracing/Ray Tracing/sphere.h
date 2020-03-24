//
// Created by benves-7 on 9/11/19.
//

#ifndef SPHERE_H
#define SPHERE_H


#include "hitable.h"
//#include "material.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r) : center(cen), radius(r) {};
    sphere(vec3 cen, float r, material* mat) : center(cen), radius(r), mat_ptr(mat) {};
    //~sphere() {
    //    delete mat_ptr;
    //}
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0*vec3(randomFloat(), randomFloat(), randomFloat()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}


#endif //PATH_TRACING_SPHERE_H
