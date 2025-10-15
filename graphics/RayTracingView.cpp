#include "RayTracingView.hpp"
#include "SFMLSystem.hpp"

RayTracingView::RayTracingView(RayTracer& ray_tracer, CameraController& camera_controller, const VectorD& pos, const VectorD& size)
    : ray_tracer_(ray_tracer), camera_controller_(camera_controller) {
    setPosition(pos);
    setSize(size);
    width_ = size.getX();
    height_ = size.getY();
    framebuffer_.resize(width_ * height_);
}

void RayTracingView::onMouseDown(const VectorD& position) {
    if (contains(position)) {
        focused_ = true;
    } else {
        focused_ = false;
    }
    
    Widget::onMouseDown(position);
}

void RayTracingView::onMouseMove(const VectorD& position) {
    Widget::onMouseMove(position);
}

void RayTracingView::onKeyDown(int key) {
    if (focused_) {
        camera_controller_.handleKeyPress(key);
        
        ray_tracer_.setCameraPosition(camera_controller_.getPosition());
        ray_tracer_.setCameraDirection(camera_controller_.getDirection());
        
        std::cout << "Camera updated - Pos: " << camera_controller_.getPosition() 
                  << " Dir: " << camera_controller_.getDirection() << std::endl;
    }
    Widget::onKeyDown(key);
}

void RayTracingView::render(SFMLSystem& system) {
    if (!isVisible()) return;
    
    double x = getPosition().getX();
    double y = getPosition().getY();
    double w = getSize().getX();
    double h = getSize().getY();
    
    Color border_color = focused_ ? Color(255, 200, 0) : Color(255, 255, 255);
    system.drawRectangle(x, y, w, h, Color(10, 10, 20), border_color, 3);
    
    renderFrame();
    
    for (int py = 0; py < height_; py += 2) {
        for (int px = 0; px < width_; px += 2) {
            Color color = framebuffer_[py * width_ + px];
            system.drawRectangle(x + px, y + py, 2, 2, color);
        }
    }
    
    if (focused_) {
        system.drawText(x + 10, y + 10, "3D View [FOCUSED - Use WASD QE ZX]", Color(255, 200, 0), 12);
    } else {
        system.drawText(x + 10, y + 10, "3D View [Click to focus]", Color(200, 200, 200), 12);
    }
}

void RayTracingView::update(float dt) {
}

void RayTracingView::renderFrame() {
    VectorD camera_pos = ray_tracer_.getCameraPosition();
    VectorD camera_dir = ray_tracer_.getCameraDirection();
    
    VectorD camera_right = ~(VectorD(0, 1, 0) ^ camera_dir);
    VectorD camera_up = ~(camera_dir ^ camera_right);
    
    std::cout << "Rendering - Pos: " << camera_pos << " Dir: " << camera_dir << std::endl;
    
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            double ndc_x = (x + 0.5) / width_ * 2.0 - 1.0;
            double ndc_y = 1.0 - (y + 0.5) / height_ * 2.0;
            
            double aspect_ratio = (double)width_ / height_;
            ndc_x *= aspect_ratio;
            
            VectorD ray_dir = ~(camera_dir + camera_right * ndc_x + camera_up * ndc_y);
            Color color = ray_tracer_.traceRay(camera_pos, ray_dir);
            framebuffer_[y * width_ + x] = color;
        }
    }
}
Color RayTracingView::getPixelColor(int x, int y) const {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        return framebuffer_[y * width_ + x];
    }
    return Color(0, 0, 0);
}   