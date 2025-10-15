#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "Vector.hpp"
#include <memory>

class Widget;

struct PointerState {
    VectorD position;
    bool pressed = false;
    Widget* target = nullptr;
    Widget* hovered = nullptr;
};

class AppState {
private:
    PointerState pointer_;
    std::unique_ptr<Widget> root_;
    
public:
    AppState();
    ~AppState();
    
    PointerState& getPointer() { return pointer_; }
    const PointerState& getPointer() const { return pointer_; }
    
    void setRoot(std::unique_ptr<Widget> root);
    Widget* getRoot() const { return root_.get(); }
    
    void updateTarget(const VectorD& position);
    void handleMouseMove(const VectorD& position);
    void handleMouseDown(const VectorD& position);
    void handleMouseUp(const VectorD& position);
};

#endif