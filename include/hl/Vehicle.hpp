#pragma once

#include <hl/LowPassFilter.hpp>

#include <SFML/Graphics.hpp>

class Vehicle : public sf::Drawable, private sf::Transformable {
public:
    Vehicle(const sf::Vector2f& position, const sf::Angle& angle);

    void update(const sf::Time& dt, float throttle, float brake, const sf::Angle& steering);
    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

    auto getPosition() const -> sf::Vector2f;
    auto getVelocity() const -> sf::Vector2f;
    auto getRotation() const -> sf::Angle;
    auto getYawRate() const -> sf::Angle;

private:
    void step(float throttle, float brake, const sf::Angle& steering);

    const sf::Time m_timestep { sf::seconds(1.f / 100.f) };
    sf::Time m_time_budget {};

    LowPassFilter<sf::Angle> m_steering { m_timestep, 10.f, sf::degrees(0) };
    sf::Vector2f m_velocity {};
    sf::Angle m_yaw_rate {};
};
