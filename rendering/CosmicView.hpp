#ifndef COSMIC_VIEW_HPP
#define COSMIC_VIEW_HPP

#include "../interface/CosmicElement.hpp"
#include "PhotonTracer.hpp"
#include <vector>
#include <thread>
#include <future>
#include <atomic>

class ObserverController {
private:
    QuantumVector position;
    QuantumVector direction;
    double yaw = 0.0;
    double pitch = 0.0;
    double moveSpeed = 0.3;

public:
    ObserverController(const QuantumVector& start = QuantumVector(0, 0, -5)) 
        : position(start) {
        updateDirection();
    }
    
    void updateDirection() {
        direction = QuantumVector(
            std::cos(pitch) * std::sin(yaw),
            std::sin(pitch),
            std::cos(pitch) * std::cos(yaw)
        ).normalize();
    }
    
    void handleKey(int key) {
        QuantumVector forward(std::sin(yaw), 0, std::cos(yaw));
        QuantumVector right(std::cos(yaw), 0, -std::sin(yaw));
        QuantumVector up(0, 1, 0);
        
        switch (key) {
            case 22: position = position + forward * moveSpeed; break;
            case 18: position = position - forward * moveSpeed; break;
            case 0:  position = position - right * moveSpeed; break;
            case 3:  position = position + right * moveSpeed; break;
            case 17: position = position + up * moveSpeed; break;
            case 5:  position = position - up * moveSpeed; break;
            case 16: yaw -= 0.1; updateDirection(); break;
            case 4:  yaw += 0.1; updateDirection(); break;
            case 25: pitch = std::max(-1.5, pitch - 0.1); updateDirection(); break;
            case 23: pitch = std::min(1.5, pitch + 0.1); updateDirection(); break;
        }
    }
    
    const QuantumVector& getPosition() const { return position; }
    const QuantumVector& getDirection() const { return direction; }
};

class CosmicView : public CosmicElement {
private:
    RayTracer& photonTracer;
    ObserverController& observer;
    std::vector<PhotonColor> frameBuffer;
    int bufferWidth, bufferHeight;
    bool focused = false;
    bool needsRedraw = true;
    bool isRendering = false;
    std::atomic<int> completedRows{0};
    std::future<void> renderFuture;
    unsigned int threadCount;

public:
    CosmicView(const QuantumVector& pos, const QuantumVector& size, 
               RayTracer& tracer, ObserverController& controller);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    void onNexusPress(int key, bool pressed) override;
    EventFlow processSignal(CosmicSignal& signal) override;

private:
    void renderFrame();
    void renderFrameAsync();
};

#endif