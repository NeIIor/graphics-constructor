#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <iostream>

class Color {
public:
    unsigned long rgb;
    Color(unsigned long r = 0, unsigned long g = 0, unsigned long b = 0) {
        rgb = (r << 16) | (g << 8) | b;
    }

    void operator=(const Color& source) {   
        rgb = source.rgb;
    }

    unsigned long getR() const {
        return (rgb >> 16) & 0xFF;
    }

    unsigned long getG() const {
        return (rgb >> 8) & 0xFF;
    }

    unsigned long getB() const {
        return rgb & 0xFF;
    }
};

template <typename U>
class Vector {
private:
    U x, y, z;
    
public:
    Vector(U x = 0, U y = 0, U z = 0) : x(x), y(y), z(z) {}
    
    U getX() const { return x; }
    U getY() const { return y; }
    U getZ() const { return z; }

    void setX(U newX) { x = newX; }
    void setY(U newY) { y = newY; }
    void setZ(U newZ) { z = newZ; }
    
    void addX(U delta) { x += delta; }
    void addY(U delta) { y += delta; }
    void addZ(U delta) { z += delta; }

    U operator&() const {
        return x*x + y*y + z*z;
    }
    
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y, z + other.z);
    }
    
    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y, z - other.z);
    }
    
    Vector operator*(double scalar) const {
        return Vector(x * scalar, y * scalar, z * scalar);
    }
    
    Vector operator!() const {    //to grow up in function ||| from cringe to legacy
        return Vector (y, -x, 0);
    }

    friend Vector operator*(double scalar, const Vector& vec) {
        return vec * scalar;
    }
    
    U operator*(const Vector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector operator^(const Vector& other) const {
        return Vector(y * other.z - z * other.y,
                      z * other.x - x * other.z,
                      x * other.y - y * other.x);
    }
    
    Vector operator~() const {
        U length = std::sqrt(x*x + y*y + z*z);
        if (length == 0) return *this;
        return Vector(x/length, y/length, z/length);
    }
    
    bool operator==(const Vector& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
    
    Vector operator>>(const Vector& other) const {
        return Vector(other.x - x, other.y - y, other.z - z);
    }
    
    U operator<<(const Vector& other) const {
        Vector diff = *this >> other;
        return std::sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
    }
    
    Vector operator()(U newX, U newY, U newZ) const {
        return Vector(x + newX, y + newY, z + newZ);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    template <typename T>
    void limVec (T lLim, T rLim) {
        if (x < lLim) x = lLim; else if (x > rLim) x = rLim;
        if (y < lLim) y = lLim; else if (y > rLim) y = rLim;
        if (z < lLim) z = lLim; else if (z > rLim) z = rLim;
    }

    void operator=(const Vector& source) {
        x = source.x;
        y = source.y;
        z = source.z;
    }

    Color vecToCol () const {
        return Color (static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)); 
    }

    bool checkPointInOrb (double rad) {
        if (&(*this) < rad * rad) {
            return true;
        } 
        return false;
    }
};

using VectorD = Vector<double>;

#endif // VECTOR_HPP
