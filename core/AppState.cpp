#include "AppState.hpp"
#include "../ui/Widget.hpp"

AppState::AppState() = default;

AppState::~AppState() = default;

void AppState::setRoot(std::unique_ptr<Widget> root) {
    root_ = std::move(root);
}

void AppState::updateTarget(const VectorD& position) {
    if (!root_) {
        pointer_.target = nullptr;
        return;
    }
    
    pointer_.target = root_->findTarget(position);
}

void AppState::handleMouseMove(const VectorD& position) {
    pointer_.position = position;
    updateTarget(position);
    
    Widget* new_hovered = pointer_.target;
    if (new_hovered != pointer_.hovered) {
        if (pointer_.hovered) {
            pointer_.hovered->onMouseLeave();
        }
        if (new_hovered) {
            new_hovered->onMouseEnter();
        }
        pointer_.hovered = new_hovered;
    }
    
    if (pointer_.target) {
        pointer_.target->onMouseMove(position);
    }
}

void AppState::handleMouseDown(const VectorD& position) {
    pointer_.position = position;
    pointer_.pressed = true;
    updateTarget(position);
    
    if (pointer_.target) {
        pointer_.target->onMouseDown(position);
    }
}

void AppState::handleMouseUp(const VectorD& position) {
    pointer_.position = position;
    pointer_.pressed = false;
    
    if (pointer_.target) {
        pointer_.target->onMouseUp(position);
    }
}