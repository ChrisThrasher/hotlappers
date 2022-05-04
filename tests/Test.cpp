#include "Vehicle.hpp"

#include <catch2/catch_all.hpp>

using Approx = Catch::Approx;

TEST_CASE("Vehicle Simulation")
{
    SECTION("Applying Forces & Torque")
    {
        constexpr auto fps = 60;
        constexpr auto timestep = 1.0f / static_cast<float>(fps);

        VehicleProperties prop;
        prop.mass = 2.0f;
        prop.inertia = 2.0f;
        Vehicle vehicle(timestep, {}, prop);

        for (auto i = 0; i < fps; ++i) {
            vehicle.apply_force({ 1000, 0.0f });
            vehicle.apply_torque(100.0f);
            vehicle.step(0.0f, 0.0f, sf::degrees(0.0f));
        }

        const auto speed = vehicle.get_linear_velocity().length();
        const auto angular_speed = vehicle.get_angular_velocity(); // rad/s

        CHECK(speed == Approx(500.0));
        CHECK(angular_speed == Approx(50.00));
    }
}
