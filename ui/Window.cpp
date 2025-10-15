#include "Window.hpp"
#include "../graphics/SFMLSystem.hpp"

Window::Window(const std::string& title, const VectorD& pos, const VectorD& size) 
    : title_(title) {
    setPosition(pos);
    setSize(size);
}

bool Window::isTitleBar(const VectorD& position) const {
    double local_x = position.getX() - position_.getX();
    double local_y = position.getY() - position_.getY();
    
    return (local_x >= 0) && (local_x <= size_.getX()) && 
           (local_y >= 0) && (local_y <= title_bar_height_);
}

void Window::onMouseMove(const VectorD& position) {
    if (dragging_) {
        VectorD new_pos = position - drag_offset_;
        setPosition(new_pos);
        return;
    }
    
    Widget::onMouseMove(position);
}

void Window::onMouseDown(const VectorD& position) {
    if (isTitleBar(position)) {
        dragging_ = true;
        drag_offset_ = position - position_;
        setZOrder(999);
        return;
    }
    
    Widget::onMouseDown(position);
}

void Window::onMouseUp(const VectorD& position) {
    dragging_ = false;
    Widget::onMouseUp(position);
}

void Window::render(SFMLSystem& system) {
    if (!isVisible()) return;
    
    double x = position_.getX();
    double y = position_.getY();
    double w = size_.getX();
    double h = size_.getY();
    
    system.drawRectangle(x, y, w, h, Color(40, 40, 50), Color(255, 255, 255), 2);
    system.drawRectangle(x, y, w, title_bar_height_, Color(60, 60, 80), Color(255, 255, 255), 1);
    
    double text_x = x + 10;
    double text_y = y + (title_bar_height_ - system.getTextHeight(14)) / 2;
    system.drawText(text_x, text_y, title_, Color(255, 255, 255), 14);
    
    Widget::render(system);
}