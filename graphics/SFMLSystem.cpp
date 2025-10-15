#include "../core/AppState.hpp"
#include "../core/Event.hpp"
#include "../core/Vector.hpp"
#include "../ui/CameraController.hpp"
#include "../ui/Widget.hpp"
#include "../ray_tracing/RayTracer.hpp"
#include "SFMLSystem.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

SFMLSystem::SFMLSystem(int width, int height, const char* title) {
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title);
    font_ = std::make_unique<sf::Font>();
    if (!font_->loadFromFile("../resources/arialmt.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
}

SFMLSystem::~SFMLSystem() = default;

void SFMLSystem::setCameraController(CameraController* controller) { 
    camera_controller_ = controller; 
}

void SFMLSystem::setRayTracer(RayTracer* tracer) { 
    ray_tracer_ = tracer; 
}

bool SFMLSystem::isOpen() const { 
    return window_->isOpen(); 
}

void SFMLSystem::processEvents(AppState& app_state) {
    sf::Event sf_event;
    while (window_->pollEvent(sf_event)) {
        switch (sf_event.type) {
            case sf::Event::Closed:
                window_->close();
                break;
                
            case sf::Event::MouseMoved:
                app_state.handleMouseMove(VectorD(sf_event.mouseMove.x, sf_event.mouseMove.y, 0));
                break;
                
            case sf::Event::MouseButtonPressed:
                if (sf_event.mouseButton.button == sf::Mouse::Left) {
                    app_state.handleMouseDown(VectorD(sf_event.mouseButton.x, sf_event.mouseButton.y, 0));
                }
                break;
                
            case sf::Event::MouseButtonReleased:
                if (sf_event.mouseButton.button == sf::Mouse::Left) {
                    app_state.handleMouseUp(VectorD(sf_event.mouseButton.x, sf_event.mouseButton.y, 0));
                }
                break;
                
            case sf::Event::KeyPressed: {
                KeyEvent key_event(sf_event.key.code, true);
                if (app_state.getRoot()) {
                    app_state.getRoot()->onEvent(key_event);
                }
                break;
            }
                
            case sf::Event::KeyReleased: {
                KeyEvent key_event(sf_event.key.code, false);
                if (app_state.getRoot()) {
                    app_state.getRoot()->onEvent(key_event);
                }
                break;
            }
        }
    }
}

void SFMLSystem::beginFrame() {
    window_->clear(sf::Color(20, 20, 30));
}

void SFMLSystem::endFrame() {
    window_->display();
}

void SFMLSystem::drawRectangle(double x, double y, double w, double h, 
                  const Color& fill, const Color& outline, 
                  double outlineThickness) {
    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color(fill.getR(), fill.getG(), fill.getB()));
    rect.setOutlineColor(sf::Color(outline.getR(), outline.getG(), outline.getB()));
    rect.setOutlineThickness(outlineThickness);
    window_->draw(rect);
}

void SFMLSystem::drawText(double x, double y, const std::string& text, 
             const Color& color, unsigned int size) {
    sf::Text sfText(text, *font_, size);
    sfText.setPosition(x, y);
    sfText.setFillColor(sf::Color(color.getR(), color.getG(), color.getB()));
    window_->draw(sfText);
}

void SFMLSystem::drawLine(double x1, double y1, double x2, double y2, 
             const Color& color, double thickness) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color(color.getR(), color.getG(), color.getB())),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color(color.getR(), color.getG(), color.getB()))
    };
    window_->draw(line, 2, sf::Lines);
}

void SFMLSystem::drawCircle(double x, double y, double radius, 
               const Color& fill) {
    sf::CircleShape circle(radius);
    circle.setPosition(x - radius, y - radius);
    circle.setFillColor(sf::Color(fill.getR(), fill.getG(), fill.getB()));
    window_->draw(circle);
}

double SFMLSystem::getTextWidth(const std::string& text, unsigned int size) {
    sf::Text sfText(text, *font_, size);
    return sfText.getLocalBounds().width;
}

double SFMLSystem::getTextHeight(unsigned int size) {
    sf::Text sfText("A", *font_, size);
    return sfText.getLocalBounds().height;
}