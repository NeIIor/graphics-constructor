#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../core/Vector.hpp"

class Sphere {
private:
    VectorD center_;
    double radius_;
    Color color_;
    double reflectivity_ = 0.0;
    double shininess_ = 64.0;

public:
    Sphere(const VectorD& center, double radius, const Color& color, 
           double reflectivity = 0.0, double shininess = 64.0);
    
    bool intersect(const VectorD& ray_origin, const VectorD& ray_dir, 
                   double& t) const;
    VectorD getNormal(const VectorD& point) const;
    
    const VectorD& getCenter() const { return center_; }
    double getRadius() const { return radius_; }
    const Color& getColor() const { return color_; }
    double getReflectivity() const { return reflectivity_; }
    double getShininess() const { return shininess_; }
    
    void setCenter(const VectorD& center) { center_ = center; }
    void setColor(const Color& color) { color_ = color; }
    void setReflectivity(double reflectivity) { reflectivity_ = reflectivity; }
};

#endif