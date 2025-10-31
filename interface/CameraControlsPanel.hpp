#ifndef CAMERA_CONTROLS_PANEL_HPP
#define CAMERA_CONTROLS_PANEL_HPP

#include "CosmicElement.hpp"
#include <functional>
#include <string>

class CameraButton : public CosmicElement {
private:
    std::string           label;
    std::function<void()> action;
    PhotonColor           baseColor;
    PhotonColor           hoverColor;
    PhotonColor           pressColor;

    bool                  hoverState = false;
    bool                  pressState = false;

public:
    CameraButton(const QuantumVector& pos, const QuantumVector& size, 
                 const std::string& text , std::function<void()> callback);
    
    void render        (RenderEngine& engine)                        override;

    void onPhotonMove  (const QuantumVector& position)               override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
};

class CameraControlsPanel : public CosmicContainer {
private:
    std::function<void(int)> keyHandler;

public:
    CameraControlsPanel(const QuantumVector& pos, const QuantumVector& size, 
                       std::function<void(int)> handler);
    
    void render        (RenderEngine& engine)                        override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
};

#endif