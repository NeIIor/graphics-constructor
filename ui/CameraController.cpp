#include "CameraController.hpp"
#include <cmath>

CameraController::CameraController(const VectorD& start_pos) 
    : position_(start_pos) {
    updateDirection();
}

void CameraController::updateDirection() {
    direction_ = VectorD(
        std::cos(pitch_) * std::sin(yaw_),
        std::sin(pitch_),
        std::cos(pitch_) * std::cos(yaw_)
    );
}

void CameraController::handleKeyPress(int key) {
    std::cout << "CameraController handling key: " << key << std::endl;
    
    VectorD forward = VectorD(std::sin(yaw_), 0, std::cos(yaw_));
    VectorD right = VectorD(std::cos(yaw_), 0, -std::sin(yaw_));
    VectorD up = VectorD(0, 1, 0);
    
    switch (key) {
        case 22: position_ = position_ + forward * move_speed_; std::cout << "W - Forward" << std::endl; break;
        case 18: position_ = position_ - forward * move_speed_; std::cout << "S - Backward" << std::endl; break;
        case 0: position_ = position_ - right * move_speed_; std::cout << "A - Left" << std::endl; break;
        case 3: position_ = position_ + right * move_speed_; std::cout << "D - Right" << std::endl; break;
        case 17: position_ = position_ + up * move_speed_; std::cout << "R - Up" << std::endl; break;
        case 5: position_ = position_ - up * move_speed_; std::cout << "F - Down" << std::endl; break;
        case 16: yaw_ -= 0.1; updateDirection(); std::cout << "Q - Rotate Left" << std::endl; break;
        case 4: yaw_ += 0.1; updateDirection(); std::cout << "E - Rotate Right" << std::endl; break;
        case 25: pitch_ = std::max(-1.5, pitch_ - 0.1); updateDirection(); std::cout << "Z - Rotate Up" << std::endl; break;
        case 23: pitch_ = std::min(1.5, pitch_ + 0.1); updateDirection(); std::cout << "X - Rotate Down" << std::endl; break;
    }
    
    std::cout << "New position: " << position_ << std::endl;
}

void CameraController::handleMouseMove(const VectorD& mouse_pos) {
}