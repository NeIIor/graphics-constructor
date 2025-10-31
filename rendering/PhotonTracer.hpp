#ifndef PHOTON_TRACER_HPP
#define PHOTON_TRACER_HPP

#include "../core/QuantumCore.hpp"
#include <memory>
#include <vector>
#include <string>

enum ObjectType {
    OBJECT_REGULAR,
    OBJECT_LIGHT_SOURCE
};

class OpticalObject {
public:
    virtual ~OpticalObject() = default;
    virtual bool intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const = 0;
    virtual QuantumVector getNormal(const QuantumVector& point) const = 0;
    virtual PhotonColor getColor() const = 0;
    virtual double getReflectivity() const = 0;
    virtual double getTransparency() const = 0;
    virtual double getRefractiveIndex() const = 0;
    virtual double getShininess() const = 0;
    virtual ObjectType getObjectType() const = 0;
    virtual double getLightIntensity() const = 0;
    virtual QuantumVector getPosition() const = 0;
    virtual double getRadius() const { return 0.0; }
};

class CrystalSphere : public OpticalObject {
private:
    QuantumVector center;
    double radius;
    PhotonColor surfaceColor;
    double reflectivity;
    double transparency;
    double refractiveIndex;
    double shininess;
    ObjectType objectType;
    double lightIntensity;

public:
    CrystalSphere(const QuantumVector& c, double r, const PhotonColor& col,
                  double refl = 0.0, double trans = 0.0, double refract = 1.0, 
                  double shine = 64.0, ObjectType type = OBJECT_REGULAR, double intensity = 0.0);
    
    bool intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const override;
    QuantumVector getNormal(const QuantumVector& point) const override;
    PhotonColor getColor() const override { return surfaceColor; }
    double getReflectivity() const override { return reflectivity; }
    double getTransparency() const override { return transparency; }
    double getRefractiveIndex() const override { return refractiveIndex; }
    double getShininess() const override { return shininess; }
    ObjectType getObjectType() const override { return objectType; }
    double getLightIntensity() const override { return lightIntensity; }
    QuantumVector getPosition() const override { return center; }
    double getRadius() const override { return radius; }
};

class FinitePlane : public OpticalObject {
private:
    QuantumVector position;
    QuantumVector normal;
    QuantumVector right;
    QuantumVector up;
    double width;
    double height;
    PhotonColor color;
    double reflectivity;
    double transparency;
    double refractiveIndex;
    double shininess;

public:
    FinitePlane(const QuantumVector& pos, const QuantumVector& norm, 
                const QuantumVector& rightVec, double w, double h, const PhotonColor& col,
                double refl = 0.0, double trans = 0.0, double refract = 1.0, double shine = 64.0);
    
    bool intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const override;
    QuantumVector getNormal(const QuantumVector& point) const override;
    PhotonColor getColor() const override { return color; }
    double getReflectivity() const override { return reflectivity; }
    double getTransparency() const override { return transparency; }
    double getRefractiveIndex() const override { return refractiveIndex; }
    double getShininess() const override { return shininess; }
    ObjectType getObjectType() const override { return OBJECT_REGULAR; }
    double getLightIntensity() const override { return 0.0; }
    QuantumVector getPosition() const override { return position; }
};

class Pyramid : public OpticalObject {
private:
    QuantumVector baseCenter;
    QuantumVector apex;
    double baseRadius;
    int sides;
    double height;
    PhotonColor color;
    double reflectivity;
    double transparency;
    double refractiveIndex;
    double shininess;
    
    struct Face {
        QuantumVector v1, v2, v3;
        QuantumVector normal;
        QuantumVector center;
    };
    std::vector<Face> faces;

    void calculateGeometry();

public:
    Pyramid(const QuantumVector& baseCenter, const QuantumVector& apexDir, double baseRad, 
            int numSides, const PhotonColor& col, double refl = 0.0, double trans = 0.0, 
            double refract = 1.0, double shine = 64.0);
    
    bool intersect(const QuantumVector& origin, const QuantumVector& direction, double& t) const override;
    QuantumVector getNormal(const QuantumVector& point) const override;
    PhotonColor getColor() const override { return color; }
    double getReflectivity() const override { return reflectivity; }
    double getTransparency() const override { return transparency; }
    double getRefractiveIndex() const override { return refractiveIndex; }
    double getShininess() const override { return shininess; }
    ObjectType getObjectType() const override { return OBJECT_REGULAR; }
    double getLightIntensity() const override { return 0.0; }
    QuantumVector getPosition() const override { return baseCenter; }
};

class RayTracer {
private:
    std::vector<std::unique_ptr<OpticalObject>> objects;
    QuantumVector observerPosition;
    QuantumVector observerDirection;
    int maxDepth = 3; // ОПТИМИЗАЦИЯ: уменьшена глубина

    int pyramidCount = 0;
    int sphereCount = 0;
    int lightSourceCount = 0;
    int planeCount = 0;

public:
    RayTracer();
    
    void addObject(std::unique_ptr<OpticalObject> object);
    void removeLastObject();
    void removeLastLightSource();
    
    void setObserverPosition(const QuantumVector& pos) { observerPosition = pos; }
    void setObserverDirection(const QuantumVector& dir) { observerDirection = dir.normalize(); }
    
    const QuantumVector& getObserverPosition() const { return observerPosition; }
    const QuantumVector& getObserverDirection() const { return observerDirection; }
    
    size_t getObjectCount() const { return objects.size(); }
    size_t getLightCount() const;
    
    int getPyramidCount() const { return pyramidCount; }
    int getSphereCount() const { return sphereCount; }
    int getLightSourceCount() const { return lightSourceCount; }
    int getPlaneCount() const { return planeCount; }
    
    void updateObjectStatistics();
    std::vector<std::string> getObjectInfosByType(const std::string& type) const;
    
    PhotonColor traceRay(const QuantumVector& origin, const QuantumVector& direction, int depth = 0);
    
private:
    PhotonColor calculateLighting(const OpticalObject* object, const QuantumVector& point,
                                 const QuantumVector& normal, const QuantumVector& viewDir);
    bool isInShadow(const QuantumVector& point, const QuantumVector& lightDir, double lightDist);
    const OpticalObject* findClosestIntersection(const QuantumVector& rayStart, const QuantumVector& rayDir, 
                                                QuantumVector& intersection, float& distance) const;
    int findLastLightSourceIndex() const;
};

#endif