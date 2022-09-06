#include <hl/Vehicle.hpp>

#include <catch2/catch_all.hpp>

using namespace sf::Literals;
using namespace std::literals;

namespace sf {
std::ostream& operator<<(std::ostream& out, const Vector2f& v) { return out << '(' << v.x << ", " << v.y << ')'; }

std::ostream& operator<<(std::ostream& out, const Angle& a) { return out << a.asDegrees() << " deg"; }
}

TEST_CASE("Vehicle class")
{
    SECTION("Construction")
    {
        const Vehicle vehicle({ 1, 2 }, 3_deg);
        CHECK(vehicle.getPosition() == sf::Vector2f(1, 2));
        CHECK(vehicle.getVelocity() == sf::Vector2f());
        CHECK(vehicle.getRotation() == 3_deg);
        CHECK(vehicle.getYawRate() == 0_deg);
    }

    SECTION("Drive forward then brake")
    {
        Vehicle vehicle({}, {});
        vehicle.update(1s, 1, {}, {});
        CHECK(vehicle.getPosition().x > 0.f);
        CHECK(vehicle.getPosition().y == 0.f);
        CHECK(vehicle.getVelocity().x > 0.f);
        CHECK(vehicle.getVelocity().y == 0.f);
        CHECK(vehicle.getRotation() == 0_deg);
        CHECK(vehicle.getYawRate() == 0_deg);

        vehicle.update(1s, {}, 1, {});
        CHECK(vehicle.getPosition().x > 0.f);
        CHECK(vehicle.getPosition().y == 0.f);
        CHECK(vehicle.getVelocity().x == Catch::Approx(0).margin(0.1));
        CHECK(vehicle.getVelocity().y == 0.f);
        CHECK(vehicle.getRotation() == 0_deg);
        CHECK(vehicle.getYawRate() == 0_deg);
    }
}
