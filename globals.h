#ifndef GLOBALS_H
#define GLOBALS_H
#include "gamesprites.h"

Ship* my_ship;
std::list<Sprite> exp_group;

/** Image Infos for images used in the game */
ImageInfo missile_info = ImageInfo(Point2D(16, 16), Point2D(32, 32), 8);
ImageInfo ship_info = ImageInfo(Point2D(65 / 2, 65 / 2), Point2D(65, 65), 23);
ImageInfo exp_info = ImageInfo(Point2D(65 / 2, 65 / 2), Point2D(65, 65), 0);
ImageInfo enemy_info = ImageInfo(Point2D(65 / 2, 65 / 2), Point2D(65, 65), 25);
ImageInfo shooter_info = ImageInfo(Point2D(98 / 2, 98 / 2), Point2D(98, 98), 40);
ImageInfo turret_info = ImageInfo(Point2D(65 / 2, 65 / 2), Point2D(65, 65), 0);
ImageInfo bar_info = ImageInfo(Point2D(81 / 2, 240), Point2D(81, 480), 0);
ImageInfo barp_info = ImageInfo(Point2D(16, 16), Point2D(32, 32), 0);
ImageInfo pwup_info = ImageInfo(Point2D(16, 16), Point2D(32, 32), 14);
ImageInfo aura_info = ImageInfo(Point2D(45, 45), Point2D(90, 90), 0);
ImageInfo seeker_info = ImageInfo(Point2D(16, 16), Point2D(32, 32), 13);
ImageInfo boss1_info = ImageInfo(Point2D(49, 49), Point2D(98, 98), 45);
ImageInfo danger = ImageInfo(Point2D(369 / 2, 134 / 2), Point2D(369, 134), 0);

/** Surfaces for images used in the game */
SDL_Surface* exp_image = NULL;
SDL_Surface* enemy_image = NULL;
SDL_Surface* shooter_image = NULL;
SDL_Surface* ship_image = NULL;
SDL_Surface* missile_image = NULL;
SDL_Surface* turret_image = NULL;
SDL_Surface* bar_image = NULL;
SDL_Surface* barp_image = NULL;
SDL_Surface* pwup_image = NULL;
SDL_Surface* rapid_image = NULL;
SDL_Surface* pwaura_image = NULL;
SDL_Surface* aura_image = NULL;
SDL_Surface* clouds = NULL;
SDL_Surface* lives_image = NULL;
SDL_Surface* seeker_image = NULL;
SDL_Surface* boss1_image = NULL;
SDL_Surface* danger_image = NULL;
SDL_Surface* paused_image = NULL;
SDL_Surface* pause_overlay = NULL;

/** Sound used in the game */
Mix_Chunk *shot_sound;
Mix_Chunk *explosion;
Mix_Music *music = NULL;

SDL_Event event;

/** Pointer to the screen's surface */
SDL_Surface* screen;

// Max enemies per screen
const unsigned int MAX_ENEMIES = 7;
const int K_VEL = 10;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 24;
const int FRAMES_PER_SECOND = 60;
const double MISSILE_ABS_VEL = 20.0;
const int SEEKER_MISSILE_LIFE = 30;
const int SEEKER_MISSILE_VEL = 4;


int MAX_OFFSET = SCREEN_WIDTH - 105;
int DELAY = 10;

// Player's lives
int lives = 3;

// Fps using the framerate class
framerate fpst;

#endif // GLOBALS_H
