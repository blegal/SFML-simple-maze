#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

//SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class TileMap : public sf::Drawable, public sf::Transformable
{
    sf::Vector2u tileSize;
    unsigned int width;
    unsigned int height;

public:

    bool load(const std::string& tileset, sf::Vector2u _tileSize, const uint8_t* tiles, unsigned int _width, unsigned int _height)
    {
        tileSize = _tileSize;
        width    = _width;
        height   = _height;

        // on charge la texture du tileset
        if (!m_tileset.loadFromFile(tileset))
            return false;

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
                quad[0].position = sf::Vector2f( i * tileSize.x,      j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // on définit ses quatre coordonnées de texture
                quad[0].texCoords = sf::Vector2f( tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f( tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

    virtual void setHeroPosition(const uint32_t x, const uint32_t y, const uint32_t sprite)
    {
    }

    virtual void setTile(const uint32_t x, const uint32_t y, const uint32_t sprite)
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

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // on applique la transformation
        states.transform *= getTransform();

        // on applique la texture du tileset
        states.texture = &m_tileset;

        // et on dessine enfin le tableau de vertex
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};


#define UNKNOW_CELL  1
#define VISITED_CELL 4
#define WALL_CELL    5
#define DOOR_CELL    6
#define HERO_CELL    7
#define WINGAME_CELL 3
#define HERO_UP      8
#define HERO_LEFT    9
#define HERO_DOWN    10
#define HERO_RIGHT   11


class Maze
{
private:
    uint32_t w;
    uint32_t h;

    uint32_t porte_up;
    uint32_t porte_dw;

    uint32_t hero_x;
    uint32_t hero_y;

    uint8_t* Level;
    uint8_t* visited;

public:

    Maze(const uint32_t _w, const uint32_t _h)
    {
        w = _w;
        h = _h;

        Level   = new uint8_t[w * h];
        visited = new uint8_t[w * h];
    }

    ~Maze()
    {
        delete[] Level;
        delete[] visited;
    }

    void generate()
    {
        memset(Level,   UNKNOW_CELL, w * h);
        memset(visited, UNKNOW_CELL, w * h);

        for(int y = 0; y < h; y += 2)
            for(int x = 0; x < w; x += 1)
                Level[y * w + x] = WALL_CELL;

        for(int x = 0; x < w; x += 2)
            for(int y = 0; y < h; y += 1)
                Level[y * w + x] = WALL_CELL;

        for(int x = 1; x < w-1; x += 1)
            Level[1 * w + x] = VISITED_CELL; // On fait la barre du dessus

        for(int y = 3; y < h; y += 2)
        {
            int startP = 1;
            for(int x = 1; x < w-1; x += 2)
            {
                if( (rand()%3 != 0) && (x != (w-2)) ){
                    Level[y * w + x    ] = VISITED_CELL;
                    Level[y * w + x + 1] = VISITED_CELL;
                }else{
                    Level[y * w + x    ] = VISITED_CELL;
                    const int route = (x - startP) / 2;
                    const int truc = 2 * (rand() % ((route == 0)?1:route));
                    Level[(y-1) * w + (x-truc)] = VISITED_CELL;
                    startP = x + 2;
                }
            }
        }

        porte_up                    = (2 * rand()%(w/2) + 1);
        porte_dw                    = (2 * rand()%(w/2) + 1);
        Level[            porte_up] = DOOR_CELL; // On fait la barre du dessus
        Level[(h-1) * w + porte_dw] = DOOR_CELL; // On fait la barre du dessus
        Level[(h-2) * w + porte_dw] = HERO_DOWN; // On fait la barre du dessus

        hero_x = porte_dw;
        hero_y = (h-2);
    }

    bool CanHeroMoveUp()
    {
        if( hero_y == 0 )   // Cas lorsque l'on a atteint la sortie...
            return false;   //

        return  ( Level[(hero_y-1) * w + hero_x] == VISITED_CELL )
                || ( Level[(hero_y-1) * w + hero_x] == DOOR_CELL    );
    }

    bool isGameFinished()
    {
//        printf("(DD) Is game finished (hero_y == %d) && (Level[hero_x]==%d) = %d...\n", hero_y, Level[hero_x], ((hero_y == 0) && (Level[hero_x] == DOOR_CELL)));
        return  ((hero_y == 0) && (Level[hero_x] == WINGAME_CELL));
    }

    void MoveHeroMoveUp()
    {
        Level[hero_y * w + hero_x] = VISITED_CELL;
        hero_y -= 1;
        if( Level[hero_y * w + hero_x] == DOOR_CELL ){
            Level[hero_y * w + hero_x] = WINGAME_CELL;
        }else{
            Level[hero_y * w + hero_x] = HERO_CELL;
        }
    }

    bool CanHeroMoveDown()
    {
        return ( Level[(hero_y+1) * w + hero_x] == VISITED_CELL );
    }

    void MoveHeroMoveDown()
    {
        Level[hero_y * w + hero_x] = VISITED_CELL;
        hero_y += 1;
        Level[hero_y * w + hero_x] = HERO_CELL;
    }

    bool CanHeroMoveLeft()
    {
        return ( Level[hero_y * w + (hero_x - 1)] == VISITED_CELL );
    }

    void MoveHeroLeft()
    {
        Level[hero_y * w + hero_x] = VISITED_CELL;
        hero_x -= 1;
        Level[hero_y * w + hero_x] = HERO_CELL;
    }

    bool CanHeroMoveRight()
    {
        return ( Level[hero_y * w + (hero_x + 1)] == VISITED_CELL );
    }

    void MoveHeroRight()
    {
        Level[hero_y * w + hero_x] = VISITED_CELL;
        hero_x += 1;
        Level[hero_y * w + hero_x] = HERO_CELL;
    }

    uint32_t GetHeroPosX()
    {
        return ( hero_x );
    }

    uint32_t GetHeroPosY()
    {
        return ( hero_y );
    }

    uint8_t* GetLevel()
    {
        return Level;
    }
};

class MazeSolver
{
private:
    uint32_t w;
    uint32_t h;

    uint32_t hero_x;
    uint32_t hero_y;

    uint8_t* Level;
    uint8_t* visited;

public:

    MazeSolver(const uint32_t _w, const uint32_t _h)
    {
        w = _w;
        h = _h;

        Level   = new uint8_t[w * h];
        visited = new uint8_t[w * h];
    }

    ~MazeSolver()
    {
        delete[] Level;
        delete[] visited;
    }

};

int main(int argc, char* argv[])
{
    uint32_t width     = 64 + 1;
    uint32_t height    = 32 + 1;
    uint32_t tile_size = 16;

    int c;
    while (1)
    {
        static struct option long_options[] =
        {
            {"width",   required_argument, 0, 'w'},
            {"height",  required_argument, 0, 'h'},
            {"tile",    required_argument, 0, 't'},
            {"random",  no_argument, 0,       'r'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long (argc, argv, "rw:h:t", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1) break;
        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'r':
                srand( time(NULL) );
                break;

            case 'w':
                width  = std::atoi( optarg );
                break;

            case 'h':
                height = std::atoi( optarg );
                break;

            case 't':
                tile_size = std::atoi( optarg );
                break;

            default:
                abort ();
        }
    }
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc) printf ("%s ", argv[optind++]);
        putchar ('\n');
        exit( EXIT_FAILURE );
    }

    printf("(DD) Generating a maze of size %dx%d\n", width, height);

    Maze maze(width, height);
    maze.generate();

    sf::RenderWindow window(sf::VideoMode(tile_size * width, tile_size * height), "Tilemap");

    // on crée la tilemap avec le niveau précédemment défini
    TileMap map;
    if (!map.load("tileset_16px.png", sf::Vector2u(tile_size, tile_size), maze.GetLevel(), width, height))
        return -1;

    auto t1 = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        // on gère les évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    case sf::Keyboard::Space:
                        window.close();
                        break;

                    case sf::Keyboard::Return:
                        window.close();
                        break;

                    case sf::Keyboard::G:
                        maze.generate();
                        break;

                    case sf::Keyboard::Up:
                        if( maze.CanHeroMoveUp() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), VISITED_CELL);
                            maze.MoveHeroMoveUp();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), HERO_UP);
                        }
                        break;

                    case sf::Keyboard::Down:
                        if( maze.CanHeroMoveDown() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), VISITED_CELL);
                            maze.MoveHeroMoveDown();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), HERO_DOWN);
                        }
                        break;

                    case sf::Keyboard::Left:
                        if( maze.CanHeroMoveLeft() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), VISITED_CELL);
                            maze.MoveHeroLeft();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), HERO_LEFT);
                        }
                        break;

                    case sf::Keyboard::Right:
                        if( maze.CanHeroMoveRight() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), VISITED_CELL);
                            maze.MoveHeroRight();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), HERO_RIGHT);
                        }
                        break;

                    default:
                        break;
                }
            }
        }

        // on dessine le niveau
        window.clear();
        window.draw(map);
        window.display();

        if( maze.isGameFinished() == true )
        {
            auto t2 = std::chrono::steady_clock::now();
            auto d_milli = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() / 1000.0f;
            window.setTitle( "Your time is " + std::to_string(d_milli) + " secondes" );

            while (window.pollEvent(event));

            sf::sleep(sf::milliseconds(2000));
            uint8_t* lvl = maze.GetLevel();
            maze.generate();
            for(int y = 0; y < height; y += 1)
            {
                for(int x = 0; x < width; x += 1)
                {
                    map.setTile(x, y, lvl[y * width + x]);
                }
            }

            t1 = std::chrono::steady_clock::now();
        }

        sf::sleep(sf::milliseconds(100));

    }

    return 0;
}
