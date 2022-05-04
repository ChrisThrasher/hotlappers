#include "Vehicle.hpp"

Vehicle::Vehicle(float timestep, const sf::Vector2f& size, const VehicleProperties& properties)
    : sf::RectangleShape(size)
    , m_timestep(timestep)
    , m_vehicle_properties(properties)
{
    // Can't generate an inverse for 0.0!
    assert(properties.mass != 0.0f);
    assert(properties.inertia != 0.0f);

    m_inverse_mass = 1.0f / properties.mass;
    m_inverse_inertia = 1.0f / properties.inertia;
}

void Vehicle::step(float throttle_percentage, float brake_percentage, const sf::Angle& steering)
{
    (void)steering;

    const auto rotation = getRotation();
    const auto direction_facing = sf::Vector2f(std::cos(rotation.asRadians()), std::sin(rotation.asRadians()));
    const auto speed = m_linear_velocity.length();
    sf::Vector2f direction_velocity;
    if (m_linear_velocity != sf::Vector2f())
        direction_velocity = m_linear_velocity.normalized();

    if (speed < 0.2f)
        m_linear_velocity = {};

    // Straight line forces
    sf::Vector2f traction_force, drag_force, rr_force, braking_force, combined_force;

    traction_force = direction_facing * (m_vehicle_properties.engine_force * throttle_percentage);
    braking_force = -direction_velocity * (m_vehicle_properties.braking_constant * brake_percentage);
    drag_force = -m_vehicle_properties.drag * m_linear_velocity * speed;
    rr_force = m_linear_velocity * -m_vehicle_properties.rr;
    combined_force = traction_force + braking_force + drag_force + rr_force;

    apply_force(combined_force);
    integrate();
}

void Vehicle::integrate()
{
    // Velocity integration
    m_linear_velocity += (m_inverse_mass * m_force) * m_timestep;
    m_angular_velocity += (m_torque * m_inverse_inertia) * m_timestep;

    // Position & orientation integration
    move(m_linear_velocity * m_timestep);
    rotate(sf::radians(m_angular_velocity * m_timestep));

    m_force = {};
    m_torque = 0.0f;
}
