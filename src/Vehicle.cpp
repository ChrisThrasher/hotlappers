#include <hl/Vehicle.hpp>

/* Vehicle & Physics related constants */
static constexpr auto mass = 1540.f;
static constexpr auto inverse_mass = 1.f / mass;
static constexpr auto inertia = 2000.f;
static constexpr auto inverse_inertia = 1.f / inertia;
static constexpr auto engine_force = 35000;
static constexpr auto braking_constant = 108891.f;
static constexpr auto drag = 0.3f;
static constexpr auto rr = 0.3f * 12.f;
static constexpr auto metres_per_pixel = 0.0541666666666667f;

/* Graphics related constants */
static constexpr auto vehicle_rect_size = sf::Vector2f { 48.f, 32.f };

Vehicle::Vehicle(const sf::Vector2f& position, const sf::Angle& angle)
    : m_body_shape(vehicle_rect_size)
{
    // Setup Vehicle transform
    setOrigin(m_body_shape.getSize() * 0.5f);
    setPosition(position);
    setRotation(angle);

    // Setup graphical properties on vehicle
    m_body_shape.setFillColor(sf::Color::Yellow);
}

void Vehicle::step(float throttle, float brake, const sf::Angle& steering)
{
    assert(throttle >= 0.f);
    assert(throttle <= 1.f);
    assert(brake >= 0.f);
    assert(brake <= 1.f);

    m_steering.update(steering);

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

    // Cornering forces
    auto turnRadius = 0.f;
    if (steering != sf::Angle::Zero) {
        turnRadius = (vehicle_rect_size.x * metres_per_pixel) / std::sin(steering.asRadians());
        m_angular_velocity = sf::radians(m_velocity.length() / turnRadius);
    } else {
        m_angular_velocity = sf::radians(0.f);
    }

    // Velocity integration
    auto torque = sf::degrees(0);
    m_velocity += (inverse_mass * combined_force) * m_timestep.asSeconds();
    m_angular_velocity += (torque * inverse_inertia) * m_timestep.asSeconds();

    // Pose integration
    m_velocity = m_velocity.rotatedBy(m_angular_velocity * m_timestep.asSeconds());
    move((m_velocity * m_timestep.asSeconds()) / metres_per_pixel);
    rotate(m_angular_velocity * m_timestep.asSeconds());
}

void Vehicle::update(const sf::Time& dt, float throttle, float brake, const sf::Angle& steering)
{
    m_time_budget += dt;
    for (; m_time_budget >= m_timestep; m_time_budget -= m_timestep)
        step(throttle, brake, steering);
}

void Vehicle::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
    auto statesCopy = states;
    statesCopy.transform *= getTransform();
    target.draw(m_body_shape, statesCopy);
    const auto transform = getTransform();

    auto half_size = vehicle_rect_size / 2.f;

    auto fl_wheel = sf::RectangleShape(half_size / 2.f);
    fl_wheel.setOrigin(fl_wheel.getSize() / 2.f);
    fl_wheel.setPosition(transform.transformPoint(sf::Vector2f(half_size.x, -half_size.y) + getOrigin()));
    fl_wheel.setRotation(getRotation() + m_steering.value());
    fl_wheel.setFillColor(sf::Color::Black);
    target.draw(fl_wheel);

    auto fr_wheel = sf::RectangleShape(half_size / 2.f);
    fr_wheel.setOrigin(fr_wheel.getSize() / 2.f);
    fr_wheel.setPosition(transform.transformPoint(sf::Vector2f(half_size.x, half_size.y) + getOrigin()));
    fr_wheel.setRotation(getRotation() + m_steering.value());
    fr_wheel.setFillColor(sf::Color::Black);
    target.draw(fr_wheel);

    auto bl_wheel = sf::RectangleShape(half_size / 2.f);
    bl_wheel.setOrigin(bl_wheel.getSize() / 2.f);
    bl_wheel.setPosition(transform.transformPoint(sf::Vector2f(-half_size.x, -half_size.y) + getOrigin()));
    bl_wheel.setRotation(getRotation());
    bl_wheel.setFillColor(sf::Color::Black);
    target.draw(bl_wheel);

    auto rr_wheel = sf::RectangleShape(half_size / 2.f);
    rr_wheel.setOrigin(rr_wheel.getSize() / 2.f);
    rr_wheel.setPosition(transform.transformPoint(sf::Vector2f(-half_size.x, half_size.y) + getOrigin()));
    rr_wheel.setRotation(getRotation());
    rr_wheel.setFillColor(sf::Color::Black);
    target.draw(rr_wheel);
}

auto Vehicle::getPosition() const -> sf::Vector2f { return sf::Transformable::getPosition(); }

auto Vehicle::getVelocity() const -> sf::Vector2f { return m_velocity; }

auto Vehicle::getRotation() const -> sf::Angle { return sf::Transformable::getRotation(); }

auto Vehicle::getYawRate() const -> sf::Angle { return m_angular_velocity; }
