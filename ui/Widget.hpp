#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "../core/Vector.hpp"
#include <memory>
#include <vector>

class SFMLSystem; 

class Widget {
protected:
    VectorD position_;
    VectorD size_;
    std::vector<std::unique_ptr<Widget>> children_;
    Widget* parent_ = nullptr;
    int z_order_ = 0;
    bool visible_ = true;
    bool contains_mouse_ = false;

public:
    Widget();
    virtual ~Widget();
    
    virtual bool contains(const VectorD& point) const;
    virtual Widget* findTarget(const VectorD& point);
    
    virtual void onEvent(class Event& event);
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onMouseMove(const VectorD& position);
    virtual void onMouseDown(const VectorD& position);
    virtual void onMouseUp(const VectorD& position);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual void onIdle();
    
    virtual void render(SFMLSystem& system); 
    virtual void update(float dt);
    
    void addChild(std::unique_ptr<Widget> child);
    
    void setPosition(const VectorD& pos);
    void setSize(const VectorD& size);
    void setZOrder(int order);
    void setVisible(bool visible);
    
    const VectorD& getPosition() const { return position_; }
    const VectorD& getSize() const { return size_; }
    int getZOrder() const { return z_order_; }
    bool isVisible() const { return visible_; }
    Widget* getParent() const { return parent_; }
};

#endif