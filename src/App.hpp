#pragma once

#include <hl/Vehicle.hpp>

class App {
    sf::RenderWindow m_window { sf::VideoMode({800, 600}), "Hotlappers [indev]", sf::Style::Default ^ sf::Style::Resize };
    Vehicle m_vehicle { sf::Vector2f(m_window.getSize()) / 2.f, sf::Angle::Zero };
    float m_average_fps { 0.f };
    bool m_show_imgui { true };
    struct {
        bool throttle { false };
        bool brake { false };
        bool steer_left { false };
        bool steer_right { false };
    } m_input_state;

public:
    App();
    ~App();

    void run();

private:
    void handleInputs(const sf::Event& event);
    void update(const sf::Time& dt);
    void updateAverageFPS(const sf::Time& dt);
};
