#include "Event.hpp"
#include "../ui/Widget.hpp"

EventResult MouseMoveEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    widget->onMouseMove(position_);
    return bubbling_ ? EventResult::PROPAGATE : EventResult::CONSUME;
}

EventResult MouseButtonEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    if (pressed_) {
        widget->onMouseDown(position_);
    } else {
        widget->onMouseUp(position_);
    }
    return bubbling_ ? EventResult::PROPAGATE : EventResult::CONSUME;
}

EventResult KeyEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    if (pressed_) {
        widget->onKeyDown(key_);
    } else {
        widget->onKeyUp(key_);
    }
    return bubbling_ ? EventResult::PROPAGATE : EventResult::CONSUME;
}

EventResult IdleEvent::apply(Widget* widget) {
    if (!widget) return EventResult::PROPAGATE;
    
    widget->onIdle();
    return bubbling_ ? EventResult::PROPAGATE : EventResult::CONSUME;
}