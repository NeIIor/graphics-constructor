#include "ObjectListPanel.hpp"
#include "../system/RenderEngine.hpp"
#include "../rendering/PhotonTracer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

DropdownArrow::DropdownArrow(const QuantumVector& pos, const QuantumVector& size, 
                             std::function<void(bool)> callback)
    : CosmicElement(pos, size), toggleCallback(callback),
      normalColor(NexusColors::Interface),
      hoverColor(NexusColors::Quantum) {}

void DropdownArrow::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    int x = static_cast<int>(absPos.getX());
    int y = static_cast<int>(absPos.getY());
    int w = static_cast<int>(dimensions.getX());
    int h = static_cast<int>(dimensions.getY());
    
    PhotonColor currentColor = normalColor;
    
    engine.drawRect(x, y, w, h, currentColor, NexusColors::Light, 2);
    
    double textWidth = engine.getTextWidth("Objects", 14);
    double textHeight = engine.getTextHeight(11);
    int textX = x + 5;
    int textY = y + static_cast<int>((h - textHeight) / 2);
    
    engine.drawText(textX, textY, "Objects", NexusColors::Light, 14);
    
    int arrowX = x + w - 12;
    int arrowY = y + h / 2;
    int arrowSize = 4;
    
    if (expanded) {
        engine.drawLine(arrowX, arrowY - arrowSize, arrowX - arrowSize, arrowY + arrowSize, 
                       NexusColors::Light, 2);
        engine.drawLine(arrowX, arrowY - arrowSize, arrowX + arrowSize, arrowY + arrowSize, 
                       NexusColors::Light, 2);
    } else {
        engine.drawLine(arrowX - arrowSize, arrowY - arrowSize, arrowX, arrowY + arrowSize, 
                       NexusColors::Light, 2);
        engine.drawLine(arrowX + arrowSize, arrowY - arrowSize, arrowX, arrowY + arrowSize, 
                       NexusColors::Light, 2);
    }
}

void DropdownArrow::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (!pressed && contains(position)) {
        expanded = !expanded;
        if (toggleCallback) {
            toggleCallback(expanded);
        }
    }
}

void DropdownArrow::onPhotonMove(const QuantumVector& position) {}

ObjectTypeList::ObjectTypeList(const QuantumVector& pos, const QuantumVector& size)
    : CosmicElement(pos, size),
      bgColor(NexusColors::Interface),
      borderColor(NexusColors::Light),
      textColor(NexusColors::Light) {
    
    objectTypes = {
        {"Pyramids", 0, false},
        {"Spheres", 0, false},
        {"LightSources", 0, false},
        {"Planes", 0, false}
    };
}

void ObjectTypeList::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    int x = static_cast<int>(absPos.getX());
    int y = static_cast<int>(absPos.getY());
    int w = static_cast<int>(dimensions.getX());
    int h = static_cast<int>(dimensions.getY());
    
    engine.drawRect(x, y, w, h, bgColor, borderColor, 2);
    
    int itemHeight = 25;
    int currentY = y + 5;
    
    for (size_t i = 0; i < objectTypes.size(); i++) {
        if (currentY + itemHeight > y + h) break;
        
        const auto& objType = objectTypes[i];
        
        std::string itemText = objType.name + ": " + std::to_string(objType.count);
        
        PhotonColor itemBg = (i == selectedIndex) ? NexusColors::Plasma : bgColor;
        engine.drawRect(x + 2, currentY, w - 4, itemHeight - 2, itemBg, borderColor, 1);
        
        engine.drawText(x + 5, currentY + 4, itemText, textColor, 13);
        
        int arrowX = x + w - 15;
        int arrowY = currentY + itemHeight / 2;
        int arrowSize = 4;
        
        if (objType.expanded) {
            engine.drawLine(arrowX, arrowY - arrowSize, arrowX - arrowSize, arrowY + arrowSize, 
                           textColor, 1);
            engine.drawLine(arrowX, arrowY - arrowSize, arrowX + arrowSize, arrowY + arrowSize, 
                           textColor, 1);
        } else {
            engine.drawLine(arrowX - arrowSize, arrowY - arrowSize, arrowX, arrowY + arrowSize, 
                           textColor, 1);
            engine.drawLine(arrowX + arrowSize, arrowY - arrowSize, arrowX, arrowY + arrowSize, 
                           textColor, 1);
        }
        
        currentY += itemHeight;
    }
}

void ObjectTypeList::updateObjectCounts(const std::vector<std::string>& types, const std::vector<int>& counts) {
    for (size_t i = 0; i < objectTypes.size() && i < types.size(); ++i) {
        objectTypes[i].name = types[i];
        objectTypes[i].count = counts[i];
    }
}

void ObjectTypeList::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (!pressed) return;
    
    QuantumVector localPos = position - getAbsolutePosition();
    int itemHeight = 20;
    int clickedIndex = static_cast<int>((localPos.getY() - 5) / itemHeight);
    
    if (clickedIndex >= 0 && clickedIndex < static_cast<int>(objectTypes.size())) {
        for (auto& objType : objectTypes) {
            objType.expanded = false;
        }
        
        objectTypes[clickedIndex].expanded = !objectTypes[clickedIndex].expanded;
        selectedIndex = clickedIndex;
        
        if (expansionCallback) {
            expansionCallback(objectTypes[clickedIndex].name, objectTypes[clickedIndex].expanded);
        }
    }
}

std::string ObjectTypeList::getSelectedItem() const {
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(objectTypes.size())) {
        return objectTypes[selectedIndex].name;
    }
    return "";
}

void ObjectTypeList::setExpansionCallback(std::function<void(const std::string&, bool)> callback) {
    expansionCallback = callback;
}

ObjectDetailsPanel::ObjectDetailsPanel(const QuantumVector& pos, const QuantumVector& size)
    : CosmicElement(pos, size),
      bgColor(NexusColors::Interface),
      selectedColor(NexusColors::Plasma),
      textColor(NexusColors::Light),
      borderColor(NexusColors::Light) {}

void ObjectDetailsPanel::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    int x = static_cast<int>(absPos.getX());
    int y = static_cast<int>(absPos.getY());
    int w = static_cast<int>(dimensions.getX());
    int h = static_cast<int>(dimensions.getY());
    
    engine.drawRect(x, y, w, h, bgColor, borderColor, 2);
    
    engine.drawRect(x, y, w, 25, PhotonColor(60, 70, 120), borderColor, 1);
    engine.drawText(x + 5, y + 4, "Object Details", textColor, 14);
    
    int contentY = y + 30;
    int contentHeight = h - 30;
    
    for (int i = 0; i < visibleItems && (i + scrollOffset) < objectInfos.size(); ++i) {
        int itemIndex = i + scrollOffset;
        int itemY = contentY + static_cast<int>(i * itemHeight);
        
        if (itemY + itemHeight > y + h) break;
        
        PhotonColor itemBgColor = (itemIndex == selectedIndex) ? selectedColor : bgColor;
        
        engine.drawRect(x + 2, itemY, w - 4, static_cast<int>(itemHeight) - 2, 
                       itemBgColor, borderColor, 1);
        
        std::string info = objectInfos[itemIndex];
        engine.drawText(x + 5, itemY + 4, info, textColor, 12);
    }
    
    if (objectInfos.size() > visibleItems) {
        renderScrollbar(engine, x, contentY, w, contentHeight);
    }
}

void ObjectDetailsPanel::renderScrollbar(RenderEngine& engine, int x, int y, int w, int h) {
    int scrollbarWidth = 12;
    int scrollbarX = x + w - scrollbarWidth;
    
    engine.drawRect(scrollbarX, y, scrollbarWidth, h, 
                   PhotonColor(50, 60, 100), borderColor, 1);
    
    if (objectInfos.size() <= visibleItems) return;
    
    double contentRatio = static_cast<double>(visibleItems) / objectInfos.size();
    int thumbHeight = static_cast<int>(h * contentRatio);
    thumbHeight = std::max(thumbHeight, 20);
    
    double scrollRatio = static_cast<double>(scrollOffset) / (objectInfos.size() - visibleItems);
    int thumbY = y + static_cast<int>(scrollRatio * (h - thumbHeight));
    
    thumbY = std::max(y, std::min(thumbY, y + h - thumbHeight));
    
    PhotonColor thumbColor = draggingScrollbar ? NexusColors::Plasma : NexusColors::Quantum;
    engine.drawRect(scrollbarX, thumbY, scrollbarWidth, thumbHeight, 
                   thumbColor, borderColor, 1);
}

bool ObjectDetailsPanel::isOverScrollbar(const QuantumVector& localPos) const {
    if (objectInfos.size() <= visibleItems) return false;
    
    int scrollbarWidth = 12;
    int scrollbarX = static_cast<int>(dimensions.getX()) - scrollbarWidth;
    
    return localPos.getX() >= scrollbarX && 
           localPos.getY() >= 25 && 
           localPos.getY() <= dimensions.getY() - 5;
}

void ObjectDetailsPanel::updateScrollFromDrag(const QuantumVector& localPos) {
    if (objectInfos.size() <= visibleItems) return;
    
    int contentY = 25;
    int contentHeight = static_cast<int>(dimensions.getY()) - 30;
    
    double dragY = localPos.getY() - contentY;
    double trackHeight = contentHeight;
    
    double thumbHeight = trackHeight * (static_cast<double>(visibleItems) / objectInfos.size());
    thumbHeight = std::max(thumbHeight, 20.0);
    
    double availableScroll = objectInfos.size() - visibleItems;
    double dragRatio = (dragY - thumbHeight / 2) / (trackHeight - thumbHeight);
    dragRatio = std::max(0.0, std::min(1.0, dragRatio));
    
    int newScrollOffset = static_cast<int>(dragRatio * availableScroll + 0.5);
    scrollOffset = std::max(0, std::min(newScrollOffset, static_cast<int>(objectInfos.size()) - visibleItems));
}

void ObjectDetailsPanel::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (!contains(position)) return;
    
    QuantumVector localPos = position - getAbsolutePosition();
    
    if (localPos.getY() >= 25 && localPos.getY() <= dimensions.getY() - 5) {
        if (pressed) {
            if (isOverScrollbar(localPos) && objectInfos.size() > visibleItems) {
                draggingScrollbar = true;
                dragStartY = static_cast<int>(localPos.getY());
                dragStartScrollOffset = scrollOffset;
                updateScrollFromDrag(localPos);
            } else {
                int contentY = static_cast<int>(localPos.getY() - 25);
                int clickedIndex = contentY / static_cast<int>(itemHeight) + scrollOffset;
                
                if (clickedIndex >= 0 && clickedIndex < static_cast<int>(objectInfos.size())) {
                    selectedIndex = clickedIndex;
                }
            }
        } else {
            draggingScrollbar = false;
        }
    } else {
        draggingScrollbar = false;
    }
}

void ObjectDetailsPanel::onPhotonMove(const QuantumVector& position) {
    if (draggingScrollbar) {
        QuantumVector localPos = position - getAbsolutePosition();
        updateScrollFromDrag(localPos);
    }
}

void ObjectDetailsPanel::onNexusPress(int key, bool pressed) {
    if (!pressed) return;
    
    switch (key) {
        case 73:
            if (scrollOffset > 0) scrollOffset--;
            if (selectedIndex > 0) selectedIndex--;
            if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
            break;
        case 74:
            if (scrollOffset < static_cast<int>(objectInfos.size()) - visibleItems) scrollOffset++;
            if (selectedIndex < static_cast<int>(objectInfos.size()) - 1) selectedIndex++;
            if (selectedIndex >= scrollOffset + visibleItems) scrollOffset = selectedIndex - visibleItems + 1;
            break;
        case 75:
            scrollOffset = std::max(0, scrollOffset - visibleItems);
            if (selectedIndex >= 0) {
                selectedIndex = std::max(0, selectedIndex - visibleItems);
            }
            break;
        case 78:
            scrollOffset = std::min(static_cast<int>(objectInfos.size()) - visibleItems, scrollOffset + visibleItems);
            if (selectedIndex >= 0) {
                selectedIndex = std::min(static_cast<int>(objectInfos.size()) - 1, selectedIndex + visibleItems);
            }
            break;
        case 58:
            scrollOffset = 0;
            selectedIndex = 0;
            break;
        case 62:
            scrollOffset = std::max(0, static_cast<int>(objectInfos.size()) - visibleItems);
            selectedIndex = std::max(0, static_cast<int>(objectInfos.size()) - 1);
            break;
    }
}

void ObjectDetailsPanel::setObjectInfos(const std::vector<std::string>& infos) {
    objectInfos = infos;
    selectedIndex = -1;
    scrollOffset = 0;
    draggingScrollbar = false;
}

void ObjectDetailsPanel::clearObjects() {
    objectInfos.clear();
    selectedIndex = -1;
    scrollOffset = 0;
    draggingScrollbar = false;
}

ObjectListPanel::ObjectListPanel(const QuantumVector& pos, const QuantumVector& size, RayTracer* tracer)
    : CosmicContainer(pos, size), rayTracer(tracer), currentExpandedType(""),
      pyramidDetails(nullptr), sphereDetails(nullptr), lightDetails(nullptr), planeDetails(nullptr) {
    
    auto arrowBtn = std::make_unique<DropdownArrow>(
        QuantumVector(0, 0, 0), 
        QuantumVector(150, 28, 0),
        [this](bool expanded) { toggleExpanded(); }
    );
    arrow = arrowBtn.get();
    addChild(std::move(arrowBtn));
    
    auto typeListElem = std::make_unique<ObjectTypeList>(
        QuantumVector(0, 33, 0),
        QuantumVector(180, 150, 0)
    );
    typeList = typeListElem.get();
    typeList->setVisible(false);
    
    typeList->setExpansionCallback([this](const std::string& type, bool expanded) {
        if (expanded) {
            showDetailPanel(type);
        } else {
            hideDetailPanels();
        }
    });
    
    addChild(std::move(typeListElem));
    
    createDetailPanels();
}

void ObjectListPanel::createDetailPanels() {
    auto pyramidPanel = std::make_unique<ObjectDetailsPanel>(
        QuantumVector(0, 160, 0),
        QuantumVector(450, 280, 0)
    );
    pyramidDetails = pyramidPanel.get();
    pyramidDetails->setVisible(false);
    addChild(std::move(pyramidPanel));
    
    auto spherePanel = std::make_unique<ObjectDetailsPanel>(
        QuantumVector(0, 160, 0),
        QuantumVector(450, 280, 0)
    );
    sphereDetails = spherePanel.get();
    sphereDetails->setVisible(false);
    addChild(std::move(spherePanel));
    
    auto lightPanel = std::make_unique<ObjectDetailsPanel>(
        QuantumVector(0, 160, 0),
        QuantumVector(450, 280, 0)
    );
    lightDetails = lightPanel.get();
    lightDetails->setVisible(false);
    addChild(std::move(lightPanel));
    
    auto planePanel = std::make_unique<ObjectDetailsPanel>(
        QuantumVector(0, 160, 0),
        QuantumVector(450, 280, 0)
    );
    planeDetails = planePanel.get();
    planeDetails->setVisible(false);
    addChild(std::move(planePanel));
}

void ObjectListPanel::showDetailPanel(const std::string& type) {
    hideDetailPanels();
    currentExpandedType = type;
    
    updateObjectDetails();
    
    if (type == "Pyramids" && pyramidDetails) {
        pyramidDetails->setVisible(true);
    }
    else if (type == "Spheres" && sphereDetails) {
        sphereDetails->setVisible(true);
    }
    else if (type == "LightSources" && lightDetails) {
        lightDetails->setVisible(true);
    }
    else if (type == "Planes" && planeDetails) {
        planeDetails->setVisible(true);
    }
}

void ObjectListPanel::hideDetailPanels() {
    if (pyramidDetails) pyramidDetails->setVisible(false);
    if (sphereDetails) sphereDetails->setVisible(false);
    if (lightDetails) lightDetails->setVisible(false);
    if (planeDetails) planeDetails->setVisible(false);
    currentExpandedType = "";
}

void ObjectListPanel::updateObjectDetails() {
    if (currentExpandedType.empty() || !rayTracer) return;
    
    auto objectInfos = getObjectInfosByType(currentExpandedType);
    
    if (currentExpandedType == "Pyramids" && pyramidDetails) {
        pyramidDetails->setObjectInfos(objectInfos);
    }
    else if (currentExpandedType == "Spheres" && sphereDetails) {
        sphereDetails->setObjectInfos(objectInfos);
    }
    else if (currentExpandedType == "LightSources" && lightDetails) {
        lightDetails->setObjectInfos(objectInfos);
    }
    else if (currentExpandedType == "Planes" && planeDetails) {
        planeDetails->setObjectInfos(objectInfos);
    }
}

std::vector<std::string> ObjectListPanel::getObjectInfosByType(const std::string& type) const {
    if (!rayTracer) return {};
    return rayTracer->getObjectInfosByType(type);
}

void ObjectListPanel::render(RenderEngine& engine) {
    if (!visible) return;
    
    CosmicContainer::render(engine);
}

void ObjectListPanel::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (isExpanded || (arrow && arrow->contains(position))) {
        CosmicContainer::onQuantumClick(position, pressed);
    } else {
        if (arrow) {
            arrow->onQuantumClick(position, pressed);
        }
    }
}

void ObjectListPanel::updateObjectCounts(const std::vector<std::string>& types, const std::vector<int>& counts) {
    if (typeList) {
        typeList->updateObjectCounts(types, counts);
    }
}

void ObjectListPanel::toggleExpanded() {
    isExpanded = !isExpanded;
    if (typeList) {
        typeList->setVisible(isExpanded);
    }
    
    if (!isExpanded) {
        hideDetailPanels();
    }
    
    if (isExpanded) {
        dimensions = QuantumVector(200, 160, 0);
    } else {
        dimensions = QuantumVector(150, 28, 0);
    }
}