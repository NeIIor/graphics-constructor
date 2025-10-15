#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Widget.hpp"
#include <functional>
#include <string>

class Button : public Widget {
private:
    std::string label_;
    std::function<void()> onClick_;
    bool hovered_ = false;
    bool pressed_ = false;
    Color normal_color_ = Color(200, 200, 200);
    Color hover_color_ = Color(150, 150, 200);
    Color press_color_ = Color(100, 100, 150);

public:
    Button(const std::string& label, const VectorD& pos, const VectorD& size);
    
    void onMouseEnter() override;
    void onMouseLeave() override;
    void onMouseDown(const VectorD& position) override;
    void onMouseUp(const VectorD& position) override;
    
    void render(SFMLSystem& system) override;  
    
    void setOnClick(std::function<void()> callback) { onClick_ = std::move(callback); }
    void setLabel(const std::string& label) { label_ = label; }
};

#endif