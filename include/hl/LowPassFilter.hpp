#pragma once

#include <SFML/System/Time.hpp>

#include <cmath>

template <typename T>
class LowPassFilter {
public:
    LowPassFilter(const sf::Time dt, float cutoff_frequency, T initial_value)
        : m_dt(dt)
        , m_cutoff_frequency(cutoff_frequency)
        , m_previous(initial_value)
    {
    }

    void update(const T input)
    {
        const auto a = std::exp(-m_cutoff_frequency * m_dt.asSeconds());
        m_previous = m_previous * a + input * (1 - a);
    }

    [[nodiscard]] T value() const { return m_previous; }

private:
    sf::Time m_dt;
    float m_cutoff_frequency;
    T m_previous;
};
