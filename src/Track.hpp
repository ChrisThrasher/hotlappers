#pragma once

#include <SFML/Graphics.hpp>

class Track : public sf::Drawable {
    sf::VertexArray m_vertices;
    sf::Texture m_track_texture;
    sf::RenderTexture m_render_texture;
    sf::Image m_texture_as_image;

public:
    enum class Surface { ASPHALT = 0, GRASS, GRAVEL };
    Track(const sf::Vector2f& size);
    ~Track() = default;

    void update();

    Surface getSurfaceAtLocation(const sf::Vector2f& location) const;

protected:
    virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const;

private:
};