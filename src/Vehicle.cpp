#include <hl/Vehicle.hpp>

static constexpr auto timestep = sf::seconds(1.0f / 100.0f);
static constexpr auto mass = 2220.0f;
static constexpr auto inverse_mass = 1.f / mass;
static constexpr auto inertia = 1048.0f;
static constexpr auto inverse_inertia = 1.f / inertia;
static constexpr auto engine_force = 50000;
static constexpr auto braking_constant = 108891.0f;
static constexpr auto drag = 0.3f;
static constexpr auto rr = 0.3f * 12.0f;

Vehicle::Vehicle(const sf::Vector2f& position, const sf::Angle& angle)
{
    setPosition(position);
    setRotation(angle);
}

void Vehicle::step(float throttle, float brake, const sf::Angle& steering)
{
    assert(throttle >= 0.f);
    assert(throttle <= 1.f);
    assert(brake >= 0.f);
    assert(brake <= 1.f);
    m_steering = steering;

    const auto rotation = getRotation();
    const auto direction_facing = sf::Vector2f(std::cos(rotation.asRadians()), std::sin(rotation.asRadians()));
    const auto speed = m_velocity.length();
    sf::Vector2f direction_velocity;
    if (m_velocity != sf::Vector2f())
        direction_velocity = m_velocity.normalized();

    if (speed < 0.2f)
        m_velocity = {};

    // Straight line forces
    const auto traction_force = direction_facing * (engine_force * throttle);
    const auto braking_force = -direction_velocity * (braking_constant * brake);
    const auto drag_force = -drag * m_velocity * speed;
    const auto rr_force = m_velocity * -rr;
    const auto combined_force = traction_force + braking_force + drag_force + rr_force;

    // Velocity integration
    auto torque = sf::degrees(0);
    m_velocity += (inverse_mass * combined_force) * timestep.asSeconds();
    m_yaw_rate += (torque * inverse_inertia) * timestep.asSeconds();

    // Pose integration
    move(m_velocity * timestep.asSeconds());
    rotate(m_yaw_rate * timestep.asSeconds());
}

void Vehicle::update(const sf::Time& dt, float throttle, float brake, const sf::Angle& steering)
{
    m_time_budget += dt;
    for (; m_time_budget >= timestep; m_time_budget -= timestep)
        step(throttle, brake, steering);
}

void Vehicle::draw(sf::RenderTarget& target, const sf::RenderStates& /*states*/) const
{
    auto rectangle = sf::RectangleShape({ 128, 64 });
    rectangle.setOrigin(rectangle.getSize() / 2.f);
    rectangle.setPosition(getPosition());
    rectangle.setRotation(getRotation());
    rectangle.setFillColor(sf::Color::Yellow);
    target.draw(rectangle);

    auto half_size = rectangle.getSize() / 2.f;

    auto fl_wheel = sf::RectangleShape(half_size / 2.f);
    fl_wheel.setOrigin(fl_wheel.getSize() / 2.f);
    fl_wheel.setPosition(getPosition() + sf::Vector2f(half_size.x, -half_size.y));
    fl_wheel.setRotation(m_steering);
    fl_wheel.setFillColor(sf::Color::Black);
    target.draw(fl_wheel);

    auto fr_wheel = sf::RectangleShape(half_size / 2.f);
    fr_wheel.setOrigin(fr_wheel.getSize() / 2.f);
    fr_wheel.setPosition(getPosition() + sf::Vector2f(half_size.x, half_size.y));
    fr_wheel.setRotation(m_steering);
    fr_wheel.setFillColor(sf::Color::Black);
    target.draw(fr_wheel);

    auto bl_wheel = sf::RectangleShape(half_size / 2.f);
    bl_wheel.setOrigin(bl_wheel.getSize() / 2.f);
    bl_wheel.setPosition(getPosition() + sf::Vector2f(-half_size.x, -half_size.y));
    bl_wheel.setFillColor(sf::Color::Black);
    target.draw(bl_wheel);

    auto rr_wheel = sf::RectangleShape(half_size / 2.f);
    rr_wheel.setOrigin(rr_wheel.getSize() / 2.f);
    rr_wheel.setPosition(getPosition() + sf::Vector2f(-half_size.x, half_size.y));
    rr_wheel.setFillColor(sf::Color::Black);
    target.draw(rr_wheel);
}

auto Vehicle::getPosition() const -> sf::Vector2f { return sf::Transformable::getPosition(); }

auto Vehicle::getVelocity() const -> sf::Vector2f { return m_velocity; }

auto Vehicle::getRotation() const -> sf::Angle { return sf::Transformable::getRotation(); }

auto Vehicle::getYawRate() const -> sf::Angle { return m_yaw_rate; }
