#include "core/AppState.hpp"
#include "graphics/SFMLSystem.hpp"
#include "ray_tracing/RayTracer.hpp"
#include "ui/ControlPanel.hpp"
#include "ui/InfoPanel.hpp"
#include "ui/CameraController.hpp"
#include "graphics/RayTracingView.hpp"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

int main() {
    SFMLSystem system(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Ray Tracing");
    RayTracer ray_tracer;
    AppState app_state;
    CameraController camera_controller;
    
    system.setCameraController(&camera_controller);
    system.setRayTracer(&ray_tracer);
    
    auto root = std::make_unique<Widget>();
    root->setSize(VectorD(WINDOW_WIDTH, WINDOW_HEIGHT, 0));
    
    auto control_panel = std::make_unique<ControlPanel>(ray_tracer, VectorD(50, 50, 0), VectorD(200, 250, 0));
    auto info_panel = std::make_unique<InfoPanel>(ray_tracer, VectorD(50, 320, 0), VectorD(200, 200, 0));
    auto ray_tracing_view = std::make_unique<RayTracingView>(ray_tracer, camera_controller, VectorD(270, 50, 0), VectorD(700, 600, 0));
    
    root->addChild(std::move(control_panel));
    root->addChild(std::move(info_panel));
    root->addChild(std::move(ray_tracing_view));
    
    app_state.setRoot(std::move(root));
    
    ray_tracer.addLight(std::make_unique<Light>(VectorD(2, 1, 0), Color(255, 255, 255), 1.0));
    ray_tracer.addSphere(std::make_unique<Sphere>(VectorD(0, 0, 3), 1.0, Color(200, 100, 80), 0.3));
    ray_tracer.addSphere(std::make_unique<Sphere>(VectorD(-2, 0, 4), 1.0, Color(100, 200, 100), 0.5));
    ray_tracer.addSphere(std::make_unique<Sphere>(VectorD(2, 0, 4), 1.0, Color(100, 100, 200), 0.7));
    
    while (system.isOpen()) {
        system.processEvents(app_state);
        system.beginFrame();
        app_state.getRoot()->render(system);
        system.endFrame();
    }
    
    return 0;
}