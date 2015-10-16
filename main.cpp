#include "gamesprites.h"
#include "timer.h"
#include <cstdlib>
#include <ctime>

#include <sstream>
#include <fstream>

#define MAXMAP 1000
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 24
#define FRAMES_PER_SECOND 100

using namespace std;

extern SDL_Surface* screen;

extern ImageInfo ship_info;
extern ImageInfo missile_info;
extern ImageInfo exp_info;
extern ImageInfo turret_info;
extern ImageInfo bar_info;
extern ImageInfo barp_info;
extern ImageInfo pwup_info;
extern ImageInfo aura_info;
extern ImageInfo seeker_info;
extern ImageInfo boss1_info;
extern ImageInfo danger;

extern SDL_Surface* ship_image;
extern SDL_Surface* missile_image;
extern SDL_Surface* enemy_image;
extern SDL_Surface* shooter_image;
extern SDL_Surface* exp_image;
extern SDL_Surface* turret_image;
extern SDL_Surface* bar_image;
extern SDL_Surface* barp_image;
extern SDL_Surface* pwup_image;
extern SDL_Surface* rapid_image;
extern SDL_Surface* pwaura_image;
extern SDL_Surface* aura_image;
extern SDL_Surface* clouds;
extern SDL_Surface* lives_image;
extern SDL_Surface* seeker_image;
extern SDL_Surface* boss1_image;
extern SDL_Surface* danger_image;
extern SDL_Surface* paused_image;
extern SDL_Surface* pause_overlay;

extern Ship* my_ship;
extern std::list<Sprite> exp_group;

extern Mix_Chunk* shot_sound;
extern Mix_Chunk *explosion;
extern Mix_Music* music;
extern framerate fpst;

extern int lives;
extern int DELAY;
extern int MAX_OFFSET;

char mapa[MAXMAP + 1];
int vely = 0;
FILE* f;

//The dot dimensions
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

//The dimensions of the level
const int LEVEL_WIDTH = 640;
const int LEVEL_HEIGHT = 6400;

//Tile constants
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = 1000;
const int TILE_SPRITES = 21;

//The different tile sprites
const int TILE_00 = 0;
const int TILE_01 = 1;
const int TILE_02 = 2;
const int TILE_10 = 3;
const int TILE_11 = 4;
const int TILE_12 = 5;
const int TILE_20 = 6;
const int TILE_21 = 7;
const int TILE_22 = 8;
const int TILE_30 = 9;
const int TILE_31 = 10;
const int TILE_32 = 11;
const int TILE_40 = 12;
const int TILE_41 = 13;
const int TILE_42 = 14;
const int TILE_50 = 15;
const int TILE_51 = 16;
const int TILE_52 = 17;
const int TILE_60 = 18;
const int TILE_61 = 19;
const int TILE_62 = 20;


//The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *tileSheet = NULL;

//Sprite from the tile sheet
SDL_Rect clips[ TILE_SPRITES ];

//The camera
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//The tile
class Tile
{
    private:
    //The attributes of the tile
    SDL_Rect box;

    //The tile type
    int type;

    public:
    //Initializes the variables
    Tile( int x, int y, int tileType );

    //Shows the tile
    void show();

    //Get the tile type
    int get_type();

    //Get the collision box
    SDL_Rect get_box();
};

//The dot
class Dot
{
    private:
    //The dot's collision box
    SDL_Rect box;

    //The velocity of the dot
    int xVel, yVel;

    public:
    //Initializes the variables
    Dot();

    void setYVel( int vel) { yVel = vel; }
    //Takes key presses and adjusts the dot's velocity
    void handle_input();

    //Moves the dot
    void move( Tile *tiles[] );

    //Shows the dot on the screen
    void show();

    //Sets the camera over the dot
    void set_camera();
};

bool check_collision( SDL_Rect A, SDL_Rect B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Move the Dot", NULL );

    //If everything initialized fine
    return true;
}

void clip_tiles()
{
    //Clip the sprite sheet
    clips[ TILE_00 ].x = 0;
    clips[ TILE_00 ].y = 0;
    clips[ TILE_00 ].w = TILE_WIDTH;
    clips[ TILE_00 ].h = TILE_HEIGHT;

    clips[ TILE_01 ].x = 64;
    clips[ TILE_01 ].y = 0;
    clips[ TILE_01 ].w = TILE_WIDTH;
    clips[ TILE_01 ].h = TILE_HEIGHT;

    clips[ TILE_02 ].x = 128;
    clips[ TILE_02 ].y = 0;
    clips[ TILE_02 ].w = TILE_WIDTH;
    clips[ TILE_02 ].h = TILE_HEIGHT;

    clips[ TILE_10 ].x = 0;
    clips[ TILE_10 ].y = 64;
    clips[ TILE_10 ].w = TILE_WIDTH;
    clips[ TILE_10 ].h = TILE_HEIGHT;

    clips[ TILE_11 ].x = 64;
    clips[ TILE_11 ].y = 64;
    clips[ TILE_11 ].w = TILE_WIDTH;
    clips[ TILE_11 ].h = TILE_HEIGHT;

    clips[ TILE_12 ].x = 128;
    clips[ TILE_12 ].y = 64;
    clips[ TILE_12 ].w = TILE_WIDTH;
    clips[ TILE_12 ].h = TILE_HEIGHT;

    clips[ TILE_20 ].x = 0;
    clips[ TILE_20 ].y = 128;
    clips[ TILE_20 ].w = TILE_WIDTH;
    clips[ TILE_20 ].h = TILE_HEIGHT;

    clips[ TILE_21 ].x = 64;
    clips[ TILE_21 ].y = 128;
    clips[ TILE_21 ].w = TILE_WIDTH;
    clips[ TILE_21 ].h = TILE_HEIGHT;

    clips[ TILE_22 ].x = 128;
    clips[ TILE_22 ].y = 128;
    clips[ TILE_22 ].w = TILE_WIDTH;
    clips[ TILE_22 ].h = TILE_HEIGHT;

    clips[ TILE_30 ].x = 0;
    clips[ TILE_30 ].y = 192;
    clips[ TILE_30 ].w = TILE_WIDTH;
    clips[ TILE_30 ].h = TILE_HEIGHT;

    clips[ TILE_31 ].x = 64;
    clips[ TILE_31 ].y = 192;
    clips[ TILE_31 ].w = TILE_WIDTH;
    clips[ TILE_31 ].h = TILE_HEIGHT;

    clips[ TILE_32 ].x = 128;
    clips[ TILE_32 ].y = 192;
    clips[ TILE_32 ].w = TILE_WIDTH;
    clips[ TILE_32 ].h = TILE_HEIGHT;

    clips[ TILE_40 ].x = 0;
    clips[ TILE_40 ].y = 256;
    clips[ TILE_40 ].w = TILE_WIDTH;
    clips[ TILE_40 ].h = TILE_HEIGHT;

    clips[ TILE_41 ].x = 64;
    clips[ TILE_41 ].y = 256;
    clips[ TILE_41 ].w = TILE_WIDTH;
    clips[ TILE_41 ].h = TILE_HEIGHT;

    clips[ TILE_42 ].x = 128;
    clips[ TILE_42 ].y = 256;
    clips[ TILE_42 ].w = TILE_WIDTH;
    clips[ TILE_42 ].h = TILE_HEIGHT;

    clips[ TILE_50 ].x = 0;
    clips[ TILE_50 ].y = 320;
    clips[ TILE_50 ].w = TILE_WIDTH;
    clips[ TILE_50 ].h = TILE_HEIGHT;

    clips[ TILE_51 ].x = 64;
    clips[ TILE_51 ].y = 320;
    clips[ TILE_51 ].w = TILE_WIDTH;
    clips[ TILE_51 ].h = TILE_HEIGHT;

    clips[ TILE_52 ].x = 128;
    clips[ TILE_52 ].y = 320;
    clips[ TILE_52 ].w = TILE_WIDTH;
    clips[ TILE_52 ].h = TILE_HEIGHT;

    clips[ TILE_60 ].x = 0;
    clips[ TILE_60 ].y = 384;
    clips[ TILE_60 ].w = TILE_WIDTH;
    clips[ TILE_60 ].h = TILE_HEIGHT;

    clips[ TILE_61 ].x = 64;
    clips[ TILE_61 ].y = 384;
    clips[ TILE_61 ].w = TILE_WIDTH;
    clips[ TILE_61 ].h = TILE_HEIGHT;

    clips[ TILE_62 ].x = 128;
    clips[ TILE_62 ].y = 384;
    clips[ TILE_62 ].w = TILE_WIDTH;
    clips[ TILE_62 ].h = TILE_HEIGHT;

}

bool set_tiles( Tile *tiles[] )
{
    //The tile offsets
    int x = 0, y = 0;
    FILE* f;
    if( (f = fopen("maps/level1.map", "r") ) != NULL)
    {
        fread(mapa, MAXMAP, 1, f);
        fclose(f);
    }
    else
    {
        return false;
    }

    //Initialize the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        //Determines what kind of tile will be made
        int tileType = -1;

        tileType = mapa[t];
        //If the number is a valid tile number
        if( ( tileType >= 0 ) && ( tileType < TILE_SPRITES ) )
        {
            tiles[ t ] = new Tile( x, y - LEVEL_HEIGHT + SCREEN_HEIGHT, tileType );
        }
        //If we don't recognize the tile type
        else
        {
            return false;
        }

        //Move to next tile spot
        x += TILE_WIDTH;

        //If we've gone too far
        if( x >= LEVEL_WIDTH )
        {
            //Move back
            x = 0;

            //Move to the next row
            y += TILE_HEIGHT;
        }
    }

    fclose(f);
    //If the map was loaded fine
    return true;
}

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    box.x = x;
    box.y = y;

    //Set the collision box
    box.w = TILE_WIDTH;
    box.h = TILE_HEIGHT;

    //Get the tile type
    type = tileType;
}

void Tile::show()
{
    //If the tile is on screen
    if( check_collision( camera, box ) == true )
    {
        //Show the tile
        apply_surface( box.x - camera.x, box.y - camera.y, tileSheet, screen, &clips[ type ] );
    }
}

int Tile::get_type()
{
    return type;
}

SDL_Rect Tile::get_box()
{
    return box;
}

Dot::Dot()
{
    //Initialize the offsets
    box.x = 0;
    box.y = 0;
    box.w = DOT_WIDTH;
    box.h = DOT_HEIGHT;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void Dot::move( Tile *tiles[] )
{
    //Move the dot up or down
    box.y += yVel;

    //If the dot went too far up or down or touched a wall
    if( ( box.y < 0 ) || ( box.y + DOT_HEIGHT > LEVEL_HEIGHT ) )
    {
        //move back
        box.y -= yVel;
    }
}

void Dot::set_camera()
{
    //Center the camera over the dot
    camera.x = ( box.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
    camera.y = ( box.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds.
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > LEVEL_WIDTH - camera.w )
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if( camera.y > LEVEL_HEIGHT - camera.h )
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}


int main(int argc, char* argv[])
{
    SDL_Event event;
    Uint8* keys;

    //The tiles that will be used
    Tile *tiles[ TOTAL_TILES ];

    //The dot
    Dot myDot;

    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;
    }

    if (TTF_Init() == -1)
    {
        return 1;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE);
    SDL_WM_SetCaption( "Naves", NULL );
    bool done = false;

    SDL_Color transp = {120, 195, 128};
    SDL_Color black = {0, 0, 0};

    //Clip the tile sheet
    clip_tiles();

    //Set the tiles
    if( set_tiles( tiles ) == false )
    {
        return 1;
    }

    /** Loading game's images */
    ship_image = load_image("pics/triple_ship.png", false, &transp);
    missile_image = load_image("pics/bullet.png", false, &transp);
    enemy_image = load_image("pics/enemy1.png", false, &transp);
    exp_image = load_image("pics/kaboom.png", false, &black);
    turret_image = load_image("pics/player_turret.png", false, &transp);
    shooter_image = load_image("pics/shooter_img.png", false, &transp);
    lives_image = load_image("pics/life_sp.png", false, &transp);
    clouds = load_image("pics/clouds.png", true);
    bar_image = load_image("pics/hud.png", false, &transp);
    barp_image = load_image("pics/bar_pointer.png", false, &transp);
    pwup_image = load_image("pics/pwup_tri.png", false, &transp);
    aura_image = load_image("pics/aura.png", true);
    rapid_image = load_image("pics/pwup_rapid.png", false, &transp);
    pwaura_image = load_image("pics/pwup_aura.png", false, &transp);
    seeker_image = load_image("pics/misil.png", false, &transp);
    boss1_image = load_image("pics/boss1.png", false, &transp);
    danger_image = load_image("pics/danger.png", true);
    paused_image = load_image("pics/paused.png", true);
    pause_overlay = load_image("pics/pause_overlay.png", true);

    tileSheet = load_image( "pics/tiles.png" );

    /** Loading Sounds */
    shot_sound = Mix_LoadWAV( "sounds/shoot.ogg" );
    explosion = Mix_LoadWAV("sounds/explosion.ogg");
    music = Mix_LoadMUS( "music/asdx.ogg" );

    ImageInfo lives_info = ImageInfo(Point2D(16, 16), Point2D(32, 32), 10);

    /** Player's ship */
    my_ship = new Ship(Point2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), Point2D(0, 0), SHIP_CENTER, ship_image, ship_info);
    Sprite lives_sp = Sprite(Point2D(SCREEN_WIDTH - 50, 20), Point2D(0, 0), lives_image, lives_info);
    Sprite turret = Sprite(my_ship->getPos(), Point2D(0, 0), turret_image, turret_info, true);
    Sprite bar = Sprite(Point2D(SCREEN_WIDTH - bar_image->w / 2, SCREEN_HEIGHT / 2), Point2D(0, 0), bar_image, bar_info);
    Sprite barp = Sprite(Point2D(SCREEN_WIDTH - bar_image->w / 2 + 3, (SCREEN_HEIGHT + bar_image->h - 63) / 2 ),
                         Point2D(0, -5), barp_image, barp_info);

    Sprite *aura = NULL;

    SDL_Color color = {255, 201, 14};
    SDL_Color s_color = {255, 255, 255};

    /**
     * Main Loop
    */

    bool pressed = false;
    bool esc_pressed = false;
    Timer delta;

    int time_p = 0;
    char score_s[10];
    char life_s[10];
    char ammo[10];
    int score = 0;
    bool paused = false;

    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        if( Mix_PlayMusic( music, -1 ) == -1 )
        {
            return 1;
        }
    }

    delta.start();
    Timer fps;
    fpst.Init(50);

    std::list<Sprite*> enemy_group;
    std::list<Sprite*> missile_group;
    std::list<Sprite*> bullets;
    std::list<Sprite*> seekerMissiles;

    int shoot_time = 0;

    //The offsets of the background
    int bgX = 0, bgY = 0;

    srand( time(NULL) );
    Buff* buff = NULL;
    Sprite* buff_icon = NULL;
    int wait = 0, wait_boss = 0;

    int enemies = 300;
    int wait_for_boss = 0;
    bool boss_active = false;
    MissileShip* boss1  = NULL;

    while (!done)
    {
        fps.start();

        keys = SDL_GetKeyState( NULL );
        if (!paused){
            if (camera.y > -(LEVEL_HEIGHT - camera.h) )
                camera.y -= 2 * fpst.speedfactor;
            else
                camera.y = 0;

            //Show the tiles
            for( int t = 0; t < TOTAL_TILES; t++ )
            {
                tiles[ t ]->show();
            }
            sprintf(score_s, "%d", score);

            //Show the background
            apply_surface( bgX, bgY, clouds, screen );
            apply_surface( bgX , bgY - clouds->h, clouds, screen );
            my_ship->update();
            turret.setPos(my_ship->getPos());
            turret.update( bullets );

            if (time_p % 1000 == 0 && time_p != 0 && buff == NULL)
            {
                int type = rand() % 3;
                switch(type)
                {
                case 0:
                    buff = new Buff(Point2D(rand() % MAX_OFFSET, -10), Point2D(0, 2), pwup_image, pwup_info, true, 2);
                    break;
                case 1:
                    buff = new Buff(Point2D(rand() % MAX_OFFSET, -10), Point2D(0, 2), rapid_image, pwup_info, true, 2);
                    break;
                default:
                    buff = new Buff(Point2D(rand() % MAX_OFFSET, -10), Point2D(0, 2), pwaura_image, pwup_info, true, 2);
                }
                buff->setType(type);
            }


            if (buff)
            {
                buff->update(missile_group);
                buff->draw(screen);

                if ( wait % 10 == 0 )
                    buff->setNextFrame();

                if (buff->getCurFrame() >= buff->getFrames())
                {
                    buff->setCurFrame( 0 );
                }

                if (buff->collide(*my_ship))
                {
                    if (buff_icon != NULL && buff->getType() != 2)
                    {
                        delete buff_icon;
                        buff_icon = NULL;
                    }

                    if (buff->getType() == 2)
                    {
                        my_ship->setAura( true );
                        aura = new Sprite(my_ship->getPos(), Point2D(0, 0), aura_image, aura_info);
                    }
                    else
                    {
                        buff_icon = new Sprite(Point2D(SCREEN_WIDTH - 20, 60), Point2D(0, 0), buff->getImg(), pwup_info, true, 2);
                        switch(buff->getType())
                        {
                            case 0:
                                my_ship->setBuff( 1 );
                                my_ship->setAmmo( my_ship->getAmmo() + 30 );
                                my_ship->setDelay( DELAY );
                                break;
                            case 1:
                                my_ship->setBuff( 2 );
                                my_ship->setAmmo( my_ship->getAmmo() + 30 );
                                my_ship->setDelay( 5 );
                                break;
                        }
                    }

                    delete buff;
                    buff = NULL;
                }
                else if (out_of_screen(buff->getPos()))
                {
                    delete buff;
                    buff = NULL;
                }
                wait++;
            }


            if (boss1)
            {
                boss1->update(seekerMissiles);
                boss1->draw(screen);
                if (boss1->can_get_damaged)
                {
                    if (group_collide(missile_group, *boss1))
                    {
                        boss1->setCurFrame(1);
                        wait_boss = 0;
                        boss1->setLife(boss1->getLife() - 1);
                    }
                    if (boss1->getCurFrame() == 1)
                    {
                        wait_boss++;
                    }
                    if (wait_boss % 7 == 0 && wait_boss != 0)
                    {
                        boss1->setCurFrame(0);
                    }
                    if (boss1->getLife() <= 0)
                    {
                        delete boss1;
                        boss1 = NULL;
                    }
                }

            }
            if (time_p % 100 == 0 && time_p > 0 && barp.getPos().getY() >= 110)
                barp.update( bullets );

            for (list<Sprite>::iterator it = exp_group.begin(), end = exp_group.end(); it != end; ++it) {
                it->draw(screen);
                it->setNextFrame();
                if (it->getCurFrame() >= it->getFrames())
                {
                    it = exp_group.erase(it);
                }
            }

            for (list<Sprite*>::iterator it = enemy_group.begin(), end = enemy_group.end(); it != end; ++it) {
                (*it)->draw(screen);
                (*it)->update( bullets );
                if (out_of_screen((*it)->getPos()))
                {
                    delete *it;
                    it = enemy_group.erase(it);
                }
            }
            my_ship->draw(screen);
            turret.draw(screen);
            if ( aura != NULL)
            {
                aura->setPos(my_ship->getPos());
                aura->draw(screen);
            }

            score += group_group_collide(missile_group, enemy_group);
            score += group_group_collide(missile_group, seekerMissiles);

            if (keys[SDLK_w]) { my_ship->setState(SHIP_CENTER); my_ship->changeVelY(-5); my_ship->setMovement(true); }
            if (keys[SDLK_s]) { my_ship->setState(SHIP_CENTER); my_ship->changeVelY(5); my_ship->setMovement(true); }
            if (keys[SDLK_a]) { my_ship->setState(SHIP_LEFT); my_ship->changeVelX(-5); my_ship->setMovement(true); }
            if (keys[SDLK_d]) { my_ship->setState(SHIP_RIGHT); my_ship->changeVelX(5); my_ship->setMovement(true); }

            if (keys[SDLK_UP]) { turret.setFrame( 0 ); }
            if (keys[SDLK_DOWN]) { turret.setFrame( 2 ); }
            if (keys[SDLK_LEFT]) { turret.setFrame( 1 ); }
            if (keys[SDLK_RIGHT]) { turret.setFrame( 3 ); }

            if (keys[SDLK_UP] && keys[SDLK_LEFT]) { turret.setFrame( 4 ); }
            if (keys[SDLK_DOWN] && keys[SDLK_LEFT]) { turret.setFrame( 5 ); }
            if (keys[SDLK_DOWN] && keys[SDLK_RIGHT]) { turret.setFrame( 6 ); }
            if (keys[SDLK_UP] && keys[SDLK_RIGHT]) { turret.setFrame( 7 ); }

            if (!keys[SDLK_d] && !keys[SDLK_a]) { my_ship->setMovement(false); }

            if (keys[SDLK_UP] || keys[SDLK_DOWN] || keys[SDLK_LEFT] || keys[SDLK_RIGHT])
            {
                if (!pressed)
                {
                    my_ship->shot(turret, missile_group);
                    if( Mix_PlayChannel( -1, shot_sound, 0 ) == -1 )
                    {
                        //return 1;
                    }
                }
                else if (shoot_time % my_ship->getDelay() == 0 && shoot_time > 0)
                {
                    my_ship->shot(turret, missile_group);
                    if( Mix_PlayChannel( -1, shot_sound, 0 ) == -1 )
                    {
                        //return 1;
                    }
                }
                pressed = true;
                shoot_time++;
            }

            for (list<Sprite*>::iterator it = missile_group.begin(), end = missile_group.end(); it != end; ++it) {
                (*it)->draw(screen);
                (*it)->update( bullets );
                if (out_of_screen((*it)->getPos()))
                {
                    delete *it;
                    it = missile_group.erase(it);
                }
            }

            for (std::list<Sprite*>::iterator it = bullets.begin(), end = bullets.end(); it != end; ++it)
            {
                (*it)->draw(screen);
                (*it)->update( bullets );
                if (out_of_screen((*it)->getPos()))
                {
                    delete *it;
                    it = bullets.erase(it);
                }
            }

            for (std::list<Sprite*>::iterator it = seekerMissiles.begin(), end = seekerMissiles.end(); it != end; ++it)
            {
                (*it)->draw(screen);
                (*it)->update( bullets );
                if (out_of_screen((*it)->getPos()))
                {
                    delete *it;
                    it = bullets.erase(it);
                }
            }

            //Scroll background
            bgY += 3 * fpst.speedfactor;

            //If the background has gone too far
            if( bgY >= clouds->h )
            {
                //Reset the offset
                bgY = 0;
            }

            bar.draw(screen);
            barp.draw(screen);

            if (buff_icon != NULL)
            {
                buff_icon->draw(screen);
                sprintf(ammo, "%d", my_ship->getAmmo());
                draw_text(screen, ammo, Point2D(SCREEN_WIDTH - 60, 50), 15, s_color, "ariblk.ttf");
                if (my_ship->getAmmo() <= 0)
                {
                    my_ship->setAmmo( 0 );
                    delete buff_icon;
                    buff_icon = NULL;
                }
            }


            sprintf(life_s, "x %d", lives);
            lives_sp.draw(screen);

            draw_text(screen, life_s, Point2D(SCREEN_WIDTH - 40, 5), 18, color, "ariblk.ttf");
            draw_text(screen, score_s, Point2D(SCREEN_WIDTH - 64, SCREEN_HEIGHT - 22.5), 13, s_color, "ariblk.ttf");

            if (group_collide(bullets, *my_ship) || group_collide(enemy_group, *my_ship) ||
                group_collide(seekerMissiles, *my_ship) || (boss1 && boss1->collide(*my_ship)))
            {
                if ( aura == NULL)
                {
                    Sprite exp = Sprite(my_ship->getPos(), Point2D(0, 0), exp_image, exp_info, true, 7);
                    exp_group.push_back(exp);
                    my_ship->setPos(Point2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
                    lives--;
                    my_ship->setDelay( DELAY );
                    if (my_ship->getBuff())
                    {
                        my_ship->setBuff(0);
                        my_ship->setAmmo( 0 );
                        delete buff_icon;
                        buff_icon = NULL;
                    }
                }
                else
                {
                    my_ship->setAura(false);
                    delete aura;
                    aura = NULL;
                }

            }

            if (!keys[SDLK_UP] && !keys[SDLK_DOWN] && !keys[SDLK_LEFT] && !keys[SDLK_RIGHT]) { pressed = false; shoot_time = 0; }

            if (time_p % enemies == 0 && time_p > 0 && !boss_active )
                enemy_spawner(enemy_group);

            if (time_p % 100 == 0 && time_p > 0 && enemies > 30)
            {
                enemies -= 10;
            }

            if (barp.getPos().getY() <= 120 && !boss_active) { boss_active = true; }

            if ( boss_active && wait_for_boss < 200)
            {
                draw_image(screen, danger_image, danger.getCenter(), danger.getSize(),
                           Point2D( MAX_OFFSET / 2 + 20, SCREEN_HEIGHT / 2), danger.getSize());
                wait_for_boss++;
                if (wait_for_boss >= 200)
                {
                    boss1 = new MissileShip(Point2D(SCREEN_WIDTH / 2, -5), Point2D(5, 0), boss1_image, boss1_info, true, 3);
                    boss1->setCurFrame(2);
                }
            }
            time_p++;

        }
        else
        {
            apply_surface(0, 0, pause_overlay, screen);
            draw_image(screen, paused_image, danger.getCenter(), danger.getSize(),
                           Point2D( MAX_OFFSET / 2 + 20, SCREEN_HEIGHT / 2), danger.getSize());
        }

        if (keys[SDLK_ESCAPE] && !esc_pressed)
        {
            //If there is no music playing
            if( Mix_PlayingMusic() == 0 )
            {
                //Play the music
                if( Mix_PlayMusic( music, -1 ) == -1 )
                {
                    return 1;
                }
            }
            else if( Mix_PausedMusic() == 1 )
            {
                //Resume the music
                Mix_ResumeMusic();
            }
            else
            {
                //Pause the music
                Mix_PauseMusic();
            }
            paused = !paused; esc_pressed = true;
        }
        if (!keys[SDLK_ESCAPE]) { esc_pressed = false; }

        if (lives <= 0)
        {
            draw_text(screen, "GAME OVER", Point2D((SCREEN_WIDTH - 240) / 2, SCREEN_HEIGHT / 2 - 40), 40, color, "ariblk.ttf");
            SDL_Flip(screen);
            SDL_Delay( 2000 );
            done = true;
        }
        while (SDL_PollEvent( &event ))
        {
            if ( event.type == SDL_QUIT )
            {
                done = true;
            }
        }

        if (SDL_Flip(screen) == -1)
        {
            return -1;
        }

        fpst.SetSpeedFactor();
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }


    }

    delete my_ship;
    if ( aura != NULL)
        delete aura;

    for (std::list<Sprite*>::iterator it = enemy_group.begin(), end = enemy_group.end(); it != end; ++it)
    {
        delete *it;
        it = enemy_group.erase(it);
    }
    enemy_group.clear();

    for (std::list<Sprite*>::iterator it = missile_group.begin(), end = missile_group.end(); it != end; ++it)
    {
        delete *it;
        it = missile_group.erase(it);
    }
    missile_group.clear();

    for (std::list<Sprite*>::iterator it = bullets.begin(), end = bullets.end(); it != end; ++it)
    {
        delete *it;
        it = bullets.erase(it);
    }
    bullets.clear();

    exp_group.clear();

    //Free the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        delete tiles[ t ];
    }

    SDL_FreeSurface( ship_image );
    SDL_FreeSurface( missile_image );
    SDL_FreeSurface( enemy_image );
    SDL_FreeSurface( shooter_image );
    SDL_FreeSurface( exp_image );
    SDL_FreeSurface( turret_image );
    SDL_FreeSurface( bar_image );
    SDL_FreeSurface( barp_image );
    SDL_FreeSurface( pwup_image );
    SDL_FreeSurface( rapid_image );
    SDL_FreeSurface( pwaura_image );
    SDL_FreeSurface( aura_image );
    SDL_FreeSurface( clouds );
    SDL_FreeSurface( lives_image );
    SDL_FreeSurface( seeker_image );
    SDL_FreeSurface( boss1_image );
    SDL_FreeSurface( danger_image );
    SDL_FreeSurface( paused_image );
    SDL_FreeSurface( pause_overlay );

    Mix_FreeMusic( music );
    Mix_FreeChunk( shot_sound );
    Mix_FreeChunk( explosion );

    //Quit SDL_ttf
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
