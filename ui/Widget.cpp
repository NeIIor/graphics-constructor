#include "Widget.hpp"
#include "../core/Event.hpp"
#include "../graphics/SFMLSystem.hpp"
#include <algorithm>

Widget::Widget() : position_(0, 0, 0), size_(0, 0, 0) {}

Widget::~Widget() = default;

bool Widget::contains(const VectorD& point) const {
    double px = point.getX();
    double py = point.getY();
    double x = position_.getX();
    double y = position_.getY();
    double w = size_.getX();
    double h = size_.getY();
    
    return (px >= x) && (px <= x + w) && (py >= y) && (py <= y + h);
}

Widget* Widget::findTarget(const VectorD& point) {
    if (!contains(point) || !visible_) {
        return nullptr;
    }
    
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        auto& child = *it;
        if (child->isVisible()) {
            Widget* target = child->findTarget(point);
            if (target) {
                return target;
            }
        }
    }
    
    return this;
}

void Widget::onEvent(Event& event) {
    event.apply(this);
}

void Widget::onMouseEnter() {
    contains_mouse_ = true;
}

void Widget::onMouseLeave() {
    contains_mouse_ = false;
}

void Widget::onMouseMove(const VectorD& position) {
    for (auto& child : children_) {
        if (child->isVisible() && child->contains(position)) {
            child->onMouseMove(position);
        }
    }
}

void Widget::onMouseDown(const VectorD& position) {
    for (auto& child : children_) {
        if (child->isVisible() && child->contains(position)) {
            child->onMouseDown(position);
        }
    }
}

void Widget::onMouseUp(const VectorD& position) {
    for (auto& child : children_) {
        if (child->isVisible() && child->contains(position)) {
            child->onMouseUp(position);
        }
    }
}

void Widget::onKeyDown(int key) {
    std::cout << "Widget key: " << key << std::endl;
    for (auto& child : children_) {
        child->onKeyDown(key);
    }
}

void Widget::onKeyUp(int key) {
    for (auto& child : children_) {
        child->onKeyUp(key);
    }
}

void Widget::onIdle() {
    for (auto& child : children_) {
        child->onIdle();
    }
}

void Widget::render(SFMLSystem& system) {
    if (!visible_) return;
    
    for (auto& child : children_) {
        child->render(system);
    }
}

void Widget::update(float dt) {
    for (auto& child : children_) {
        child->update(dt);
    }
}

void Widget::addChild(std::unique_ptr<Widget> child) {
    child->parent_ = this;
    children_.push_back(std::move(child));
    
    std::sort(children_.begin(), children_.end(), 
              [](const auto& a, const auto& b) { 
                  return a->getZOrder() < b->getZOrder(); 
              });
}

void Widget::setPosition(const VectorD& pos) {
    VectorD delta = pos - position_;
    position_ = pos;
    
    for (auto& child : children_) {
        VectorD child_pos = child->getPosition();
        child->setPosition(child_pos + delta);
    }
}

void Widget::setSize(const VectorD& size) {
    size_ = size;
}

void Widget::setZOrder(int order) {
    z_order_ = order;
    if (parent_) {
        parent_->addChild(std::unique_ptr<Widget>(this));
    }
}

void Widget::setVisible(bool visible) {
    visible_ = visible;
}