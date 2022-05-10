#include "Track.hpp"

Track::Track(const sf::Vector2f& size)
    : m_vertices { sf::PrimitiveType::TriangleFan }
{
    // Load our texture for the map & also create a render texture
    if (!m_track_texture.loadFromFile("data/textures/grass.png"))
        throw std::runtime_error("Failed to load grass texture for track");

    const auto texture_size = sf::Vector2f { m_track_texture.getSize() };

    if (!m_render_texture.create({ static_cast<sf::Uint32>(size.x), static_cast<sf::Uint32>(size.y) }))
        throw std::runtime_error("Failed to create render texture for track");

    m_vertices.append({ { 0.0f, 0.0f }, { 0.0f, 0.0f } });
    m_vertices.append({ { size.x, 0.0f }, { texture_size.x, 0.0f } });
    m_vertices.append({ { size.x, size.y }, { texture_size.x, texture_size.y } });
    m_vertices.append({ { 0.0f, size.y }, { 0.0f, texture_size.y } });
}

void Track::update()
{
    sf::RenderStates states;
    states.texture = &m_track_texture;

    // We draw ourselves to a texture then copy
    // it to an image because sampling a texture
    // is not possible.
    m_render_texture.clear();
    m_render_texture.draw(m_vertices, states);
    m_render_texture.display();
    m_texture_as_image = m_render_texture.getTexture().copyToImage();
}

Track::Surface Track::getSurfaceAtLocation(const sf::Vector2f& location) const
{
    const auto screen = m_render_texture.mapCoordsToPixel(location);
    const auto c
        = m_texture_as_image.getPixel({ static_cast<sf::Uint32>(screen.x), static_cast<sf::Uint32>(screen.y) });
    (void)screen;
    (void)c;

    // Some kind of colour comparison operation
    // needs to occur here to detect whether grass
    // or asphalt or gravel...

    return Track::Surface::ASPHALT;
}

void Track::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
    // Intellisense thinks this is unintended? Pff, clearly hasn't inherited
    // from sf::Drawable before.
    auto states_copy = states;
    states_copy.texture = &m_track_texture;
    target.draw(m_vertices, states_copy);
}
