#include "App.hpp"

App::App()
{
    m_window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(m_window))
        throw std::runtime_error("Failed to initialize ImGui");
}

App::~App() { ImGui::SFML::Shutdown(m_window); }

void App::run()
{
    sf::Clock clock;
    while (m_window.isOpen()) {
        const auto dt = clock.restart();

        for (sf::Event event; m_window.pollEvent(event);) {
            if (event.type == sf::Event::Closed)
                m_window.close();

            ImGui::SFML::ProcessEvent(event);
            handleInputs(event);
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
    auto throttle = 0.f;
    if (m_input_state.throttle)
        throttle = 1.f;

    auto brake = 0.f;
    if (m_input_state.brake)
        brake = 1.f;

    auto steering = sf::degrees(0);
    if (m_input_state.steer_left)
        steering = sf::degrees(-10.f);
    if (m_input_state.steer_right)
        steering = sf::degrees(10.f);

    // here we clamp the max value we can add to the accumulator
    // otherwise hanging on a breakpoint will cause the physics
    // step loop to go on for an eternity locking up the app
    auto accumulator = sf::Time::Zero;
    if (dt < sf::seconds(0.25f))
        accumulator += dt;
    else
        accumulator += sf::seconds(0.25f);

    m_vehicle.update(accumulator, throttle, brake, steering);

    updateAverageFPS(dt);

    if (m_show_imgui) {
        ImGui::Begin("Developer Tools");
        ImGui::BeginTabBar("Car Metedata");
        if (ImGui::BeginTabItem("Vehicle Sim Data")) {

            ImGui::Text("Position: {%.2f %.2f}", m_vehicle.getPosition().x, m_vehicle.getPosition().y);
            ImGui::Text("Heading (degrees): %.2f", m_vehicle.getRotation().asDegrees());

            ImGui::Text("Speed (m/s): %.2f", m_vehicle.getVelocity().length());
            ImGui::Text("Velocity (m/s): {%.2f %.2f}", m_vehicle.getVelocity().x, m_vehicle.getVelocity().y);
            ImGui::Text("Yaw rate (rad/s): %.2f", m_vehicle.getYawRate().asRadians());
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

void App::updateAverageFPS(const sf::Time& dt)
{
    static auto frame_counter = 0;
    static auto dt_sum = sf::Time::Zero;

    if (frame_counter < 10) {
        dt_sum += dt;
        ++frame_counter;
    } else {
        m_average_fps = static_cast<float>(frame_counter) / dt_sum.asSeconds();
        frame_counter = {};
        dt_sum = {};
    }
}

void App::handleInputs(const sf::Event& event)
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
