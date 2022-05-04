#pragma once

#include "Vehicle.hpp"
#include <SFML/Graphics.hpp>

class App {
    struct InputState {
        bool throttle { false };
        bool brake { false };
        bool steer_left { false };
        bool steer_right { false };
    };

    sf::RenderWindow m_window;
    InputState m_input_state;
    Vehicle m_vehicle;
    float m_average_fps { 0.0f };
    bool m_show_imgui { true };

public:
    App();
    ~App();

    void run();

private:
    void handle_inputs(const sf::Event& event);
    void update(const sf::Time& dt);
    void update_average_fps(const sf::Time& dt);
};
