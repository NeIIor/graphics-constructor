#ifndef CONTROL_PANEL_HPP
#define CONTROL_PANEL_HPP

#include "Window.hpp"
#include "../ray_tracing/RayTracer.hpp"
#include <memory>

class ControlPanel : public Window {
private:
    RayTracer& ray_tracer_;
    
public:
    ControlPanel(RayTracer& ray_tracer, const VectorD& pos, const VectorD& size);
    
private:
    void createUI();
    void addSphereButton();
    void addLightButton();
    void removeSphereButton();
    void removeLightButton();
    void updateCameraInfo();
};

#endif