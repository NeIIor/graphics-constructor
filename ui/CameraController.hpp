#ifndef CAMERA_CONTROLLER_HPP
#define CAMERA_CONTROLLER_HPP

#include "../core/Vector.hpp"

class CameraController {
private:
    VectorD position_;
    VectorD direction_;
    double yaw_ = 0.0;
    double pitch_ = 0.0;
    double move_speed_ = 0.3;
    double mouse_sensitivity_ = 0.1;
    VectorD last_mouse_pos_;
    bool mouse_captured_ = false;

    void updateDirection();

public:
    CameraController(const VectorD& start_pos = VectorD(0, 0, -5));
    
    void handleKeyPress(int key);
    void handleMouseMove(const VectorD& mouse_pos);
    
    const VectorD& getPosition() const { return position_; }
    const VectorD& getDirection() const { return direction_; }
    
    void setMoveSpeed(double speed) { move_speed_ = speed; }
};

#endif