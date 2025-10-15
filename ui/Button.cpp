#include "Button.hpp"
#include "../graphics/SFMLSystem.hpp"

Button::Button(const std::string& label, const VectorD& pos, const VectorD& size) 
    : label_(label) {
    setPosition(pos);
    setSize(size);
}

void Button::onMouseEnter() {
    hovered_ = true;
    Widget::onMouseEnter();
}

void Button::onMouseLeave() {
    hovered_ = false;
    pressed_ = false;
    Widget::onMouseLeave();
}

void Button::onMouseDown(const VectorD& position) {
    if (contains(position)) {
        pressed_ = true;
    }
    Widget::onMouseDown(position);
}

void Button::onMouseUp(const VectorD& position) {
    if (pressed_ && hovered_ && onClick_) {
        onClick_();
    }
    pressed_ = false;
    Widget::onMouseUp(position);
}

void Button::render(SFMLSystem& system) {
    if (!isVisible()) return;
    
    double x = position_.getX();
    double y = position_.getY();
    double w = size_.getX();
    double h = size_.getY();
    
    Color fill_color = normal_color_;
    if (pressed_ && hovered_) {
        fill_color = press_color_;
    } else if (hovered_) {
        fill_color = hover_color_;
    }
    
    system.drawRectangle(x, y, w, h, fill_color, Color(0, 0, 0), 2);
    
    double text_width = system.getTextWidth(label_, 14);
    double text_height = system.getTextHeight(14);
    double text_x = x + (w - text_width) / 2;
    double text_y = y + (h - text_height) / 2;
    
    system.drawText(text_x, text_y, label_, Color(0, 0, 0), 14);
}