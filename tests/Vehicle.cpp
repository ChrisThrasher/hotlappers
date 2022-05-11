#include <hl/Vehicle.hpp>

#include <catch2/catch_all.hpp>

using Approx = Catch::Approx;

namespace sf {
std::ostream& operator<<(std::ostream& out, const Vector2f& v) { return out << '(' << v.x << ", " << v.y << ')'; }

std::ostream& operator<<(std::ostream& out, const Angle& a) { return out << a.asDegrees() << " deg"; }
}

TEST_CASE("Vehicle class")
{
    SECTION("Construction")
    {
        const Vehicle vehicle({ 1, 2 }, sf::degrees(3));
        CHECK(vehicle.getPosition() == sf::Vector2f(1, 2));
        CHECK(vehicle.getVelocity() == sf::Vector2f());
        CHECK(vehicle.getRotation() == sf::degrees(3));
        CHECK(vehicle.getYawRate() == sf::degrees(0));
    }

    SECTION("Drive forward then brake")
    {
        Vehicle vehicle({}, {});
        vehicle.update(sf::seconds(1), 1.f, {}, {});
        CHECK(vehicle.getPosition().x > 0.f);
        CHECK(vehicle.getPosition().y == 0.f);
        CHECK(vehicle.getVelocity().x > 0.f);
        CHECK(vehicle.getVelocity().y == 0.f);
        CHECK(vehicle.getRotation() == sf::degrees(0));
        CHECK(vehicle.getYawRate() == sf::degrees(0));

        vehicle.update(sf::seconds(1), {}, 1.f, {});
        CHECK(vehicle.getPosition().x > 0.f);
        CHECK(vehicle.getPosition().y == 0.f);
        CHECK(vehicle.getVelocity().x == Approx(0).margin(0.001));
        CHECK(vehicle.getVelocity().y == 0.f);
        CHECK(vehicle.getRotation() == sf::degrees(0));
        CHECK(vehicle.getYawRate() == sf::degrees(0));
    }
}
