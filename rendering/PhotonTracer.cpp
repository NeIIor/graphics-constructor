#include "PhotonTracer.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

CrystalSphere::CrystalSphere(const QuantumVector& c, double r, const PhotonColor& col,
              double refl, double trans, double refract, double shine, ObjectType type, double intensity)
    : center(c), radius(r), surfaceColor(col), reflectivity(refl),
      transparency(trans), refractiveIndex(refract), shininess(shine),
      objectType(type), lightIntensity(intensity) {}

bool CrystalSphere::intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const {
    QuantumVector oc = origin - center;
    double a = direction.dot(direction);
    double b = 2.0 * oc.dot(direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) return false;
    
    double sqrtDisc = std::sqrt(discriminant);
    double t1 = (-b - sqrtDisc) / (2.0 * a);
    double t2 = (-b + sqrtDisc) / (2.0 * a);
    
    t = t1;
    if (t1 < 0.001) {
        t = t2;
        if (t2 < 0.001) return false;
    }
    return true;
}

QuantumVector CrystalSphere::getNormal(const QuantumVector& point) const {
    return (point - center).normalize();
}

FinitePlane::FinitePlane(const QuantumVector& pos, const QuantumVector& norm, 
            const QuantumVector& rightVec, double w, double h, const PhotonColor& col,
            double refl, double trans, double refract, double shine)
    : position(pos), normal(norm.normalize()), right(rightVec.normalize()), 
      up(normal.cross(right)), width(w), height(h), color(col),
      reflectivity(refl), transparency(trans), refractiveIndex(refract), shininess(shine) {}

bool FinitePlane::intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const {
    double denom = normal.dot(direction);
    if (std::abs(denom) < 1e-6) return false;
    
    t = normal.dot(position - origin) / denom;
    if (t < 0.001) return false;
    
    QuantumVector hitPoint = origin + direction * t;
    QuantumVector localPos = hitPoint - position;
    
    double rightCoord = localPos.dot(right);
    double upCoord    = localPos.dot(up);
    
    return (std::abs(rightCoord) <= width/2) && (std::abs(upCoord) <= height/2);
}

QuantumVector FinitePlane::getNormal(const QuantumVector& point) const {
    return normal;
}

Pyramid::Pyramid(const QuantumVector& baseCenter, const QuantumVector& apexDir, double baseRad, 
        int numSides, const PhotonColor& col, double refl, double trans, 
        double refract, double shine)
    : baseCenter(baseCenter), baseRadius(baseRad), sides(numSides), color(col),
      reflectivity(refl), transparency(trans), refractiveIndex(refract), shininess(shine) {
    
    QuantumVector dirNormalized = apexDir.normalize();
    height = apexDir.length();
    apex = baseCenter + dirNormalized * height;
    
    calculateGeometry();
}

void Pyramid::calculateGeometry() {
    faces.clear();
    
    double angleStep = 2 * M_PI / sides;
    
    std::vector<QuantumVector> baseVertices;
    for (int i = 0; i < sides; ++i) {
        double angle = i * angleStep;
        double x = baseRadius * std::cos(angle);
        double z = baseRadius * std::sin(angle);
        baseVertices.push_back(baseCenter + QuantumVector(x, 0, z));
    }
    
    for (int i = 0; i < sides; ++i) {
        int next = (i + 1) % sides;
        Face face;
        face.v1 = baseVertices[i];
        face.v2 = baseVertices[next];
        face.v3 = apex;
        face.center = (face.v1 + face.v2 + face.v3) * (1.0/3.0);
        
        QuantumVector edge1 = face.v2 - face.v1;
        QuantumVector edge2 = face.v3 - face.v1;
        face.normal = edge1.cross(edge2).normalize();
        
        QuantumVector toCenter = baseCenter - face.center;
        if (face.normal.dot(toCenter) > 0) {
            face.normal = face.normal * -1.0;
        }
        
        faces.push_back(face);
    }
    
    for (int i = 0; i < sides; ++i) {
        int next = (i + 1) % sides;
        Face face;
        face.v1 = baseVertices[i];
        face.v2 = baseVertices[next];
        face.v3 = baseCenter;
        face.center = (face.v1 + face.v2 + face.v3) * (1.0/3.0);
        
        QuantumVector edge1 = face.v2 - face.v1;
        QuantumVector edge2 = face.v3 - face.v1;
        face.normal = edge1.cross(edge2).normalize();
        
        if (face.normal.getY() > 0) {
            face.normal = face.normal * -1.0;
        }
        
        faces.push_back(face);
    }
}

bool Pyramid::intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const {
    double closestT = 1e10;
    bool found = false;
    
    for (const auto& face : faces) {
        QuantumVector edge1 = face.v2 - face.v1;
        QuantumVector edge2 = face.v3 - face.v1;
        QuantumVector h = direction.cross(edge2);
        double a = edge1.dot(h);
        
        if (a > -1e-8 && a < 1e-8) continue;
        
        double f = 1.0 / a;
        QuantumVector s = origin - face.v1;
        double u = f * s.dot(h);
        
        if (u < 0.0 || u > 1.0) continue;
        
        QuantumVector q = s.cross(edge1);
        double v = f * direction.dot(q);
        
        if (v < 0.0 || u + v > 1.0) continue;
        
        double currentT = f * edge2.dot(q);
        
        if (currentT > 0.001 && currentT < closestT) {
            closestT = currentT;
            found = true;
        }
    }
    
    if (found) {
        t = closestT;
        return true;
    }
    return false;
}

QuantumVector Pyramid::getNormal(const QuantumVector& point) const {
    double minDistance = 1e10;
    int bestFaceIndex = -1;
    
    for (int i = 0; i < faces.size(); ++i) {
        const Face& face = faces[i];
        double distance = std::abs(face.normal.dot(point - face.v1));
        double distToCenter = point.distance(face.center);
        double score = distance * 0.7 + distToCenter * 0.3;
        
        if (score < minDistance) {
            minDistance = score;
            bestFaceIndex = i;
        }
    }
    
    if (bestFaceIndex >= 0 && bestFaceIndex < faces.size()) {
        return faces[bestFaceIndex].normal;
    }
    
    return QuantumVector(0, 1, 0);
}

RayTracer::RayTracer() {
    observerPosition  = QuantumVector(0, 0, -5);
    observerDirection = QuantumVector(0, 0, 1);
}

void RayTracer::addObject(std::unique_ptr<OpticalObject> object) {
    objects.push_back(std::move(object));
    updateObjectStatistics();
}

void RayTracer::removeLastObject() {
    if (!objects.empty()) {
        objects.pop_back();
        updateObjectStatistics();
    }
}

void RayTracer::removeLastLightSource() {
    int index = findLastLightSourceIndex();
    if (index != -1) {
        objects.erase(objects.begin() + index);
        updateObjectStatistics();
    }
}

size_t RayTracer::getLightCount() const {
    size_t count = 0;
    for (const auto& obj : objects) {
        if (obj->getObjectType() == OBJECT_LIGHT_SOURCE) {
            count++;
        }
    }
    return count;
}

void RayTracer::updateObjectStatistics() {
    pyramidCount = 0;
    sphereCount = 0;
    lightSourceCount = 0;
    planeCount = 0;
    
    for (const auto& obj : objects) {
        if (dynamic_cast<Pyramid*>(obj.get())) {
            pyramidCount++;
        } else if (auto* sphere = dynamic_cast<CrystalSphere*>(obj.get())) {
            if (sphere->getObjectType() == OBJECT_LIGHT_SOURCE) {
                lightSourceCount++;
            } else {
                sphereCount++;
            }
        } else if (dynamic_cast<FinitePlane*>(obj.get())) {
            planeCount++;
        }
    }
}

std::vector<std::string> RayTracer::getObjectInfosByType(const std::string& type) const {
    std::vector<std::string> result;
    
    for (size_t i = 0; i < objects.size(); i++) {
        auto& obj = objects[i];
        std::stringstream ss;
        
        if (type == "Pyramids" && dynamic_cast<Pyramid*>(obj.get())) {
            ss << "Pyramid #" << (i+1);
            ss << " Pos(" << obj->getPosition().getX() 
               << "," << obj->getPosition().getY() 
               << "," << obj->getPosition().getZ() << ")";
            ss << " Refl:" << obj->getReflectivity();
            if (obj->getTransparency() > 0.01) {
                ss << " Trans:" << obj->getTransparency();
            }
            if (obj->getShininess() > 0.01) {
                ss << " Shine:" << static_cast<int>(obj->getShininess());
            }
            result.push_back(ss.str());
        }
        else if (type == "Spheres") {
            if (auto sphere = dynamic_cast<CrystalSphere*>(obj.get())) {
                if (sphere->getObjectType() == OBJECT_REGULAR) {
                    ss << "Sphere #" << (i+1);
                    ss << " R:" << sphere->getRadius();
                    ss << " Pos(" << obj->getPosition().getX() 
                       << "," << obj->getPosition().getY() 
                       << "," << obj->getPosition().getZ() << ")";
                    ss << " Refl:" << obj->getReflectivity();
                    if (obj->getTransparency() > 0.01) {
                        ss << " Trans:" << obj->getTransparency();
                    }
                    if (obj->getRefractiveIndex() != 1.0) {
                        ss << " Refr:" << obj->getRefractiveIndex();
                    }
                    if (obj->getShininess() > 0.01) {
                        ss << " Shine:" << static_cast<int>(obj->getShininess());
                    }
                    result.push_back(ss.str());
                }
            }
        }
        else if (type == "LightSources") {
            if (auto sphere = dynamic_cast<CrystalSphere*>(obj.get())) {
                if (sphere->getObjectType() == OBJECT_LIGHT_SOURCE) {
                    ss << "Light #" << (i+1);
                    ss << " Pos(" << obj->getPosition().getX() 
                       << "," << obj->getPosition().getY() 
                       << "," << obj->getPosition().getZ() << ")";
                    ss << " R:" << static_cast<int>(sphere->getColor().getR());
                    ss << " G:" << static_cast<int>(sphere->getColor().getG());
                    ss << " B:" << static_cast<int>(sphere->getColor().getB());
                    ss << " I:" << sphere->getLightIntensity();
                    result.push_back(ss.str());
                }
            }
        }
        else if (type == "Planes" && dynamic_cast<FinitePlane*>(obj.get())) {
            ss << "Plane #" << (i+1);
            ss << " Pos(" << obj->getPosition().getX() 
               << "," << obj->getPosition().getY() 
               << "," << obj->getPosition().getZ() << ")";
            ss << " Refl:" << obj->getReflectivity();
            if (obj->getTransparency() > 0.01) {
                ss << " Trans:" << obj->getTransparency();
            }
            if (obj->getShininess() > 0.01) {
                ss << " Shine:" << static_cast<int>(obj->getShininess());
            }
            result.push_back(ss.str());
        }
    }
    
    return result;
}

PhotonColor RayTracer::traceRay(const QuantumVector& origin, const QuantumVector& direction, int depth) {
    if (depth > maxDepth) {
        return NexusColors::Void;
    }
    
    QuantumVector intersectionPoint;
    float closestDistance;
    const OpticalObject* hitObject = findClosestIntersection(origin, direction, intersectionPoint, closestDistance);
    
    if (!hitObject) {
        return NexusColors::Void;
    }
    
    if (hitObject->getObjectType() == OBJECT_LIGHT_SOURCE) {
        return hitObject->getColor();
    }
    
    QuantumVector surfaceNormal = hitObject->getNormal(intersectionPoint);
    QuantumVector viewDirection = (observerPosition - intersectionPoint).normalize();
    
    PhotonColor localColor = calculateLighting(hitObject, intersectionPoint, surfaceNormal, viewDirection);
    PhotonColor result = localColor;
    
    // ОПТИМИЗАЦИЯ: пропускаем сложные эффекты на большой глубине
    if (depth < 2) {
        if (hitObject->getReflectivity() > 0.001) {
            QuantumVector reflectDir = direction - surfaceNormal * (2.0 * direction.dot(surfaceNormal));
            PhotonColor reflectedColor = traceRay(intersectionPoint + surfaceNormal * 0.001, reflectDir, depth + 1);
            
            double reflectivity = hitObject->getReflectivity();
            result = PhotonColor(
                result.getR() * (1 - reflectivity) + reflectedColor.getR() * reflectivity,
                result.getG() * (1 - reflectivity) + reflectedColor.getG() * reflectivity,
                result.getB() * (1 - reflectivity) + reflectedColor.getB() * reflectivity
            );
        }
        
        if (hitObject->getTransparency() > 0.001) {
            bool entering = direction.dot(surfaceNormal) < 0;
            double n1 = entering ? 1.0 : hitObject->getRefractiveIndex();
            double n2 = entering ? hitObject->getRefractiveIndex() : 1.0;
            QuantumVector normal = entering ? surfaceNormal : surfaceNormal * -1.0;
            
            double cosI = -normal.dot(direction);
            double ratio = n1 / n2;
            double sinT2 = ratio * ratio * (1.0 - cosI * cosI);
            
            if (sinT2 <= 1.0) {
                double cosT = std::sqrt(1.0 - sinT2);
                QuantumVector refractDir = direction * ratio + normal * (ratio * cosI - cosT);
                refractDir = refractDir.normalize();
                
                QuantumVector refractStart = intersectionPoint + refractDir * 0.001;
                PhotonColor refractedColor = traceRay(refractStart, refractDir, depth + 1);
                
                double transparency = hitObject->getTransparency();
                
                double R0 = std::pow((n1 - n2) / (n1 + n2), 2.0);
                double fresnel = R0 + (1.0 - R0) * std::pow(1.0 - cosI, 5.0);
                double refractWeight = (1.0 - fresnel) * transparency;
                
                result = PhotonColor(
                    result.getR() * (1 - refractWeight) + refractedColor.getR() * refractWeight,
                    result.getG() * (1 - refractWeight) + refractedColor.getG() * refractWeight,
                    result.getB() * (1 - refractWeight) + refractedColor.getB() * refractWeight
                );
            }
        }
    }
    
    return result;
}

PhotonColor RayTracer::calculateLighting(const OpticalObject* object, const QuantumVector& point,
                                        const QuantumVector& normal, const QuantumVector& viewDir) {
    PhotonColor objectColor = object->getColor();
    
    // ОПТИМИЗАЦИЯ: уменьшили ambient
    double ambientStrength = 0.05;
    PhotonColor ambient(
        objectColor.getR() * ambientStrength,
        objectColor.getG() * ambientStrength, 
        objectColor.getB() * ambientStrength
    );
    
    double totalR = ambient.getR();
    double totalG = ambient.getG();
    double totalB = ambient.getB();
    
    // ОПТИМИЗАЦИЯ: кэшируем источники света
    static std::vector<const OpticalObject*> lightSources;
    static bool lightsCached = false;
    
    if (!lightsCached) {
        for (const auto& obj : objects) {
            if (obj->getObjectType() == OBJECT_LIGHT_SOURCE) {
                lightSources.push_back(obj.get());
            }
        }
        lightsCached = true;
    }
    
    for (const auto& lightObj : lightSources) {
        QuantumVector lightPos = lightObj->getPosition();
        QuantumVector lightDir = (lightPos - point).normalize();
        double lightDistance = point.distance(lightPos);
        
        // ОПТИМИЗАЦИЯ: быстрая проверка тени
        if (isInShadow(point, lightDir, lightDistance)) {
            continue;
        }
        
        double nDotL = std::max(0.0, normal.dot(lightDir));
        
        if (nDotL > 0) {
            // ОПТИМИЗАЦИЯ: упрощенное затухание
            double attenuation = 1.0 / (1.0 + 0.05 * lightDistance);
            double intensity = lightObj->getLightIntensity() * nDotL * attenuation;
            
            PhotonColor lightColor = lightObj->getColor();
            
            double diffuseR = (objectColor.getR() * lightColor.getR() / 255.0) * intensity;
            double diffuseG = (objectColor.getG() * lightColor.getG() / 255.0) * intensity;
            double diffuseB = (objectColor.getB() * lightColor.getB() / 255.0) * intensity;
            
            // ОПТИМИЗАЦИЯ: specular только для блестящих материалов
            double specularR = 0, specularG = 0, specularB = 0;
            if (object->getShininess() > 10.0) {
                QuantumVector reflectDir = (normal * (2.0 * nDotL) - lightDir).normalize();
                double rDotV = std::max(0.0, reflectDir.dot(viewDir));
                
                if (rDotV > 0) {
                    double specIntensity = std::pow(rDotV, object->getShininess() * 0.1) * intensity;
                    specularR = lightColor.getR() * specIntensity / 255.0;
                    specularG = lightColor.getG() * specIntensity / 255.0;
                    specularB = lightColor.getB() * specIntensity / 255.0;
                }
            }
            
            totalR += diffuseR + specularR;
            totalG += diffuseG + specularG;
            totalB += diffuseB + specularB;
        }
    }
    
    totalR = std::min(255.0, std::max(0.0, totalR));
    totalG = std::min(255.0, std::max(0.0, totalG));
    totalB = std::min(255.0, std::max(0.0, totalB));
    
    // ОПТИМИЗАЦИЯ: убрали gamma коррекцию для скорости
    return PhotonColor(static_cast<unsigned long>(totalR),
                      static_cast<unsigned long>(totalG),
                      static_cast<unsigned long>(totalB));
}

bool RayTracer::isInShadow(const QuantumVector& point, const QuantumVector& lightDir, double lightDistance) {
    QuantumVector shadowOrigin = point + lightDir * 0.001;
    
    // ОПТИМИЗАЦИЯ: проверяем только ближайшие объекты
    for (const auto& object : objects) {
        if (object->getObjectType() == OBJECT_LIGHT_SOURCE) continue;
        
        double t;
        if (object->intersect(shadowOrigin, lightDir, t) && t < lightDistance && t > 0.001) {
            return true;
        }
    }
    
    return false;
}

const OpticalObject* RayTracer::findClosestIntersection(const QuantumVector& rayStart, const QuantumVector& rayDir, 
                                                       QuantumVector& intersection, float& distance) const {
    const OpticalObject* closestObject = nullptr;
    float minDistance = 1e10f;
    
    // ОПТИМИЗАЦИЯ: простой spatial partitioning - сначала проверяем сферы
    for (const auto& object : objects) {
        if (auto sphere = dynamic_cast<const CrystalSphere*>(object.get())) {
            double currentDistance;
            if (sphere->intersect(rayStart, rayDir, currentDistance) && currentDistance < minDistance) {
                minDistance = currentDistance;
                closestObject = object.get();
            }
        }
    }
    
    // Затем проверяем другие объекты
    for (const auto& object : objects) {
        if (dynamic_cast<const CrystalSphere*>(object.get())) continue; // Уже проверили
        
        double currentDistance;
        if (object->intersect(rayStart, rayDir, currentDistance) && currentDistance < minDistance) {
            minDistance = currentDistance;
            closestObject = object.get();
        }
    }
    
    if (closestObject) {
        intersection = rayStart + rayDir * minDistance;
        distance = minDistance;
    }
    
    return closestObject;
}

int RayTracer::findLastLightSourceIndex() const {
    for (int i = objects.size() - 1; i >= 0; --i) {
        if (objects[i]->getObjectType() == OBJECT_LIGHT_SOURCE) {
            return i;
        }
    }
    return -1;
}