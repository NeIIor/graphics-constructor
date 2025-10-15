#include "Sphere.hpp"
#include <cmath>

Sphere::Sphere(const VectorD& center, double radius, const Color& color,
               double reflectivity, double shininess)
    : center_(center), radius_(radius), color_(color),
      reflectivity_(reflectivity), shininess_(shininess) {}

bool Sphere::intersect(const VectorD& ray_origin, const VectorD& ray_dir, 
                       double& t) const {
    VectorD oc = ray_origin - center_;
    double a = ray_dir * ray_dir;
    double b = 2.0 * (oc * ray_dir);
    double c = (oc * oc) - radius_ * radius_;
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return false;
    }
    
    double sqrt_disc = std::sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2.0 * a);
    double t2 = (-b + sqrt_disc) / (2.0 * a);
    
    if (t1 > 0.001) {
        t = t1;
        return true;
    } else if (t2 > 0.001) {
        t = t2;
        return true;
    }
    
    return false;
}

VectorD Sphere::getNormal(const VectorD& point) const {
    return ~(point - center_);
}