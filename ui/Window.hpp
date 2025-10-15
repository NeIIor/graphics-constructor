#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Widget.hpp"
#include <string>

class Window : public Widget {
protected:
    std::string title_;
    bool dragging_ = false;
    VectorD drag_offset_;
    double title_bar_height_ = 30.0;

public:
    Window(const std::string& title, const VectorD& pos, const VectorD& size);
    
    void onMouseMove(const VectorD& position) override;
    void onMouseDown(const VectorD& position) override;
    void onMouseUp(const VectorD& position) override;
    
    void render(SFMLSystem& system) override;
    
    const std::string& getTitle() const { return title_; }
    double getTitleBarHeight() const { return title_bar_height_; }
    
private:
    bool isTitleBar(const VectorD& position) const;
};

#endif