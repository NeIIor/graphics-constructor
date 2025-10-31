#include "CameraControlsPanel.hpp"
#include "../system/RenderEngine.hpp"
#include <iostream>

CameraButton::CameraButton(const QuantumVector& pos, const QuantumVector& size , const std::string& text, std::function<void()> callback)
                                : CosmicElement(pos,                      size),              label(text),               action(callback),
      baseColor (NexusColors::Interface),
      hoverColor(NexusColors::Quantum  ),
      pressColor(NexusColors::Plasma   )  {}

void CameraButton::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    int x = static_cast<int>(absPos.getX());
    int y = static_cast<int>(absPos.getY());
    int w = static_cast<int>(dimensions.getX());
    int h = static_cast<int>(dimensions.getY());
    
    PhotonColor currentColor = baseColor;
    
    if (pressState) {
        currentColor = pressColor;

    } else if (hoverState) {
        currentColor = hoverColor;
    }
    
    engine.drawRect(x, y, w, h, currentColor, NexusColors::Light, 2);
    
    unsigned int textSize   = 13;
    double       textWidth  = engine.getTextWidth (label, textSize);
    double       textHeight = engine.getTextHeight(textSize);
    
    double textX = x + (w - textWidth) / 2;
    double textY = y + (h - textHeight) / 2;
    
    int textXInt = static_cast<int>(textX);
    int textYInt = static_cast<int>(textY);
    
    PhotonColor textColor = NexusColors::Light;
    
    engine.drawText(textXInt, textYInt, label, textColor, textSize);
}

void CameraButton::onPhotonMove  (const QuantumVector& position) {
    hoverState = contains(position);
}

void CameraButton::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (contains(position)) {
        pressState = pressed;

        if (!pressed && action) {
            action();
        }
    }
}

CameraControlsPanel::CameraControlsPanel(const QuantumVector& pos, const QuantumVector& size , std::function<void(int)> handler)
                                            : CosmicContainer(pos,                      size),               keyHandler(handler) {
    
    double buttonWidth  = 100;
    double buttonHeight = 32;
    double spacing      = 12;
    double startY       = 45;
    
    auto forwardBtn = std::make_unique<CameraButton>(
        QuantumVector(20, startY, 0), QuantumVector(buttonWidth, buttonHeight, 0),
        "Forward", [this]() { if (keyHandler) keyHandler(22); }  // W
    );

    addChild(std::move(forwardBtn));
    
    auto backBtn = std::make_unique<CameraButton>(
        QuantumVector(20, startY + buttonHeight + spacing, 0), 
        QuantumVector(buttonWidth, buttonHeight,           0),
        "Back", [this]() { if (keyHandler) keyHandler(18); }  // S
    );

    addChild(std::move(backBtn));
    
    auto leftBtn = std::make_unique<CameraButton>(
        QuantumVector(20 + buttonWidth + spacing, startY, 0), 
        QuantumVector(     buttonWidth, buttonHeight,     0),
        "Left", [this]() { if (keyHandler) keyHandler(0); }  // A
    );

    addChild(std::move(leftBtn));
    
    auto rightBtn = std::make_unique<CameraButton>(
        QuantumVector(20 + buttonWidth + spacing, startY + buttonHeight + spacing, 0), 
        QuantumVector(     buttonWidth,                    buttonHeight,           0),
        "Right", [this]() { if (keyHandler) keyHandler(3); }  // D
    );

    addChild(std::move(rightBtn));
    
    auto upBtn = std::make_unique<CameraButton>(
        QuantumVector(20, startY + 2*(buttonHeight + spacing), 0), 
        QuantumVector(buttonWidth,    buttonHeight,            0),
        "Up", [this]() { if (keyHandler) keyHandler(17); }  // R
    );

    addChild(std::move(upBtn));
    
    auto downBtn = std::make_unique<CameraButton>(
        QuantumVector(20, startY + 3*(buttonHeight + spacing), 0), 
        QuantumVector(buttonWidth,    buttonHeight,            0),
        "Down", [this]() { if (keyHandler) keyHandler(5); }  // F
    );

    addChild(std::move(downBtn));
    
    auto turnLeftBtn = std::make_unique<CameraButton>(
        QuantumVector(20 + buttonWidth + spacing, startY + 2*(buttonHeight + spacing), 0), 
        QuantumVector(     buttonWidth,                       buttonHeight,            0),
        "Turn Left", [this]() { if (keyHandler) keyHandler(16); }  // Q
    );

    addChild(std::move(turnLeftBtn));
    
    auto turnRightBtn = std::make_unique<CameraButton>(
        QuantumVector(20 + buttonWidth + spacing, startY + 3*(buttonHeight + spacing), 0), 
        QuantumVector(     buttonWidth,                       buttonHeight,            0),
        "Turn Right", [this]() { if (keyHandler) keyHandler(4); }  // E
    );

    addChild(std::move(turnRightBtn));
    
    auto lookUpBtn = std::make_unique<CameraButton>(
        QuantumVector(20, startY + 4*(buttonHeight + spacing), 0), 
        QuantumVector(buttonWidth,    buttonHeight,            0),
        "Look Up", [this]() { if (keyHandler) keyHandler(23); }  
    );

    addChild(std::move(lookUpBtn));

    auto lookDownBtn = std::make_unique<CameraButton>(
        QuantumVector(20 + buttonWidth + spacing, startY + 4*(buttonHeight + spacing), 0), 
        QuantumVector(     buttonWidth,                       buttonHeight,            0),
        "Look Down", [this]() { if (keyHandler) keyHandler(25); }  
    );

    addChild(std::move(lookDownBtn));
}

void CameraControlsPanel::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), dimensions.getY(),
                   NexusColors::Interface,   NexusColors::Light, 2);
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), 35,
                   PhotonColor(80, 90, 150), NexusColors::Light, 1);
    
    engine.drawText(absPos.getX() + 10, absPos.getY() + 8, "Camera Controls", NexusColors::Light, 16);
    
    CosmicContainer::render(engine);
}

void CameraControlsPanel::onQuantumClick(const QuantumVector& position, bool pressed) {
     CosmicContainer    ::onQuantumClick(                     position,      pressed);
}