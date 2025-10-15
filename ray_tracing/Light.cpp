#include "Light.hpp"

Light::Light(const VectorD& position, const Color& color, double intensity)
    : position_(position), color_(color), intensity_(intensity) {}