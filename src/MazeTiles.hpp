//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef MazeTiles
#define MazeTiles

#define T_INITIAL      0
#define T_BUG          0
#define T_CHEMIN       1
#define T_OBSTACLE     2
#define T_CADRE        3

#define T_HERO_D       4   // DROITE
#define T_HERO_B       5   // BAS
#define T_HERO_G       6   // GAUCHE
#define T_HERO_H       7   // HAUT

#define T_MAZE_START   8
#define T_MAZE_STOP    9

#define T_SOLUCE      10   // PTR DE DEBUT
#define T_SOLUCE_D    10   // DROITE
#define T_SOLUCE_B    11   // BAS
#define T_SOLUCE_G    12   // GAUCHE
#define T_SOLUCE_H    13   // HAUT

#define T_VISITE      14
#define T_UNDEF       15

#define T_1x_RIGHT    16   // HAUT
#define T_1x_DOWN     17   // HAUT
#define T_1x_LEFT     18   // HAUT
#define T_1x_UP       19   // HAUT

#define T_2x_LR       20   // HAUT
#define T_2x_UD       21   // HAUT

#define T_2x_RD       22   // HAUT
#define T_2x_DL       23   // HAUT
#define T_2x_LU       24   // HAUT
#define T_2x_UR       25   // HAUT

#define T_3x_LEFT     26   // BLOQUE A GAUCHE
#define T_3x_UP       27   // BLOQUE EN HAUT
#define T_3x_RIGHT    28   // BLOQUE A DROITE
#define T_3x_DOWN     29   // BLOQUE EN BAS

#define T_4x          30   // HAUT

#endif //MazeTiles
