//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "TileMap.hpp"


bool TileMap::load(const std::string& tileset, sf::Vector2u _tileSize, const uint8_t* tiles, unsigned int _width, unsigned int _height)
{
    tileSize = _tileSize;
    width    = _width;
    height   = _height;

    if ( !m_tileset.loadFromFile(tileset) )
    {
        return false;
    }

    printf("(DD) Tile loading is successfull :%dx%d\n", width, height);
    printf("(DD) - Tile size    : (%3dx%3d)\n", tileSize.x, tileSize.y);
    printf("(DD) - Texture size : (%3dx%3d)\n", m_tileset.getSize().x, m_tileset.getSize().y);

    // on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // on remplit le tableau de vertex, avec un quad par tuile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // on récupère le numéro de tuile courant
            int tileNumber = tiles[i + j * width];

            // on en déduit sa position dans la texture du tileset
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // on récupère un pointeur vers le quad à définir dans le tableau de vertex
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // on définit ses quatre coins
            quad[0].position = sf::Vector2f( i      * tileSize.x,  j      * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x,  j      * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f( i      * tileSize.x, (j + 1) * tileSize.y);

            // on définit ses quatre coordonnées de texture
            quad[0].texCoords = sf::Vector2f( tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f( tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}


void TileMap::setHeroPosition(const uint32_t x, const uint32_t y, const uint32_t sprite)
{

}


void TileMap::setTile(const uint32_t x, const uint32_t y, const uint32_t sprite)
{
    int tileNumber   = sprite;
    int tu           = tileNumber % (m_tileset.getSize().x / tileSize.x);
    int tv           = tileNumber / (m_tileset.getSize().x / tileSize.x);
    sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

    // on définit ses quatre coordonnées de texture
    quad[0].texCoords = sf::Vector2f( tu      * tileSize.x,  tv      * tileSize.y);
    quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x,  tv      * tileSize.y);
    quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
    quad[3].texCoords = sf::Vector2f( tu      * tileSize.x, (tv + 1) * tileSize.y);
}


void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // on applique la transformation
    states.transform *= getTransform();

    // on applique la texture du tileset
    states.texture = &m_tileset;

    // et on dessine enfin le tableau de vertex
    target.draw(m_vertices, states);
}
