#ifndef EVENT_HPP
#define EVENT_HPP

#include "Vector.hpp"

class Widget;

enum class EventResult {
    CONSUME,
    PROPAGATE
};

class Event {
protected:
    bool bubbling_ = true;

public:
    virtual ~Event() = default;
    virtual EventResult apply(Widget* widget) = 0;
    
    void stopPropagation() { bubbling_ = false; }
    bool isBubbling() const { return bubbling_; }
};

class MouseMoveEvent : public Event {
private:
    VectorD position_;

public:
    MouseMoveEvent(const VectorD& position) : position_(position) {}
    VectorD getPosition() const { return position_; }
    EventResult apply(Widget* widget) override;
};

class MouseButtonEvent : public Event {
private:
    VectorD position_;
    bool pressed_;
    int button_;

public:
    MouseButtonEvent(const VectorD& position, bool pressed, int button)
        : position_(position), pressed_(pressed), button_(button) {}
    
    VectorD getPosition() const { return position_; }
    bool isPressed() const { return pressed_; }
    int getButton() const { return button_; }
    EventResult apply(Widget* widget) override;
};

class KeyEvent : public Event {
private:
    int key_;
    bool pressed_;

public:
    KeyEvent(int key, bool pressed) : key_(key), pressed_(pressed) {}
    int getKey() const { return key_; }
    bool isPressed() const { return pressed_; }
    EventResult apply(Widget* widget) override;
};

class IdleEvent : public Event {
public:
    IdleEvent() = default;
    EventResult apply(Widget* widget) override;
};

#endif