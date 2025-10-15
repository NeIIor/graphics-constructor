#include "RayTracer.hpp"
#include <cmath>
#include <algorithm>

RayTracer::RayTracer() {
    camera_position_ = VectorD(0, 0, -5);
    camera_direction_ = VectorD(0, 0, 1);
}

void RayTracer::addSphere(std::unique_ptr<Sphere> sphere) {
    spheres_.push_back(std::move(sphere));
}

void RayTracer::addLight(std::unique_ptr<Light> light) {
    lights_.push_back(std::move(light));
}

void RayTracer::removeLastSphere() {
    if (!spheres_.empty()) {
        spheres_.pop_back();
    }
}

void RayTracer::removeLastLight() {
    if (!lights_.empty()) {
        lights_.pop_back();
    }
}

Color RayTracer::traceRay(const VectorD& origin, const VectorD& direction, 
                         int depth) const {
    if (depth > max_reflection_depth_) {
        return background_color_;
    }
    
    double closest_t = 1e10;
    const Sphere* closest_sphere = nullptr;
    
    for (const auto& sphere : spheres_) {
        double t;
        if (sphere->intersect(origin, direction, t) && t < closest_t) {
            closest_t = t;
            closest_sphere = sphere.get();
        }
    }
    
    if (!closest_sphere) {
        return background_color_;
    }
    
    VectorD hit_point = origin + direction * closest_t;
    VectorD normal = closest_sphere->getNormal(hit_point);
    VectorD view_dir = ~(camera_position_ - hit_point);
    
    Color local_color = calculateLighting(*closest_sphere, hit_point, normal, view_dir);
    
    if (closest_sphere->getReflectivity() > 0.0 && depth < max_reflection_depth_) {
        VectorD reflect_dir = direction - normal * (2.0 * (direction * normal));
        Color reflected_color = traceRay(hit_point + normal * 0.001, reflect_dir, depth + 1);
        
        double reflectivity = closest_sphere->getReflectivity();
        return Color(
            static_cast<unsigned long>(local_color.getR() * (1 - reflectivity) + reflected_color.getR() * reflectivity),
            static_cast<unsigned long>(local_color.getG() * (1 - reflectivity) + reflected_color.getG() * reflectivity),
            static_cast<unsigned long>(local_color.getB() * (1 - reflectivity) + reflected_color.getB() * reflectivity)
        );
    }
    
    return local_color;
}

Color RayTracer::calculateLighting(const Sphere& sphere, const VectorD& point, 
                                  const VectorD& normal, const VectorD& view_dir) const {
    Color ambient(30, 30, 30);
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);
    
    Color sphere_color = sphere.getColor();
    
    for (const auto& light : lights_) {
        VectorD light_dir = ~(light->getPosition() - point);
        double light_distance = (light->getPosition() - point) << VectorD(0,0,0);
        
        if (isInShadow(point, light_dir, light_distance)) {
            continue;
        }
        
        double n_dot_l = normal * light_dir;
        if (n_dot_l > 0) {
            double intensity = light->getIntensity() * n_dot_l;
            diffuse = Color(
                static_cast<unsigned long>(diffuse.getR() + sphere_color.getR() * intensity),
                static_cast<unsigned long>(diffuse.getG() + sphere_color.getG() * intensity),
                static_cast<unsigned long>(diffuse.getB() + sphere_color.getB() * intensity)
            );
        }
        
        VectorD reflect_dir = light_dir - normal * (2.0 * n_dot_l);
        double r_dot_v = reflect_dir * view_dir;
        if (r_dot_v > 0) {
            double spec_intensity = light->getIntensity() * 
                                   std::pow(r_dot_v, sphere.getShininess());
            specular = Color(
                static_cast<unsigned long>(specular.getR() + 255 * spec_intensity),
                static_cast<unsigned long>(specular.getG() + 255 * spec_intensity),
                static_cast<unsigned long>(specular.getB() + 255 * spec_intensity)
            );
        }
    }
    
    unsigned long r = ambient.getR() + diffuse.getR() + specular.getR();
    unsigned long g = ambient.getG() + diffuse.getG() + specular.getG();
    unsigned long b = ambient.getB() + diffuse.getB() + specular.getB();
    
    Color final_color = Color(
        r > 255 ? 255 : r,
        g > 255 ? 255 : g,
        b > 255 ? 255 : b
    );
    
    return final_color;
}

bool RayTracer::isInShadow(const VectorD& point, const VectorD& light_dir, 
                          double light_distance) const {
    VectorD shadow_origin = point + light_dir * 0.001;
    
    for (const auto& sphere : spheres_) {
        double t;
        if (sphere->intersect(shadow_origin, light_dir, t) && t < light_distance) {
            return true;
        }
    }
    
    return false;
}   