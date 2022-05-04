#pragma once

#include <SFML/Graphics.hpp>

struct VehicleProperties {
    float mass { 1.0f };
    float inertia { 1.0f };
    float engine_force { 0.0f };
    float braking_constant { 0.0f };
    float drag { 0.0f };
    float rr { 0.0f };
};

class Vehicle : public sf::RectangleShape {
    const float m_timestep;
    // Important values for physics object
    sf::Vector2f m_linear_velocity;
    sf::Vector2f m_force;

    float m_angular_velocity { 0.0f };
    float m_torque { 0.0f };
    float m_inverse_inertia; // calculated on construction & cached to avoid always calculating
    float m_inverse_mass; // calculated on construction & cached to avoid always calculating

    VehicleProperties m_vehicle_properties;

public:
    Vehicle(float timestep, const sf::Vector2f& size, const VehicleProperties& properties);

    void step(float throttle, float brake, const sf::Angle& steering);

    void apply_force(const sf::Vector2f& force) { m_force += force; }
    void apply_torque(float torque) { m_torque += torque; }

    sf::Vector2f get_linear_velocity() const { return m_linear_velocity; }
    float get_angular_velocity() const { return m_angular_velocity; }

private:
    void integrate();
};
