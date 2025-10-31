#ifndef OBJECT_LIST_PANEL_HPP
#define OBJECT_LIST_PANEL_HPP

#include "CosmicElement.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>

class RayTracer;
class OpticalObject;

class DropdownArrow : public CosmicElement {
private:
    bool expanded = false;
    std::function<void(bool)> toggleCallback;
    PhotonColor normalColor;
    PhotonColor hoverColor;

public:
    DropdownArrow(const QuantumVector& pos, const QuantumVector& size, 
                  std::function<void(bool)> callback);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    void onPhotonMove(const QuantumVector& position) override;
    
    bool isExpanded() const { return expanded; }
    void setExpanded(bool exp) { expanded = exp; }
};

class ObjectTypeList : public CosmicElement {
private:
    struct ObjectTypeInfo {
        std::string name;
        int count;
        bool expanded;
    };
    
    std::vector<ObjectTypeInfo> objectTypes;
    PhotonColor bgColor;
    PhotonColor borderColor;
    PhotonColor textColor;
    std::function<void(const std::string&, bool)> expansionCallback;
    int selectedIndex = -1;

public:
    ObjectTypeList(const QuantumVector& pos, const QuantumVector& size);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    
    void updateObjectCounts(const std::vector<std::string>& types, const std::vector<int>& counts);
    void setExpansionCallback(std::function<void(const std::string&, bool)> callback);
    
    int getSelectedIndex() const { return selectedIndex; }
    std::string getSelectedItem() const;
};

class ObjectDetailsPanel : public CosmicElement {
private:
    std::vector<std::string> objectInfos;
    int scrollOffset = 0;
    int selectedIndex = -1;
    PhotonColor bgColor;
    PhotonColor selectedColor;
    PhotonColor textColor;
    PhotonColor borderColor;
    double itemHeight = 22.0;
    int visibleItems = 8;
    
    bool draggingScrollbar = false;
    int dragStartY = 0;
    int dragStartScrollOffset = 0;

public:
    ObjectDetailsPanel(const QuantumVector& pos, const QuantumVector& size);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    void onPhotonMove(const QuantumVector& position) override;
    void onNexusPress(int key, bool pressed) override;
    
    void setObjectInfos(const std::vector<std::string>& infos);
    void clearObjects();
    
private:
    void renderScrollbar(RenderEngine& engine, int x, int y, int w, int h);
    bool isOverScrollbar(const QuantumVector& localPos) const;
    void updateScrollFromDrag(const QuantumVector& localPos);
};

class ObjectListPanel : public CosmicContainer {
private:
    DropdownArrow* arrow;
    ObjectTypeList* typeList;
    bool isExpanded = false;
    RayTracer* rayTracer;
    std::string currentExpandedType;

    ObjectDetailsPanel* pyramidDetails;
    ObjectDetailsPanel* sphereDetails;
    ObjectDetailsPanel* lightDetails;
    ObjectDetailsPanel* planeDetails;

public:
    ObjectListPanel(const QuantumVector& pos, const QuantumVector& size, RayTracer* tracer);
    
    void render(RenderEngine& engine) override;
    void onQuantumClick(const QuantumVector& position, bool pressed) override;
    
    void updateObjectCounts(const std::vector<std::string>& types, const std::vector<int>& counts);
    void toggleExpanded();
    void updateObjectDetails();
    
private:
    void createDetailPanels();
    void showDetailPanel(const std::string& type);
    void hideDetailPanels();
    std::vector<std::string> getObjectInfosByType(const std::string& type) const;
};

#endif