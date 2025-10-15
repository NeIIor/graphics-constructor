#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../core/Vector.hpp"

class Light {
private:
    VectorD position_;
    Color color_;
    double intensity_ = 1.0;

public:
    Light(const VectorD& position, const Color& color, double intensity = 1.0);
    
    const VectorD& getPosition() const { return position_; }
    const Color& getColor() const { return color_; }
    double getIntensity() const { return intensity_; }
    
    void setPosition(const VectorD& position) { position_ = position; }
    void setColor(const Color& color) { color_ = color; }
    void setIntensity(double intensity) { intensity_ = intensity; }
};

#endif