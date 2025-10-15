#include "InfoPanel.hpp"
#include "../graphics/SFMLSystem.hpp"
#include <sstream>

InfoPanel::InfoPanel(RayTracer& ray_tracer, const VectorD& pos, const VectorD& size)
    : Window("Info Panel", pos, size), ray_tracer_(ray_tracer) {}

void InfoPanel::render(SFMLSystem& system) {
    Window::render(system);
    drawInfo(system);
}

void InfoPanel::drawInfo(SFMLSystem& system) {
    double x = getPosition().getX() + 10;
    double y = getPosition().getY() + 40;
    double line_height = 20;
    
    std::stringstream ss;
    
    ss << "Spheres: " << ray_tracer_.getSphereCount();
    system.drawText(x, y, ss.str(), Color(255, 255, 255), 12);
    y += line_height;
    
    ss.str("");
    ss << "Lights: " << ray_tracer_.getLightCount();
    system.drawText(x, y, ss.str(), Color(255, 255, 255), 12);
    y += line_height;
    
    VectorD cam_pos = ray_tracer_.getCameraPosition();
    ss.str("");
    ss << "Camera: (" << (int)cam_pos.getX() << ", " << (int)cam_pos.getY() << ", " << (int)cam_pos.getZ() << ")";
    system.drawText(x, y, ss.str(), Color(255, 255, 255), 12);
    y += line_height;
    
    ss.str("");
    ss << "Movement:";
    system.drawText(x, y, ss.str(), Color(255, 255, 255), 12);
    y += line_height;
    
    ss.str("");
    ss << "W/S - Forward/Back";
    system.drawText(x, y, ss.str(), Color(200, 200, 200), 10);
    y += line_height;
    
    ss.str("");
    ss << "A/D - Left/Right";
    system.drawText(x, y, ss.str(), Color(200, 200, 200), 10);
    y += line_height;
    
    ss.str("");
    ss << "R/F - Up/Down";
    system.drawText(x, y, ss.str(), Color(200, 200, 200), 10);
    y += line_height;
    
    ss.str("");
    ss << "Q/E - Rotate Left/Right";
    system.drawText(x, y, ss.str(), Color(200, 200, 200), 10);
    y += line_height;
    
    ss.str("");
    ss << "Z/X - Rotate Up/Down";
    system.drawText(x, y, ss.str(), Color(200, 200, 200), 10);
    y += line_height;
    
    ss.str("");
    ss << "Click 3D view to focus";
    system.drawText(x, y, ss.str(), Color(200, 200, 200), 10);
}