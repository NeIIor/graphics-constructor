#include "system/RenderEngine.hpp"
#include "interface/NexusPanel.hpp"
#include "interface/CameraControlsPanel.hpp"  
#include "interface/ObjectListPanel.hpp"
#include "rendering/CosmicView.hpp"
#include "rendering/PhotonTracer.hpp"
#include <iostream>
#include <thread>
#include <chrono>

const int WINDOW_WIDTH  = 1800;
const int WINDOW_HEIGHT = 1000;

int main() {
    try {
        RenderEngine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "Quantum Nexus - Refraction Demo");
        
        auto photonTracer = std::make_unique<RayTracer>();
        auto observerController = std::make_unique<ObserverController>(QuantumVector(0, 2, -8));
        
        photonTracer->setObserverPosition(observerController->getPosition());
        photonTracer->setObserverDirection(observerController->getDirection());
        
        auto root = std::make_unique<CosmicContainer>(
            QuantumVector(0, 0, 0), QuantumVector(WINDOW_WIDTH, WINDOW_HEIGHT, 0)
        );
        
        auto cosmicView = std::make_unique<CosmicView>(
            QuantumVector(300, 50, 0), QuantumVector(1000, 700, 0),
            *photonTracer, *observerController
        );
        
        auto objectListPanel = std::make_unique<ObjectListPanel>(
            QuantumVector(1325, 370, 0), QuantumVector(150, 30, 0),
            photonTracer.get()  
        );
        ObjectListPanel* objectListPanelPtr = objectListPanel.get();
        
        auto controlPanel = std::make_unique<ControlNexus>(
            QuantumVector(20, 50, 0), QuantumVector(250, 450, 0), 
            photonTracer.get()
        );
        
        auto infoPanel = std::make_unique<InfoNexus>(
            QuantumVector(20, 520, 0), QuantumVector(250, 230, 0), photonTracer.get()
        );
        InfoNexus* infoPanelPtr = infoPanel.get();
        
        auto cameraPanel = std::make_unique<CameraControlsPanel>(
            QuantumVector(1325, 50, 0), QuantumVector(250, 300, 0), 
            [&observerController, &photonTracer](int key) { 
                observerController->handleKey(key);
                photonTracer->setObserverPosition(observerController->getPosition());
                photonTracer->setObserverDirection(observerController->getDirection());
            }
        );

        auto updateObjectStats = [photonTracer = photonTracer.get(), objectListPanelPtr]() {
            if (photonTracer && objectListPanelPtr) {
                photonTracer->updateObjectStatistics();
                std::vector<std::string> types = {"Pyramids", "Spheres", "LightSources", "Planes"};
                std::vector<int> counts = {
                    photonTracer->getPyramidCount(),
                    photonTracer->getSphereCount(), 
                    photonTracer->getLightSourceCount(),
                    photonTracer->getPlaneCount()
                };
                objectListPanelPtr->updateObjectCounts(types, counts);
            }
        };

        controlPanel->setOnObjectsChanged(updateObjectStats);

        root->addChild(std::move(cosmicView));
        root->addChild(std::move(controlPanel));
        root->addChild(std::move(infoPanel));
        root->addChild(std::move(cameraPanel));
        root->addChild(std::move(objectListPanel));

        // Яркий источник света
        photonTracer->addObject(std::make_unique<CrystalSphere>(
            QuantumVector(0, 10, 5), 1.0, PhotonColor(255, 255, 220),
            0.0, 0.0, 1.0, 1.0, OBJECT_LIGHT_SOURCE, 3.0
        ));
        
        // Пол
        photonTracer->addObject(std::make_unique<FinitePlane>(
            QuantumVector(0, -2, 0), QuantumVector(0, 1, 0), QuantumVector(1, 0, 0),
            50.0, 50.0, PhotonColor(100, 100, 100), 0.1, 0.0, 1.0, 32.0
        ));

        // Преломляющая сфера (стекло)
        photonTracer->addObject(std::make_unique<CrystalSphere>(
            QuantumVector(0, 0, 10), 1.5, PhotonColor(200, 200, 255),
            0.05, 0.9, 1.5, 128.0
        ));

        // Кубик сзади сферы (4 пирамиды как куб)
        photonTracer->addObject(std::make_unique<Pyramid>(
            QuantumVector(0, 0, 16), QuantumVector(0, 1, 0), 1.0, 4,
            PhotonColor(255, 100, 100), 0.8, 0.0, 1.0, 64.0
        ));

        updateObjectStats();

        auto lastTime = std::chrono::high_resolution_clock::now();
        auto lastFpsTime = std::chrono::high_resolution_clock::now();
        int frameCounter = 0;
        double fps = 0.0;
        
        while (engine.isRunning()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            // Отладочный вывод FPS
            frameCounter++;
            auto fpsCurrentTime = std::chrono::high_resolution_clock::now();
            double fpsElapsed = std::chrono::duration<double>(fpsCurrentTime - lastFpsTime).count();
            
            if (fpsElapsed >= 2.0) {
                fps = frameCounter / fpsElapsed;
                std::cout << "Real FPS: " << fps << std::endl;
                frameCounter = 0;
                lastFpsTime = fpsCurrentTime;
            }
            
            if (infoPanelPtr) {
                infoPanelPtr->update(deltaTime);
            }
            
            engine.processEvents(root.get());
            engine.beginFrame();
            root->render(engine);
            engine.endFrame();
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}