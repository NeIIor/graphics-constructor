#ifndef COSMIC_ELEMENT_HPP
#define COSMIC_ELEMENT_HPP

#include "../core/QuantumCore.hpp"
#include "../core/CosmicEvents.hpp"
#include <memory>
#include <vector>

class RenderEngine;

class CosmicElement {
protected:
    QuantumVector                               location;
    QuantumVector                               dimensions;
    CosmicElement*                              parentElement = nullptr;
    std::vector<std::unique_ptr<CosmicElement>> childElements;
    bool                                        visible       = true;
    bool                                        active        = false;
    int                                         layerOrder    = 0;

public:
    CosmicElement(const QuantumVector& pos, const QuantumVector& size) 
        : location(pos), dimensions(size) {}
    
    virtual ~CosmicElement() = default;

    QuantumVector getPosition() const { return location  ; }
    QuantumVector getSize    () const { return dimensions; }
    bool isVisible           () const { return visible   ; }
    bool isActive            () const { return active    ; }
    
    void setPosition(const QuantumVector& pos)  { location      = pos   ; }
    void setSize    (const QuantumVector& size) { dimensions    = size  ; }
    void setVisible (bool v)                    { visible       = v     ; }
    void setActive  (bool a)                    { active        = a     ; }
    void setParent  (CosmicElement* parent)     { parentElement = parent; }
    
    QuantumVector getAbsolutePosition() const {
        if (parentElement) {
            QuantumVector parentPos = parentElement->getAbsolutePosition();

            return QuantumVector(parentPos.getX() + location.getX(), 
                                 parentPos.getY() + location.getY(), 0);
        }
        return location;
    }
    
    bool contains(const QuantumVector& point) const {
        QuantumVector absPos = getAbsolutePosition();

        return point.getX() >= absPos.getX() && 
               point.getY() >= absPos.getY() &&
               point.getX() <= absPos.getX() + dimensions.getX() &&
               point.getY() <= absPos.getY() + dimensions.getY();
    }
    
    void addChild(std::unique_ptr<CosmicElement> child) {
        child->setParent(this);
        childElements.push_back(std::move(child));
    }
    
    CosmicElement* findTarget(const QuantumVector& point) {
        if (!contains(point) || !visible) return nullptr;
        
        for (auto it = childElements.rbegin(); it != childElements.rend(); ++it) {
            CosmicElement* target = (*it)->findTarget(point);
            if (target) return target;
        }
        return this;
    }
    
    virtual void render            (RenderEngine& engine) = 0;
    virtual void update            (float delta)                                 {}
    
    virtual void onPhotonMove      (const QuantumVector& position)               {}
    virtual void onQuantumClick    (const QuantumVector& position, bool pressed) {}
    virtual void onNexusPress      (int key, bool pressed)                       {}
    
    virtual EventFlow processSignal(CosmicSignal& signal) {
        return signal.process(this);
    }
};

class CosmicContainer : public CosmicElement {
public:
    CosmicContainer(const QuantumVector& pos, const QuantumVector& size) 
        : CosmicElement(pos, size) {}
    
    void render(RenderEngine& engine) override {
        if (!visible) return;
        
        for (auto& child : childElements) {
            if (child->isVisible()) {
                child->render   (engine);
            }
        }
    }
    
    void update(float delta) override {
        for (auto& child : childElements) {
            child->update(delta);
        }
    }
    
    void onPhotonMove(const QuantumVector&   position)               override {
        for (auto& child : childElements) {
                   child->onPhotonMove      (position);
        }
    }
    
    void onQuantumClick(const QuantumVector& position, bool pressed) override {
        for (auto& child : childElements) {
            child->onQuantumClick(position, pressed);
        }
    }
    
    void onNexusPress(int key, bool pressed)                         override {
        for (auto& child : childElements) {
            child->onNexusPress(key, pressed);
        }
    }
};

#endif