#ifndef QUANTUM_CORE_HPP
#define QUANTUM_CORE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <cmath>

class QuantumVector {
private:
    double x, y, z;

public:
    QuantumVector(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    
    void setX(double v) { x = v; }
    void setY(double v) { y = v; }
    void setZ(double v) { z = v; }
    
    QuantumVector operator+(const QuantumVector& other) const {
        return QuantumVector(x + other.x, y + other.y, z + other.z);
    }
    
    QuantumVector operator-(const QuantumVector& other) const {
        return QuantumVector(x - other.x, y - other.y, z - other.z);
    }
    
    QuantumVector operator*(double scalar) const {
        return QuantumVector(x * scalar, y * scalar, z * scalar);
    }
    
    double dot(const QuantumVector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    QuantumVector cross(const QuantumVector& other) const {
        return QuantumVector(y * other.z - z * other.y,
                             z * other.x - x * other.z,
                             x * other.y - y * other.x);
    }
    
    double length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    QuantumVector normalize() const {
        double len = length();
        if (len == 0) return *this;

        return QuantumVector(x/len, y/len, z/len);
    }
    
    double distance(const QuantumVector& other) const {
        QuantumVector diff = *this - other;
        return diff.length();
    }
};

class PhotonColor {
public:
    unsigned long value;
    
    PhotonColor(unsigned long r = 0, unsigned long g = 0, unsigned long b = 0) {
        value = (r << 16) | (g << 8) | b;
    }
    
    unsigned long getR() const { return (value >> 16) & 0xFF; }
    unsigned long getG() const { return (value >> 8) & 0xFF; }
    unsigned long getB() const { return value & 0xFF; }
    
    PhotonColor blend(const PhotonColor& other, double ratio) const {
        return PhotonColor(
            getR() * (1-ratio) + other.getR() * ratio,
            getG() * (1-ratio) + other.getG() * ratio,
            getB() * (1-ratio) + other.getB() * ratio
        );
    }
};

namespace NexusColors {
    const PhotonColor Void     (10 , 10 , 25 );
    const PhotonColor Nebula   (70 , 30 , 150);
    const PhotonColor Plasma   (255, 40 , 100);
    const PhotonColor Quantum  (0  , 200, 255);
    const PhotonColor Crystal  (120, 240, 180);
    const PhotonColor Light    (255, 255, 220);
    const PhotonColor Interface(60 , 70 , 120);
}

#endif