#include <hl/Vehicle.hpp>

Vehicle::Vehicle(const sf::Vector2f& position, const sf::Angle angle)
{
    setPosition(position);
    setRotation(angle);
}

void Vehicle::step(const float throttle, const float brake, const sf::Angle steering)
{
    assert(throttle >= 0);
    assert(throttle <= 1);
    assert(brake >= 0);
    assert(brake <= 1);

    m_steering.update(steering);

    const auto heading = sf::Vector2f(1, getRotation());
    const auto speed = m_velocity.x > 0 ? m_velocity.length() : -m_velocity.length();

    // Force accumulation
    const auto throttle_force = 100 * throttle * heading;
    const auto resistence_force = -1.f * m_velocity;
    const auto braking_force = -8.f * brake * speed * heading;
    const auto accel = throttle_force + resistence_force + braking_force;

    const auto steering_torque = 0.1f * m_steering.value() * speed;
    const auto resistence_torque = -m_yaw_rate;
    const auto yaw_accel = steering_torque + resistence_torque;

    // Velocity integration
    m_velocity += accel * m_timestep.asSeconds();
    m_yaw_rate += yaw_accel * m_timestep.asSeconds();

    // Pose integration
    move(m_velocity * m_timestep.asSeconds());
    rotate(m_yaw_rate * m_timestep.asSeconds());
}

void Vehicle::update(const sf::Time dt, const float throttle, const float brake, const sf::Angle steering)
{
    m_time_budget += dt;
    for (; m_time_budget >= m_timestep; m_time_budget -= m_timestep)
        step(throttle, brake, steering);
}

void Vehicle::draw(sf::RenderTarget& target, const sf::RenderStates& /*states*/) const
{
    auto rectangle = sf::RectangleShape({ 64, 32 });
    rectangle.setOrigin(rectangle.getSize() / 2.f);
    rectangle.setPosition(getPosition());
    rectangle.setRotation(getRotation());
    rectangle.setFillColor(sf::Color::Yellow);
    target.draw(rectangle);
    const auto transform = rectangle.getTransform();

    const auto half_size = rectangle.getSize() / 2.f;

    auto fl_wheel = sf::RectangleShape(half_size / 2.f);
    fl_wheel.setOrigin(fl_wheel.getSize() / 2.f);
    fl_wheel.setPosition(transform.transformPoint(sf::Vector2f(half_size.x, -half_size.y) + rectangle.getOrigin()));
    fl_wheel.setRotation(getRotation() + m_steering.value());
    fl_wheel.setFillColor(sf::Color::Black);
    target.draw(fl_wheel);

    auto fr_wheel = sf::RectangleShape(half_size / 2.f);
    fr_wheel.setOrigin(fr_wheel.getSize() / 2.f);
    fr_wheel.setPosition(transform.transformPoint(sf::Vector2f(half_size.x, half_size.y) + rectangle.getOrigin()));
    fr_wheel.setRotation(getRotation() + m_steering.value());
    fr_wheel.setFillColor(sf::Color::Black);
    target.draw(fr_wheel);

    auto bl_wheel = sf::RectangleShape(half_size / 2.f);
    bl_wheel.setOrigin(bl_wheel.getSize() / 2.f);
    bl_wheel.setPosition(transform.transformPoint(sf::Vector2f(-half_size.x, -half_size.y) + rectangle.getOrigin()));
    bl_wheel.setRotation(getRotation());
    bl_wheel.setFillColor(sf::Color::Black);
    target.draw(bl_wheel);

    auto rr_wheel = sf::RectangleShape(half_size / 2.f);
    rr_wheel.setOrigin(rr_wheel.getSize() / 2.f);
    rr_wheel.setPosition(transform.transformPoint(sf::Vector2f(-half_size.x, half_size.y) + rectangle.getOrigin()));
    rr_wheel.setRotation(getRotation());
    rr_wheel.setFillColor(sf::Color::Black);
    target.draw(rr_wheel);
}

auto Vehicle::getPosition() const -> sf::Vector2f { return sf::Transformable::getPosition(); }

auto Vehicle::getVelocity() const -> sf::Vector2f { return m_velocity; }

auto Vehicle::getRotation() const -> sf::Angle { return sf::Transformable::getRotation(); }

auto Vehicle::getYawRate() const -> sf::Angle { return m_yaw_rate; }
