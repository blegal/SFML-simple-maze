//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef MAZE_TILEMAP_H
#define MAZE_TILEMAP_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Tiles/MazeTiles.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
    sf::Vector2u tileSize;
    unsigned int width;
    unsigned int height;

public:

    bool load(const std::string& tileset, sf::Vector2u _tileSize, const uint8_t* tiles, unsigned int _width, unsigned int _height);

    virtual void setHeroPosition(const uint32_t x, const uint32_t y, const uint32_t sprite);

    virtual void setTile(const uint32_t x, const uint32_t y, const uint32_t sprite);

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};


#endif //MAZE_TILEMAP_H
