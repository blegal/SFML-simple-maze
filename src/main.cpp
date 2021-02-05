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

#include "TileMap.hpp"
#include "Maze.hpp"
#include "MazeSolver.hpp"

#include "./TileAnalizers/Analyzer.hpp"
#include "./TileAnalizers/SimpleAnalyzer.hpp"
#include "./TileAnalizers/PathAnalyzer.hpp"

int main(int argc, char* argv[])
{

    uint32_t width       = 64 + 1;
    uint32_t height      = 32 + 1;
    uint32_t tile_size   = 16;
    bool     texture     = false;
    std::string filename = "../sprites/tileset_zelda_nes_16x16.png";


    int c;
    while (1)
    {
        static struct option long_options[] =
        {
            {"width",   required_argument, 0, 'w'},
            {"height",  required_argument, 0, 'h'},
            {"tile",    required_argument, 0, 't'},
            {"file",    required_argument, 0, 'f'},
            {"texture", no_argument, 0,       'T'},
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
                if( width%2 != 1 )
                {
                    printf("(EE) Error the width of the Maze is not odd.\n");
                    exit( EXIT_FAILURE );
                }
                break;

            case 'h':
                height = std::atoi( optarg );
                if( height%2 != 1 )
                {
                    printf("(EE) Error the height of the Maze is not odd.\n");
                    exit( EXIT_FAILURE );
                }
                break;

            case 't':
                tile_size = std::atoi( optarg );
                break;

            case 'T':
                texture = true;
                break;

            case 'f':
                filename = optarg ;
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

    sf::Texture m_tileset;
    if ( !m_tileset.loadFromFile( filename ) )
    {
        printf("(EE) An error happen during the pixelset reading...\n");
        return EXIT_FAILURE;
    }
    tile_size = m_tileset.getSize().x;


    Maze maze(width, height);
    maze.generate_2();

    Analyzer* tile_analyzer;
    if( texture == true)
    {
        tile_analyzer = new PathAnalyzer(maze);
    }else{
        tile_analyzer = new SimpleAnalyzer(maze);
    }

    MazeSolver ms(maze);

    sf::RenderWindow window(sf::VideoMode(tile_size * width, tile_size * height), "Tilemap");

    // on crée la tilemap avec le niveau précédemment défini
    TileMap map;
    if (!map.load(
            filename,
            sf::Vector2u(tile_size, tile_size),
            tile_analyzer->GetSprites(),
            width,
            height)
       )
    {
        return -1;
    }

    //
    // On affiche le héra sur la map
    //
    map.setTile(
            maze.GetHeroPosX(),
            maze.GetHeroPosY(),
            T_HERO_D);

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

                    case sf::Keyboard::Q:
                        window.close();
                        break;

                    case sf::Keyboard::Space:
                        window.close();
                        break;

                    case sf::Keyboard::Return:
                        window.close();
                        break;

//                    case sf::Keyboard::G:
//                        maze.generate();
//                        break;

                    case sf::Keyboard::Up:
                        if( maze.CanHeroMoveUp() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_VISITE);
                            maze.MoveHeroUp();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_HERO_H);
                        }
                        break;

                    case sf::Keyboard::Down:
                        if( maze.CanHeroMoveDown() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_VISITE);
                            maze.MoveHeroDown();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_HERO_B);
                        }
                        break;

                    case sf::Keyboard::Left:
                        if( maze.CanHeroMoveLeft() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_VISITE);
                            maze.MoveHeroLeft();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_HERO_G);
                        }
                        break;

                    case sf::Keyboard::Right:
                        if( maze.CanHeroMoveRight() )
                        {
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_VISITE);
                            maze.MoveHeroRight();
                            map.setTile(maze.GetHeroPosX(), maze.GetHeroPosY(), T_HERO_D);
                        }
                        break;

                    case sf::Keyboard::G:
                        maze.generate();
                        for(int y = 0; y < height; y += 1)
                            for(int x = 0; x < width; x += 1)
                                map.setTile(x, y, maze.GetLevel()[y * width + x]);
                        break;

                    case sf::Keyboard::S:
                        ms.Update( maze );
                        for(int p = 0; p < ms.cx.size(); p += 1)
                            map.setTile(ms.cx[p], ms.cy[p], ms.fleche[p]);
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
