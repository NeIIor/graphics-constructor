#include "CosmicView.hpp"
#include "../system/RenderEngine.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <future>

static void renderRegionTask(RayTracer& tracer, std::vector<PhotonColor>& buffer, 
                            int width, int height, int startY, int endY,
                            const QuantumVector& observerPos, const QuantumVector& observerDir,
                            const QuantumVector& right, const QuantumVector& up,
                            double aspectRatio, double fov, std::atomic<int>& completedRows) {
    for (int y = startY; y < endY; y++) {
        for (int x = 0; x < width; x++) {
            double ndcX = (x + 0.5) / width * 2.0 - 1.0;
            double ndcY = 1.0 - (y + 0.5) / height * 2.0;
            
            ndcX *= aspectRatio * fov;
            ndcY *= fov;
            
            QuantumVector rayDir = (observerDir + right * ndcX + up * ndcY).normalize();
            PhotonColor color = tracer.traceRay(observerPos, rayDir);
            buffer[y * width + x] = color;
        }
        completedRows++;
    }
}

CosmicView::CosmicView(const QuantumVector& pos, const QuantumVector& size, 
           RayTracer& tracer, ObserverController& controller)
    : CosmicElement(pos, size), photonTracer(tracer), observer(controller) {
    // Рендерим в увеличенном разрешении для качества
    bufferWidth = static_cast<int>(size.getX()) * 2;
    bufferHeight = static_cast<int>(size.getY()) * 2;
    frameBuffer.resize(bufferWidth * bufferHeight);
    
    threadCount = std::max(1u, std::thread::hardware_concurrency());
}

void CosmicView::render(RenderEngine& engine) {
    if (!visible) return;
    
    QuantumVector absPos = getAbsolutePosition();
    
    PhotonColor borderColor = focused ? NexusColors::Plasma : NexusColors::Light;
    engine.drawRect(absPos.getX(), absPos.getY(), dimensions.getX(), dimensions.getY(),
                   NexusColors::Void, borderColor, 3);
    
    if (needsRedraw) {
        renderFrameAsync();
    }
    
    if (isRendering) {
        double progress = static_cast<double>(completedRows) / bufferHeight;
        std::string progressText = "Rendering: " + std::to_string(static_cast<int>(progress * 100)) + "%";
        engine.drawText(absPos.getX() + 10, absPos.getY() + dimensions.getY() - 30, 
                       progressText, NexusColors::Plasma, 12);
    }
    
    // Масштабируем отображение - показываем каждый пиксель из буфера
    for (int py = 0; py < bufferHeight; py++) {
        for (int px = 0; px < bufferWidth; px++) {
            PhotonColor color = frameBuffer[py * bufferWidth + px];
            // Масштабируем координаты для отображения в области просмотра
            int displayX = absPos.getX() + px / 2;
            int displayY = absPos.getY() + py / 2;
            engine.drawRect(displayX, displayY, 1, 1, color);
        }
    }
    
    std::string status = focused ? 
        "QUANTUM VIEW [ACTIVE - WASD Move, QE Rotate, ZX Look]" : 
        "QUANTUM VIEW [Click to activate]";
    PhotonColor textColor = focused ? NexusColors::Plasma : NexusColors::Crystal;
    engine.drawText(absPos.getX() + 10, absPos.getY() + 10, status, textColor, 12);
    
    if (focused) {
        QuantumVector pos = observer.getPosition();
        std::string posText = "Pos: " + 
                             std::to_string((int)pos.getX()) + "," +
                             std::to_string((int)pos.getY()) + "," + 
                             std::to_string((int)pos.getZ());
        engine.drawText(absPos.getX() + 10, absPos.getY() + 30, posText, NexusColors::Crystal, 10);
    }
}

void CosmicView::renderFrameAsync() {
    if (isRendering) {
        if (completedRows >= bufferHeight) {
            isRendering = false;
            needsRedraw = false;
            renderFuture.get();
        }
        return;
    }
    
    isRendering = true;
    completedRows = 0;
    
    renderFuture = std::async(std::launch::async, [this]() {
        renderFrame();
    });
}

void CosmicView::renderFrame() {
    QuantumVector observerPos = photonTracer.getObserverPosition();
    QuantumVector observerDir = photonTracer.getObserverDirection();
    
    QuantumVector right = QuantumVector(0, 1, 0).cross(observerDir).normalize();
    QuantumVector up = observerDir.cross(right).normalize();
    
    double aspectRatio = static_cast<double>(bufferWidth) / bufferHeight;
    double fov = 1.0;
    
    std::vector<std::future<void>> futures;
    int rowsPerThread = bufferHeight / threadCount;
    
    for (int t = 0; t < threadCount; ++t) {
        int startY = t * rowsPerThread;
        int endY = (t == threadCount - 1) ? bufferHeight : (t + 1) * rowsPerThread;
        
        futures.push_back(std::async(std::launch::async, renderRegionTask,
            std::ref(photonTracer), std::ref(frameBuffer), 
            bufferWidth, bufferHeight, startY, endY,
            observerPos, observerDir, right, up, aspectRatio, fov, std::ref(completedRows)));
    }
    
    for (auto& future : futures) {
        future.get();
    }
}

void CosmicView::onQuantumClick(const QuantumVector& position, bool pressed) {
    if (contains(position)) {
        if (pressed) {
            focused = true;
            needsRedraw = true;
        }
    } else if (pressed) {
        focused = false;
        needsRedraw = true;
    }
    
    CosmicElement::onQuantumClick(position, pressed);
}

void CosmicView::onNexusPress(int key, bool pressed) {
    if (focused && pressed) {
        observer.handleKey(key);
        photonTracer.setObserverPosition(observer.getPosition());
        photonTracer.setObserverDirection(observer.getDirection());
        needsRedraw = true;
    }
    
    CosmicElement::onNexusPress(key, pressed);
}

EventFlow CosmicView::processSignal(CosmicSignal& signal) {
    if (auto pressSignal = dynamic_cast<NexusPressSignal*>(&signal)) {
        if (focused && pressSignal->isPressed()) {
            observer.handleKey(pressSignal->getKey());
            photonTracer.setObserverPosition(observer.getPosition());
            photonTracer.setObserverDirection(observer.getDirection());
            needsRedraw = true;
            return StopFlow;
        }
    }
    
    if (dynamic_cast<QuantumClickSignal*>(&signal)) {
        return ContinueFlow;
    }
    
    return CosmicElement::processSignal(signal);
}