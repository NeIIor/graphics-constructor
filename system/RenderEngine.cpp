#include "RenderEngine.hpp"
#include "../interface/CosmicElement.hpp"
#include "../core/CosmicEvents.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

RenderEngine::RenderEngine(int width, int height, const char* title) {
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title);
    font = std::make_unique<sf::Font>();
    
    if (!font->loadFromFile("arialmt.ttf")) {
        std::cerr << "Warning: Could not load font arialmt.ttf" << std::endl;
    }
}

RenderEngine::~RenderEngine() = default;

void RenderEngine::processEvents(CosmicElement* root) {
    sf::Event sfEvent;
    while (window->pollEvent(sfEvent)) {
        switch (sfEvent.type) {
            case sf::Event::Closed:
                window->close();
                break;
                
            case sf::Event::MouseMoved:
                if (root) {
                    QuantumVector pos(sfEvent.mouseMove.x, sfEvent.mouseMove.y, 0);
                    PhotonMoveSignal signal(pos);
                    EventFlow result = root->processSignal(signal);
                }
                break;
                
            case sf::Event::MouseButtonPressed:
                if (sfEvent.mouseButton.button == sf::Mouse::Left && root) {
                    QuantumVector pos(sfEvent.mouseButton.x, sfEvent.mouseButton.y, 0);
                    QuantumClickSignal signal(pos, true);
                    EventFlow result = root->processSignal(signal);
                }
                break;
                
            case sf::Event::MouseButtonReleased:
                if (sfEvent.mouseButton.button == sf::Mouse::Left && root) {
                    QuantumVector pos(sfEvent.mouseButton.x, sfEvent.mouseButton.y, 0);
                    QuantumClickSignal signal(pos, false);
                    EventFlow result = root->processSignal(signal);
                }
                break;
                
            case sf::Event::KeyPressed:
                if (root) {
                    NexusPressSignal signal(sfEvent.key.code, true);
                    EventFlow result = root->processSignal(signal);
                }
                break;
                
            case sf::Event::KeyReleased:
                if (root) {
                    NexusPressSignal signal(sfEvent.key.code, false);
                    EventFlow result = root->processSignal(signal);
                }
                break;
        }
    }
}

void RenderEngine::beginFrame() {
    window->clear(sf::Color(20, 20, 40));
}

void RenderEngine::endFrame() {
    window->display();
}

void RenderEngine::drawRect(double x, double y, double w, double h, 
                           const PhotonColor& fill, const PhotonColor& border, 
                           double borderWidth) {
    sf::RectangleShape rect(sf::Vector2f(w, h));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color(fill.getR(), fill.getG(), fill.getB()));
    rect.setOutlineColor(sf::Color(border.getR(), border.getG(), border.getB()));
    rect.setOutlineThickness(borderWidth);
    window->draw(rect);
}

void RenderEngine::drawText(double x, double y, const std::string& text, 
                           const PhotonColor& color, unsigned size) {
    sf::Text sfText(text, *font, size);
    sfText.setPosition(x, y);
    sfText.setFillColor(sf::Color(color.getR(), color.getG(), color.getB()));
    window->draw(sfText);
}

void RenderEngine::drawLine(double x1, double y1, double x2, double y2,
                           const PhotonColor& color, double thickness) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color(color.getR(), color.getG(), color.getB())),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color(color.getR(), color.getG(), color.getB()))
    };
    window->draw(line, 2, sf::Lines);
}

void RenderEngine::drawCircle(double x, double y, double radius, const PhotonColor& fill) {
    sf::CircleShape circle(radius);
    circle.setPosition(x - radius, y - radius);
    circle.setFillColor(sf::Color(fill.getR(), fill.getG(), fill.getB()));
    window->draw(circle);
}

double RenderEngine::getTextWidth(const std::string& text, unsigned size) {
    sf::Text sfText(text, *font, size);
    return sfText.getLocalBounds().width;
}

double RenderEngine::getTextHeight(unsigned size) {
    sf::Text sfText("A", *font, size);
    return sfText.getLocalBounds().height;
}

ApplicationState::ApplicationState() = default;
ApplicationState::~ApplicationState() = default;

void ApplicationState::updateTarget(const QuantumVector& position) {
    if (!rootElement) {
        pointer.target = nullptr;
        return;
    }
    
    pointer.target = rootElement->findTarget(position);
}

void ApplicationState::handlePhotonMove(const QuantumVector& position) {
    pointer.position = position;
    updateTarget(position);
    
    CosmicElement* newHovered = pointer.target;
    if (newHovered != pointer.hovered) {
        if (pointer.hovered) {
            // pointer.hovered->onMouseLeave();
        }
        if (newHovered) {
            // newHovered->onMouseEnter();
        }
        pointer.hovered = newHovered;
    }
    
    if (pointer.target) {
        pointer.target->onPhotonMove(position);
    }
}

void ApplicationState::handleQuantumClick(const QuantumVector& position, bool pressed) {
    pointer.position = position;
    pointer.pressed = pressed;
    updateTarget(position);
    
    if (pointer.target) {
        pointer.target->onQuantumClick(position, pressed);
    }
}