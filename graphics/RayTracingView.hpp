#ifndef RAY_TRACING_VIEW_HPP
#define RAY_TRACING_VIEW_HPP

#include "../ui/Widget.hpp"
#include "../ray_tracing/RayTracer.hpp"
#include "../ui/CameraController.hpp"
#include <vector>

class RayTracingView : public Widget {
private:
    RayTracer& ray_tracer_;
    CameraController& camera_controller_;
    std::vector<Color> framebuffer_;
    int width_;
    int height_;
    bool focused_ = false;

public:
    RayTracingView(RayTracer& ray_tracer, CameraController& camera_controller, const VectorD& pos, const VectorD& size);
    
    void onMouseDown(const VectorD& position) override;
    void onMouseMove(const VectorD& position) override;
    void onKeyDown(int key) override;
    
    void render(SFMLSystem& system) override;
    void update(float dt) override;
    
private:
    void renderFrame();
    Color getPixelColor(int x, int y) const;
};

#endif