#include "App.hpp"
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

// These can probably find a better home but I made them here just for now
// Let's find them a foster/forever family soon...
constexpr VehicleProperties test_vehicle { 2220.0f, 1048.0f, 50000, 108891.0f, 0.3f, 0.3f * 12.0f };
constexpr auto time_step = 1.0f / 100.0f;

App::App()
    : m_window(sf::VideoMode(800, 600), "Hotlappers [indev]", sf::Style::Default ^ sf::Style::Resize)
    , m_vehicle(time_step, { 128.0, 64.0f }, test_vehicle)
{
    m_window.setFramerateLimit(60);

    // Setup vehicle rect colour & centred origin
    m_vehicle.setFillColor(sf::Color::Yellow);
    m_vehicle.setOrigin(m_vehicle.getSize() * 0.5f);

    const auto centre = sf::Vector2f(m_window.getSize()) * 0.5f;
    m_vehicle.setPosition(centre);
    // Imgui
    if (!ImGui::SFML::Init(m_window))
        throw std::runtime_error("Failed to initialize ImGui");
}

App::~App() { ImGui::SFML::Shutdown(m_window); }

void App::run()
{
    sf::Clock delta_clock;
    while (m_window.isOpen()) {
        const auto dt = delta_clock.restart();

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();

            ImGui::SFML::ProcessEvent(event);
            handle_inputs(event);
        }

        ImGui::SFML::Update(m_window, dt);
        update(dt);

        m_window.clear({ 75, 75, 75 });
        m_window.draw(m_vehicle);
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}

void App::update(const sf::Time& dt)
{
    static sf::Time accumulator = sf::Time::Zero;
    auto throttle = 0.0f;
    auto brake = 0.0f;
    sf::Angle steering;

    if (m_input_state.throttle)
        throttle = 1.0f;

    if (m_input_state.brake)
        brake = 1.0f;
    // here we clamp the max value we can add to the accumulator
    // otherwise hanging on a breakpoint will cause the physics
    // step loop to go on for an eternity locking up the app
    if (dt.asSeconds() < 0.25f)
        accumulator += dt;
    else
        accumulator += sf::seconds(0.25f);

    while (accumulator >= dt) {
        m_vehicle.step(throttle, brake, steering);
        accumulator -= sf::seconds(time_step);
    }

    update_average_fps(dt);

    if (m_show_imgui) {
        ImGui::Begin("Developer Tools");
        ImGui::BeginTabBar("Car Metedata");
        if (ImGui::BeginTabItem("Vehicle Sim Data")) {

            ImGui::Text("Position: {%.2f %.2f}", m_vehicle.getPosition().x, m_vehicle.getPosition().y);
            ImGui::Text("Orientation (degrees): %.2f", m_vehicle.getRotation().asDegrees());

            ImGui::Text("Speed (m/s): %.2f", m_vehicle.get_linear_velocity().length());
            ImGui::Text(
                "Velocity (m/s): {%.2f %.2f}", m_vehicle.get_linear_velocity().x, m_vehicle.get_linear_velocity().y);
            ImGui::Text("Angular Velocity (rad/s): %.2f", m_vehicle.get_angular_velocity());
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Application Statistics")) {
            ImGui::Text("FPS: %.2f", m_average_fps);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
        ImGui::End();
    }
}

void App::update_average_fps(const sf::Time& dt)
{
    // FPS/DT printing to log (horrible & simple but
    // pretty common to have in game project)
    static int frame_counter = 0;
    static sf::Time dt_sum = sf::Time::Zero;

    if (frame_counter < 10) {
        dt_sum += dt;
        ++frame_counter;
    } else {
        const auto dt_average = dt_sum.asSeconds() / static_cast<float>(frame_counter);
        m_average_fps = 1.0f / dt_average;
        frame_counter = 0;
        dt_sum = sf::Time::Zero;
    }
}

void App::handle_inputs(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W)
            m_input_state.throttle = true;

        if (event.key.code == sf::Keyboard::S)
            m_input_state.brake = true;

        if (event.key.code == sf::Keyboard::A)
            m_input_state.steer_left = true;

        if (event.key.code == sf::Keyboard::D)
            m_input_state.steer_right = true;

        if (event.key.code == sf::Keyboard::Escape)
            m_window.close();
    }

    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::W)
            m_input_state.throttle = false;

        if (event.key.code == sf::Keyboard::S)
            m_input_state.brake = false;

        if (event.key.code == sf::Keyboard::A)
            m_input_state.steer_left = false;

        if (event.key.code == sf::Keyboard::D)
            m_input_state.steer_right = false;

        if (event.key.code == sf::Keyboard::F3)
            m_show_imgui = !m_show_imgui;
    }
}
