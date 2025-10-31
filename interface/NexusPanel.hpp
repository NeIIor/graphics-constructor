#ifndef NEXUS_PANEL_HPP
#define NEXUS_PANEL_HPP

#include "CosmicElement.hpp"
#include <functional>
#include <string>
#include <vector>

class QuantumButton : public CosmicElement {
private:
    std::string label;
    std::function<void()> action;
    PhotonColor baseColor;
    PhotonColor hoverColor;
    PhotonColor pressColor;
    bool hoverState = false;
    bool pressState = false;

public:
    QuantumButton(const QuantumVector& pos, const QuantumVector& size, 
                  const std::string& text, std::function<void()> callback);
    
    void render(RenderEngine& engine) override;
    void onPhotonMove(const QuantumVector& position) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
};

class ScrollList : public CosmicElement {
private:
    std::vector<std::string> items;
    int selectedIndex = -1;
    int scrollOffset = 0;
    int visibleItems = 5;
    PhotonColor selectedColor;
    PhotonColor normalColor;

public:
    ScrollList(const QuantumVector& pos, const QuantumVector& size, 
               const std::vector<std::string>& itemList);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    void onNexusPress(int key, bool pressed) override;
    
    int getSelectedIndex() const { return selectedIndex; }
    std::string getSelectedItem() const;
};

class CameraControls : public CosmicElement {
private:
    std::function<void(int)> keyHandler;

public:
    CameraControls(const QuantumVector& pos, const QuantumVector& size, 
                   std::function<void(int)> handler);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
};

class ObjectSelector : public CosmicContainer {
private:
    ScrollList* objectList;
    std::function<void(int)> selectionCallback;
    bool expanded = false;

public:
    ObjectSelector(const QuantumVector& pos, const QuantumVector& size, 
                   const std::vector<std::string>& objects,
                   std::function<void(int)> callback);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    
    void updateList(const std::vector<std::string>& newObjects);
};

class NexusWindow : public CosmicContainer {
private:
    std::string title;
    bool dragging = false;
    QuantumVector dragOffset;

public:
    NexusWindow(const QuantumVector& pos, const QuantumVector& size, const std::string& name);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    void onPhotonMove(const QuantumVector& position) override;
    EventFlow processSignal(CosmicSignal& signal) override;

private:
    bool isTitleBar(const QuantumVector& position) const;
};

class ControlNexus : public NexusWindow {
private:
    class RayTracer* rayTracer;
    std::function<void()> onObjectsChanged;

public:
    ControlNexus(const QuantumVector& pos, const QuantumVector& size, class RayTracer* tracer);
    
    void setOnObjectsChanged(std::function<void()> callback) {
        onObjectsChanged = callback;
    }
    
    void createInterface();
};
class InfoNexus : public NexusWindow {
private:
    class RayTracer* rayTracer;
    double frameTime = 0.0;
    int frameCount = 0;
    double fpsUpdateTime = 0.0;
    double currentFPS = 0.0;

public:
    InfoNexus(const QuantumVector& pos, const QuantumVector& size, class RayTracer* tracer);
    void render(RenderEngine& engine) override;
    void update(float delta);
    EventFlow processSignal(CosmicSignal& signal) override;
};

class ObjectManagerNexus : public NexusWindow {
private:
    class RayTracer* rayTracer;
    ObjectSelector* objectSelector;
    std::function<void(int)> selectionHandler;

public:
    ObjectManagerNexus(const QuantumVector& pos, const QuantumVector& size, class RayTracer* tracer);
    void render(RenderEngine& engine) override;
    void updateObjectList();
};

class CameraNexus : public NexusWindow {
private:
    CameraControls* cameraControls;
    std::function<void(int)> keyHandler;

public:
    CameraNexus(const QuantumVector& pos, const QuantumVector& size, 
                std::function<void(int)> handler);
    void render(RenderEngine& engine) override;
};

#endif