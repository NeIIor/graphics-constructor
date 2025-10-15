#include "ControlPanel.hpp"
#include "Button.hpp"
#include "../graphics/SFMLSystem.hpp"
#include <iostream>

ControlPanel::ControlPanel(RayTracer& ray_tracer, const VectorD& pos, const VectorD& size)
    : Window("Control Panel", pos, size), ray_tracer_(ray_tracer) {
    createUI();
}

void ControlPanel::createUI() {
    double panel_width = getSize().getX();
    double panel_height = getSize().getY();
    
    double button_width = panel_width - 40;
    double button_height = 30;
    double start_x = 55;
    double start_y = 100;
    double spacing = 10;
    
    auto add_sphere = std::make_unique<Button>("Add Sphere", VectorD(start_x, start_y, 0), VectorD(button_width, button_height));
    add_sphere->setOnClick([this]() {
        double x = (rand() % 100) / 10.0 - 5.0;
        double y = (rand() % 100) / 10.0 - 5.0;
        double z = (rand() % 100) / 10.0;
        double r = (rand() % 30) / 10.0 + 0.5;
        
        unsigned long red = rand() % 256;
        unsigned long green = rand() % 256;
        unsigned long blue = rand() % 256;
        
        double reflectivity = (rand() % 100) / 100.0;
        
        auto sphere = std::make_unique<Sphere>(
            VectorD(x, y, z), r, Color(red, green, blue), reflectivity
        );
        ray_tracer_.addSphere(std::move(sphere));
        std::cout << "Added sphere at (" << x << ", " << y << ", " << z << ")" << std::endl;
    });
    addChild(std::move(add_sphere));
    
    auto add_light = std::make_unique<Button>("Add Light", VectorD(start_x, start_y + button_height + spacing, 0), VectorD(button_width, button_height));
    add_light->setOnClick([this]() {
        double x = (rand() % 100) / 10.0 - 5.0;
        double y = (rand() % 100) / 10.0 - 5.0;
        double z = (rand() % 100) / 10.0 - 5.0;
        
        auto light = std::make_unique<Light>(
            VectorD(x, y, z), Color(255, 255, 255), 1.0
        );
        ray_tracer_.addLight(std::move(light));
        std::cout << "Added light at (" << x << ", " << y << ", " << z << ")" << std::endl;
    });
    addChild(std::move(add_light));
    
    auto remove_sphere = std::make_unique<Button>("Remove Sphere", VectorD(start_x, start_y + 2*(button_height + spacing), 0), VectorD(button_width, button_height));
    remove_sphere->setOnClick([this]() {
        ray_tracer_.removeLastSphere();
        std::cout << "Removed last sphere" << std::endl;
    });
    addChild(std::move(remove_sphere));
    
    auto remove_light = std::make_unique<Button>("Remove Light", VectorD(start_x, start_y + 3*(button_height + spacing), 0), VectorD(button_width, button_height));
    remove_light->setOnClick([this]() {
        ray_tracer_.removeLastLight();
        std::cout << "Removed last light" << std::endl;
    });
    addChild(std::move(remove_light));
}