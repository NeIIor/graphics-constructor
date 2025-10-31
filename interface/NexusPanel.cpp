#include "NexusPanel.hpp"
#include "../system/RenderEngine.hpp"
#include "../rendering/PhotonTracer.hpp"
#include <iostream>
#include <cstdlib>

void QuantumButton::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    PhotonColor currentColor = baseColor;
    
    if (pressState) {
        currentColor = pressColor;

    } else if (hoverState) {
        currentColor = hoverColor;
    }
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), dimensions.getY(),
                   currentColor, NexusColors::Light, 2);
    
    double textWidth  = engine.getTextWidth (label, 16);
    double textHeight = engine.getTextHeight(       16);
    double textX      = absPos.getX() + (dimensions.getX() - textWidth ) / 2;
    double textY      = absPos.getY() + (dimensions.getY() - textHeight) / 2;
    
    engine.drawText(textX, textY, label, NexusColors::Light, 16);
}

QuantumButton::QuantumButton(const QuantumVector& pos, const QuantumVector& size ,  const std::string& text, std::function<void()> callback)
                                  : CosmicElement(pos,                      size),               label(text),               action(callback),

      baseColor (NexusColors::Interface),
      hoverColor(NexusColors::Quantum  ),
      pressColor(NexusColors::Plasma   ) {}

void QuantumButton::onPhotonMove  (const QuantumVector& position) {
    hoverState = contains(position);
}

void QuantumButton::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (contains(position)) {
        pressState = pressed;

        if (!pressed && action) {
            action();
        }
    }
}

ScrollList::ScrollList(const QuantumVector& pos, const QuantumVector& size , const std::vector<std::string>& itemList)
                            : CosmicElement(pos,                      size),                           items(itemList),
      selectedColor(NexusColors::Plasma   ),
      normalColor  (NexusColors::Interface) {}

void ScrollList::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), dimensions.getY(),
                   NexusColors::Interface, NexusColors::Light, 2);
    
    double itemHeight = 20.0;
    double startY     = absPos.getY() + 5;
    
    for (int i = 0; i < visibleItems && (i + scrollOffset) < items.size(); ++i) {

        int itemIndex = i + scrollOffset;
        double itemY  = startY + i * itemHeight;
        
        PhotonColor bgColor = (itemIndex == selectedIndex) ? selectedColor : normalColor;
        
        engine.drawRect(absPos.getX() + 5, itemY, dimensions.getX() - 10, itemHeight - 2,
                       bgColor, NexusColors::Light, 1);
        
        engine.drawText(absPos.getX() + 10, itemY + 4, items[itemIndex], NexusColors::Light, 12);
    }
    
    if (items.size() > visibleItems) {
        double scrollbarWidth  = 10.0;
        double scrollbarHeight = (visibleItems * 1.0 / items.size()) * (dimensions.getY() - 10);
        double scrollbarY      = absPos.getY() + 5 + (scrollOffset * 1.0 / items.size()) * (dimensions.getY() - 10);
        
        engine.drawRect(absPos.getX() + dimensions.getX() - scrollbarWidth - 5, scrollbarY,
                       scrollbarWidth, scrollbarHeight, NexusColors::Quantum, NexusColors::Light, 1);
    }
}

void ScrollList::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (!pressed || !contains(position)) return;
    
    QuantumVector localPos = position - getAbsolutePosition();
    double itemHeight = 20.0;
    int clickedItem = static_cast<int>((localPos.getY() - 5) / itemHeight) + scrollOffset;
    
    if (clickedItem >= 0 && clickedItem < items.size()) {
        selectedIndex = clickedItem;
    }
}

void ScrollList::onNexusPress(int key, bool pressed) {
    if (!pressed) return;
    
    switch (key) {
        case 73:
            if (selectedIndex > 0                           ) selectedIndex--;
            if (selectedIndex < scrollOffset                )  scrollOffset--;
            break;
        case 74:
            if (selectedIndex < items.size() - 1            ) selectedIndex++;
            if (selectedIndex >= scrollOffset + visibleItems)  scrollOffset++;
            break;
    }
}

std::string ScrollList::getSelectedItem() const {
    if (selectedIndex >= 0 && selectedIndex < items.size()) {
        return items[selectedIndex];
    }
    return "";
}

CameraControls::CameraControls(const QuantumVector& pos, const QuantumVector& size, 
                               std::function<void(int)> handler)
    : CosmicElement(pos, size), keyHandler(handler) {}

void CameraControls::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    double buttonSize = 30.0;
    double centerX    = absPos.getX() + dimensions.getX() / 2;
    double centerY    = absPos.getY() + dimensions.getY() / 2;
    
    std::vector<std::pair<double, double>> positions = {
        {centerX, centerY - buttonSize - 5},
        {centerX - buttonSize - 5, centerY},
        {centerX + buttonSize + 5, centerY},
        {centerX, centerY + buttonSize + 5},
        {centerX - buttonSize - 5, centerY - buttonSize - 5},
        {centerX + buttonSize + 5, centerY - buttonSize - 5}
    };
    
    std::vector<std::string> labels = {"W", "A", "D", "S", "Q", "E"};
    
    for (size_t i = 0; i < positions.size(); ++i) {
        
        engine.drawRect(positions[i].first, positions[i].second, buttonSize, buttonSize,
                       NexusColors::Interface, NexusColors::Light, 2);

        engine.drawText(positions[i].first + 10, positions[i].second + 8, labels[i], NexusColors::Light, 12);
    }
    
    engine.drawText(absPos.getX() + 10, absPos.getY() + 10, "Camera Controls", NexusColors::Plasma, 14);
}

void CameraControls::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (!pressed || !contains(position)) return;
    
    QuantumVector absPos = getAbsolutePosition();
    QuantumVector localPos = position - absPos;
    
    double centerX    = dimensions.getX() / 2;
    double centerY    = dimensions.getY() / 2;
    double buttonSize = 30.0;
    
    std::vector<std::pair<double, double>> positions = {
        {centerX, centerY - buttonSize - 5},
        {centerX - buttonSize - 5, centerY},
        {centerX + buttonSize + 5, centerY},
        {centerX, centerY + buttonSize + 5},
        {centerX - buttonSize - 5, centerY - buttonSize - 5},
        {centerX + buttonSize + 5, centerY - buttonSize - 5}
    };
    
    std::vector<int> keys = {22, 0, 3, 18, 16, 4};
    
    for (size_t i = 0; i < positions.size(); ++i) {
        double dx = localPos.getX() - positions[i].first;
        double dy = localPos.getY() - positions[i].second;
        if (std::abs(dx) <= buttonSize/2 && std::abs(dy) <= buttonSize/2) {
            if (keyHandler) {
                keyHandler(keys[i]);
            }
            break;
        }
    }
}

ObjectSelector::ObjectSelector(const QuantumVector& pos, const QuantumVector& size, 
                               const std::vector<std::string>& objects,
                               std::function<void(int)> callback)
    : CosmicContainer(pos, size), selectionCallback(callback) {
    
    QuantumVector listPos(0, 25, 0);
    QuantumVector listSize(dimensions.getX(), dimensions.getY() - 30, 0);

    objectList = new ScrollList(listPos, listSize, objects);
    objectList->setVisible(expanded);

    addChild(std::unique_ptr<ScrollList>(objectList));
}

void ObjectSelector::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), dimensions.getY(),
                   NexusColors::Interface, NexusColors::Light, 2);
    
    std::string title = expanded ? "Objects \u25B2" : "Objects \u25BC";
    engine.drawText(absPos.getX() + 10, absPos.getY() + 8, title, NexusColors::Light, 14);
    
    if (expanded) {
        CosmicContainer::render(engine);
    }
}

void ObjectSelector::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (!pressed) return;
    
    QuantumVector localPos = position - getAbsolutePosition();
    
    if (localPos.getY() <= 25) {
        expanded = !expanded;

        if (objectList) {
            objectList->setVisible(expanded);
        }

    } else if (expanded) {
        CosmicContainer::onQuantumClick(position, pressed);

        if (selectionCallback && objectList && objectList->getSelectedIndex() >= 0) {
            selectionCallback (                objectList->getSelectedIndex());
        }
    }
}

void ObjectSelector::updateList(const std::vector<std::string>& newObjects) {
    if (!childElements.empty()) {
         childElements.clear();
    }
    
    QuantumVector listPos(0, 25, 0);
    QuantumVector listSize(dimensions.getX(), dimensions.getY() - 30, 0);

    objectList = new ScrollList(listPos, listSize, newObjects);
    objectList->setVisible(expanded);

    addChild(std::unique_ptr<ScrollList>(objectList));
}

void NexusWindow::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), dimensions.getY(),
                   NexusColors::Interface, NexusColors::Light, 2);
    
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), 30,
                   PhotonColor(80, 90, 150), NexusColors::Light, 1);
    
    engine.drawText(absPos.getX() + 10, absPos.getY() + 8, title, NexusColors::Light, 14);
    
    CosmicContainer::render(engine);
}

NexusWindow::NexusWindow(const QuantumVector& pos, const QuantumVector& size, const std::string& name)
    : CosmicContainer(pos, size), title(name) {}

void NexusWindow::onQuantumClick(const QuantumVector& position, bool pressed) {
    QuantumVector localPos = position - getAbsolutePosition();
    
    if (isTitleBar(localPos)) {
        dragging = pressed;

        if (pressed) {
            dragOffset = localPos;
        }
        return;
    }
    CosmicContainer::onQuantumClick(position, pressed);
}

void NexusWindow::onPhotonMove(const QuantumVector& position) {
    if (dragging) {
        QuantumVector newPos = position - dragOffset;
        setPosition(  newPos);

        return;
    }
    CosmicContainer::onPhotonMove(position);
}

EventFlow NexusWindow::processSignal(CosmicSignal& signal) {
    return ContinueFlow;
}

bool NexusWindow::isTitleBar(const QuantumVector& localPosition) const {
    return localPosition.getY() >= 0 && localPosition.getY() <= 30 && 
           localPosition.getX() >= 0 && localPosition.getX() <= dimensions.getX();
}

ControlNexus::ControlNexus(const QuantumVector& pos, const QuantumVector& size, RayTracer* tracer)
    : NexusWindow(pos, size, "Quantum Control"), rayTracer(tracer) {
    createInterface();
}

void ControlNexus::createInterface() {
    double panelWidth   = dimensions.getX();
    double buttonWidth  = panelWidth - 40;
    double buttonHeight = 30;
    double startY       = 40;
    double spacing      = 10;
    
    auto addSphereBtn = std::make_unique<QuantumButton>(
        QuantumVector(20, startY, 0), QuantumVector(buttonWidth, buttonHeight, 0),
        "Add Sphere", [this]() {
            double x = (std::rand() % 100) / 10.0 - 5.0;
            double y = (std::rand() % 100) / 10.0 - 5.0;
            double z = (std::rand() % 100) / 10.0 + 3.0;
            double r = (std::rand() % 30)  / 10.0 + 0.5;
            
            PhotonColor randomColor(
                std::rand() % 256,
                std::rand() % 256, 
                std::rand() % 256
            );
            
            double reflectivity  = (std::rand() % 100) / 100.0;
            double transparency  = (std::rand() % 50 ) / 100.0;
            
            auto sphere = std::make_unique<CrystalSphere>(
                QuantumVector(x, y, z), r, randomColor, reflectivity, transparency
            );
            rayTracer->addObject(std::move(sphere));
            if (onObjectsChanged) onObjectsChanged();
        }
    );
    addChild(std::move(addSphereBtn));
    
    auto addPyramidBtn = std::make_unique<QuantumButton>(
        QuantumVector(20, startY + buttonHeight + spacing, 0), 
        QuantumVector(buttonWidth, buttonHeight, 0),
        "Add Pyramid", [this]() {
            double x = (std::rand() % 100) / 10.0 - 5.0;
            double y = (std::rand() % 100) / 10.0 - 3.0;
            double z = (std::rand() % 100) / 10.0 + 5.0;
            
            double dirX = (std::rand() % 100 - 50) / 100.0;
            double dirY = (std::rand() % 100     ) / 100.0 + 0.5;
            double dirZ = (std::rand() % 100 - 50) / 100.0;
            
            PhotonColor randomColor(
                std::rand() % 256,
                std::rand() % 256, 
                std::rand() % 256
            );
            
            double baseRadius    = (std::rand() % 30 ) / 10.0 + 0.8;
            int sides            = (std::rand() % 5  ) + 3;
            double reflectivity  = (std::rand() % 100) / 100.0;
            double transparency  = (std::rand() % 30 ) / 100.0;
            
            auto pyramid = std::make_unique<Pyramid>(
                QuantumVector(x, y, z), 
                QuantumVector(dirX, dirY, dirZ),

                baseRadius, sides, randomColor, reflectivity, transparency
            );

            rayTracer->addObject(std::move(pyramid));
            if (onObjectsChanged) onObjectsChanged();
        }
    );
    addChild(std::move(addPyramidBtn));
    
    auto addLightBtn = std::make_unique<QuantumButton>(

        QuantumVector(20, startY + 2*(buttonHeight + spacing), 0),
        QuantumVector(buttonWidth, buttonHeight, 0),
        "Add Light", [this]() {

            double x = (std::rand() % 100) / 10.0 - 5.0;
            double y = (std::rand() % 100) / 10.0 + 1.0;
            double z = (std::rand() % 100) / 10.0 + 2.0;
            
            PhotonColor lightColor(
                std::rand() % 55 + 200,
                std::rand() % 55 + 200,
                std::rand() % 55 + 200
            );
            
            double intensity = (std::rand() % 100) / 100.0 + 0.5;
            
            auto light = std::make_unique<CrystalSphere>(
                QuantumVector(x, y, z), 0.4, lightColor,
                0.0, 0.0, 1.0, 1.0, OBJECT_LIGHT_SOURCE, intensity
            );

            rayTracer->addObject(std::move(light));
            if (onObjectsChanged) onObjectsChanged();
        }
    );

    addChild(std::move(addLightBtn));
    
    auto addPlaneBtn = std::make_unique<QuantumButton>(
        QuantumVector(20, startY + 3*(buttonHeight + spacing), 0),
        QuantumVector(buttonWidth, buttonHeight, 0),
        "Add Plane", [this]() {
            double x = (std::rand() % 100) / 10.0 - 5.0;
            double y = (std::rand() % 100) / 10.0 - 3.0;
            double z = (std::rand() % 100) / 10.0 + 5.0;
            
            PhotonColor randomColor(
                std::rand() % 256,
                std::rand() % 256, 
                std::rand() % 256
            );
            
            double width         = (std::rand() % 50 ) / 10.0 + 2.0;
            double height        = (std::rand() % 50 ) / 10.0 + 2.0;
            double reflectivity  = (std::rand() % 100) / 100.0;
            double transparency  = (std::rand() % 30 ) / 100.0;
            
            auto plane = std::make_unique<FinitePlane>(
                QuantumVector(x, y, z),
                QuantumVector(0, 1, 0),
                QuantumVector(1, 0, 0),
                width, height, randomColor, reflectivity, transparency
            );
            rayTracer->addObject(std::move(plane));
            if (onObjectsChanged) onObjectsChanged();
        }
    );
    addChild(std::move(addPlaneBtn));
    
    auto removeObjectBtn = std::make_unique<QuantumButton>(
        QuantumVector(20, startY + 4*(buttonHeight + spacing), 0),
        QuantumVector(buttonWidth, buttonHeight, 0),
        "Remove Object", [this]() {
            rayTracer->removeLastObject();
            if (onObjectsChanged) onObjectsChanged();
        }
    );
    addChild(std::move(removeObjectBtn));
    
    auto removeLightBtn = std::make_unique<QuantumButton>(
        QuantumVector(20, startY + 5*(buttonHeight + spacing), 0),
        QuantumVector(buttonWidth, buttonHeight, 0),
        "Remove Light", [this]() {
            rayTracer->removeLastLightSource();
            if (onObjectsChanged) onObjectsChanged();
        }
    );
    addChild(std::move(removeLightBtn));
}

InfoNexus::InfoNexus(const QuantumVector& pos, const QuantumVector& size, RayTracer* tracer)
    : NexusWindow(pos, size, "Quantum Info"), rayTracer(tracer) {}

void InfoNexus::render(RenderEngine& engine) {
    NexusWindow::render(engine);
    
    QuantumVector absPos = getAbsolutePosition();
    double x = absPos.getX() + 12;
    double y = absPos.getY() + 45;
    double lineHeight = 22;
    
    std::string objectsText = "Objects: " + std::to_string(rayTracer->getObjectCount());
    engine.drawText(x, y, objectsText, NexusColors::Light, 14);
    y += lineHeight;
    
    std::string lightsText = "Lights: " + std::to_string(rayTracer->getLightCount());
    engine.drawText(x, y, lightsText, NexusColors::Light, 14);
    y += lineHeight;
    
    QuantumVector observerPos = rayTracer->getObserverPosition();
    std::string observerText = "Observer: (" + 
                              std::to_string((int)observerPos.getX()) + ", " +
                              std::to_string((int)observerPos.getY()) + ", " +
                              std::to_string((int)observerPos.getZ()) + ")";
    engine.drawText(x, y, observerText, NexusColors::Light, 14);
    y += lineHeight;
    
    std::string fpsText = "FPS: " + std::to_string(static_cast<int>(currentFPS));
    engine.drawText(x, y, fpsText, NexusColors::Light, 14);
}

void InfoNexus::update(float delta) {
    frameTime += delta;
    frameCount++;
    fpsUpdateTime += delta;
    
    if (fpsUpdateTime >= 0.5) {
        currentFPS = frameCount / fpsUpdateTime;
        frameCount = 0;
        fpsUpdateTime = 0.0;
    }
}

EventFlow InfoNexus::processSignal(CosmicSignal& signal) {
    return ContinueFlow;
}

ObjectManagerNexus::ObjectManagerNexus(const QuantumVector& pos, const QuantumVector& size, RayTracer* tracer)
    : NexusWindow(pos, size, "Object Manager"), rayTracer(tracer) {
    
    std::vector<std::string> initialObjects = {"Sphere 1", "Pyramid 1", "Light 1"};
    
    selectionHandler = [this](int index) {
    };
    
    QuantumVector selectorPos(10, 40, 0);
    QuantumVector selectorSize(size.getX() - 20, 200, 0);
    objectSelector = new ObjectSelector(selectorPos, selectorSize, initialObjects, selectionHandler);
    addChild(std::unique_ptr<ObjectSelector>(objectSelector));
}

void ObjectManagerNexus::render(RenderEngine& engine) {
    NexusWindow::render(engine);
}

void ObjectManagerNexus::updateObjectList() {
    std::vector<std::string> objectNames;
    objectNames.push_back("All Objects");
    objectSelector->updateList(objectNames);
}

CameraNexus::CameraNexus(const QuantumVector& pos, const QuantumVector& size, 
                         std::function<void(int)> handler)
    : NexusWindow(pos, size, "Camera Control"), keyHandler(handler) {
    
    QuantumVector controlsPos(10, 40, 0);
    QuantumVector controlsSize(size.getX() - 20, 150, 0);
    cameraControls = new CameraControls(controlsPos, controlsSize, keyHandler);
    addChild(std::unique_ptr<CameraControls>(cameraControls));
}

void CameraNexus::render(RenderEngine& engine) {
    NexusWindow::render(engine);
}