#ifndef INFO_PANEL_HPP
#define INFO_PANEL_HPP

#include "Window.hpp"
#include "../ray_tracing/RayTracer.hpp"

class InfoPanel : public Window {
private:
    RayTracer& ray_tracer_;
    
public:
    InfoPanel(RayTracer& ray_tracer, const VectorD& pos, const VectorD& size);
    
    void render(SFMLSystem& system) override;  
    
private:
    void drawInfo(SFMLSystem& system);
};

#endif