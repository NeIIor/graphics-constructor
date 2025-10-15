#ifndef SFML_SYSTEM_HPP
#define SFML_SYSTEM_HPP

#include "../core/Vector.hpp"
#include <string>
#include <memory>

// Forward declarations
class AppState;
class CameraController;
class RayTracer;

namespace sf {
    class RenderWindow;
    class Font;
}

class SFMLSystem {
private:
    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<sf::Font> font_;
    CameraController* camera_controller_ = nullptr;
    RayTracer* ray_tracer_ = nullptr;

public:
    SFMLSystem(int width, int height, const char* title);
    ~SFMLSystem();
    
    bool isOpen() const;
    void processEvents(AppState& app_state);
    void beginFrame();
    void endFrame();
    
    void setCameraController(CameraController* controller);
    void setRayTracer(RayTracer* tracer);
    
    void drawRectangle(double x, double y, double w, double h, 
                      const Color& fill, const Color& outline = Color(0,0,0), 
                      double outlineThickness = 0);
    void drawText(double x, double y, const std::string& text, 
                 const Color& color, unsigned int size);
    void drawLine(double x1, double y1, double x2, double y2, 
                 const Color& color, double thickness = 1);
    void drawCircle(double x, double y, double radius, 
                   const Color& fill);
    
    double getTextWidth(const std::string& text, unsigned int size);
    double getTextHeight(unsigned int size);
};

#endif