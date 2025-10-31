#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include "../core/QuantumCore.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class CosmicElement;

class RenderEngine {
private:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::Font> font;

public:
    RenderEngine(int width, int height, const char* title);
    ~RenderEngine();
    
    bool isRunning() const { return window->isOpen(); }
    
    void processEvents(CosmicElement* root);
    void beginFrame();
    void endFrame();
    
    void drawRect(double x, double y, double w, double h, 
                  const PhotonColor& fill, const PhotonColor& border = NexusColors::Light, 
                  double borderWidth = 0);
    void drawText(double x, double y, const std::string& text, 
                  const PhotonColor& color, unsigned size);
    void drawLine(double x1, double y1, double x2, double y2,
                  const PhotonColor& color, double thickness = 1);
    void drawCircle(double x, double y, double radius, const PhotonColor& fill);
    
    double getTextWidth(const std::string& text, unsigned size);
    double getTextHeight(unsigned size);
};

class ApplicationState {
private:
    struct PointerData {
        QuantumVector position;
        bool pressed = false;
        CosmicElement* target = nullptr;
        CosmicElement* hovered = nullptr;
    } pointer;
    
    std::unique_ptr<CosmicElement> rootElement;

public:
    ApplicationState();
    ~ApplicationState();
    
    void setRoot(std::unique_ptr<CosmicElement> root) { rootElement = std::move(root); }
    CosmicElement* getRoot() const { return rootElement.get(); }
    
    void updateTarget(const QuantumVector& position);
    void handlePhotonMove(const QuantumVector& position);
    void handleQuantumClick(const QuantumVector& position, bool pressed);
};

#endif