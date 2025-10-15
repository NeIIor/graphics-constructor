#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include "../core/Vector.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include <vector>
#include <memory>

class CameraController;

class RayTracer {
private:
    std::vector<std::unique_ptr<Sphere>> spheres_;
    std::vector<std::unique_ptr<Light>> lights_;
    VectorD camera_position_;
    VectorD camera_direction_;
    double max_reflection_depth_ = 3;
    Color background_color_ = Color(20, 20, 30);
    CameraController* camera_controller_ = nullptr;

public:
    RayTracer();
    
    void addSphere(std::unique_ptr<Sphere> sphere);
    void addLight(std::unique_ptr<Light> light);
    void removeLastSphere();
    void removeLastLight();
    
    void setCameraPosition(const VectorD& pos) { camera_position_ = pos; }
    void setCameraDirection(const VectorD& dir) { camera_direction_ = ~dir; }
    void setMaxReflectionDepth(int depth) { max_reflection_depth_ = depth; }
    void setCameraController(CameraController* controller) { camera_controller_ = controller; }
    
    const VectorD& getCameraPosition() const { return camera_position_; }
    const VectorD& getCameraDirection() const { return camera_direction_; }
    CameraController& getCameraController() { return *camera_controller_; }
    
    Color traceRay(const VectorD& origin, const VectorD& direction, int depth = 0) const;
    Color calculateLighting(const Sphere& sphere, const VectorD& point, const VectorD& normal, const VectorD& view_dir) const;
    
    bool isInShadow(const VectorD& point, const VectorD& light_dir, double light_distance) const;
    
    size_t getSphereCount() const { return spheres_.size(); }
    size_t getLightCount() const { return lights_.size(); }
};

#endif