#include <hl/LowPassFilter.hpp>

#include <catch2/catch_all.hpp>

TEST_CASE("LowPassFilter class")
{
    const auto cutoff_frequency = 1.f;
    const auto initial_value = 0.f;
    LowPassFilter lpf(sf::seconds(0.1f), cutoff_frequency, initial_value);
    CHECK(lpf.value() == initial_value);
    auto previous = initial_value;
    for (size_t i = 0; i < 100; ++i) {
        lpf.update(1.f);
        CHECK(lpf.value() > 0.f);
        CHECK(lpf.value() < 1.f);
        CHECK(lpf.value() > previous);
        previous = lpf.value();
    }
}
