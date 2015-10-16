#ifndef GAMESPRITES_H
#define GAMESPRITES_H

#include "simplegui.h"
#include <list>

#define SHIP_LEFT 0
#define SHIP_CENTER 1
#define SHIP_RIGHT 2

class ImageInfo
{
private:
    Point2D obj_center;
    Point2D obj_size;
    double obj_radius;
    bool obj_animated;

public:
    // Constructors
    ImageInfo() { }
    ImageInfo(Point2D center, Point2D size, double radius, bool animated = false);

    // Methods
    Point2D getCenter() { return obj_center; }
    Point2D getSize() { return obj_size; }
    double getRadius() { return obj_radius; }
    bool getAnimated() { return obj_animated; }
};

class Sprite
{
private:
    Point2D sp_pos;
    Point2D sp_vel;
    SDL_Surface* sp_img;
    Point2D sp_center;
    Point2D sp_size;
    double sp_radius;
    bool sp_animated;
    int sp_frames;
    int sp_cur_frame;

public:
    Sprite() {}
    Sprite(Point2D pos, Point2D vel, SDL_Surface *img, ImageInfo info, bool animated = false, int frames = 0);
    virtual void draw(SDL_Surface* screen);
    virtual void update( std::list<Sprite*> &bullets );
    void setInfo (ImageInfo info);
    void setPos( Point2D pos ) { sp_pos = pos; }
    void setVel( Point2D vel ) { sp_vel = vel; }
    void setImg (SDL_Surface* img) { sp_img = img; }
    void setNextFrame() { sp_cur_frame++; }
    void setCurFrame( int frame ) { sp_cur_frame = frame; }
    void setFrame( int frame ) { sp_cur_frame = frame; }
    void setFrames ( int frames ) { sp_frames = frames; }
    void setAnimated(bool animated) { sp_animated = animated; }
    void changeVelX (int c) { sp_vel.setX(c);}
    void changeVelY (int c) { sp_vel.setY(c);}
    void changePosX (int c) { sp_pos.setX(c);}
    void changePosY (int c) { sp_pos.setY(c);}
    SDL_Surface* getImg() { return sp_img; }
    Point2D getPos() { return sp_pos; }
    Point2D getVel() { return sp_vel; }
    Point2D getCenter() { return sp_center; }
    Point2D getSize() { return sp_size; }
    double getRadius() { return sp_radius; }
    int getCurFrame() { return sp_cur_frame; }
    int getFrames() { return sp_frames; }
    bool collide(Sprite &another_sprite);
    virtual ~Sprite() {}

    Sprite& operator=(Sprite other) {
        std::swap(sp_pos, other.sp_pos);
        std::swap(sp_vel, other.sp_vel);
        std::swap(sp_img, other.sp_img);
        std::swap(sp_center, other.sp_center);
        std::swap(sp_size, other.sp_size);
        std::swap(sp_radius, other.sp_radius);
        std::swap(sp_animated, other.sp_animated);
        std::swap(sp_frames, other.sp_frames);
        std::swap(sp_cur_frame, other.sp_cur_frame);
        return *this;
    }

    Sprite(const Sprite &obj)
    {
        sp_pos = obj.sp_pos;
        sp_vel = obj.sp_vel;
        sp_img = new SDL_Surface;
        *sp_img = *obj.sp_img;
        sp_center = obj.sp_center;
        sp_size = obj.sp_size;
        sp_radius = obj.sp_radius;
        sp_animated = obj.sp_animated;
        sp_frames = obj.sp_frames;
        sp_cur_frame = obj.sp_cur_frame;
    }

};

class Ship : public Sprite
{
private:
    int s_state;
    int s_buffs;
    int s_ammo;
    bool s_aura;
    bool is_moving;
    int s_delay;

public:
    Ship() {}
    Ship(Point2D pos, Point2D vel, int state, SDL_Surface* img, ImageInfo info);
    void setState(int state) { s_state = state; }
    void setMovement( bool mov ) { is_moving = mov; }
    void setBuff (int buff) { s_buffs = buff; }
    void setAmmo ( int ammo ) { s_ammo = ammo; }
    void setAura(bool aura) { s_aura = aura; }
    void setDelay(int delay) { s_delay = delay; }
    int getBuff() { return s_buffs; }
    int getAmmo() {return s_ammo; }
    bool getAura() { return s_aura; }
    int getDelay() { return s_delay; }
    void draw(SDL_Surface* screen);
    void handle_input();
    void update();
    void shot(Sprite turret, std::list<Sprite*> &missile_group);
    ~Ship() {}

};

class Kamikaze : public Sprite
{
private:
    int s_state;
    bool is_moving;
    int wait;
    int wait_frames;
    double angle;

public:
    Kamikaze() {}
    Kamikaze(Point2D pos, Point2D vel, int state, SDL_Surface* img, ImageInfo info);
    void setState(int state) { s_state = state; }
    void setMovement( bool mov ) { is_moving = mov; }
    void update( std::list<Sprite*> &bullets );
    ~Kamikaze() {}

};

class Shooter : public Sprite
{
private:
    int s_state;
    bool is_moving;
    int wait;
    int wait_frames;
    double angle;

public:
    Shooter() {}
    Shooter(Point2D pos, Point2D vel, int state, SDL_Surface* img, ImageInfo info);
    void setState(int state) { s_state = state; }
    void setMovement( bool mov ) { is_moving = mov; }
    void update( std::list<Sprite*> &bullets );
    void shot( std::list<Sprite*> &bullets );
    ~Shooter() {}
};

class Buff : public Sprite
{
private:
    int s_type;

public:
    Buff() {}
    Buff(Point2D pos, Point2D vel, SDL_Surface* img, ImageInfo info, bool animated, int frames);
    int getType() { return s_type; }
    void setType( int type ) { s_type = type; }
};

class SeekerMissile : public Sprite
{
private:
    int missile_life;
public:
    SeekerMissile() {}
    SeekerMissile(Point2D pos, Point2D vel, SDL_Surface* img, ImageInfo info, bool animated, int frames);
    void update( std::list<Sprite*> &bullets );
};

class MissileShip : public Sprite
{
private:
    int life;
    int wait;
    bool intro;
    bool s_intro;
    int intro_wait;
public:
    bool can_get_damaged;
    MissileShip() {}
    MissileShip(Point2D pos, Point2D vel, SDL_Surface* img, ImageInfo info, bool animated = false, int frames = 0);
    void setLife(int new_life) { life = new_life; }
    int getLife() { return life; }
    void update( std::list<Sprite*> &seekerMissiles );
    void shot(std::list<Sprite*> &seekerMissiles);

};

bool group_collide(std::list<Sprite*> &group, Sprite &other_object);
int group_group_collide(std::list<Sprite*> &group, std::list<Sprite*> &other_group);
void enemy_spawner(std::list<Sprite*> &enemy_group);
void buff_spawner(std::list<Buff*> &buff_group);
bool ship_enhancement(std::list<Buff*> &buff_group, Ship &ship);
bool out_of_screen(Point2D point);

#endif // GAMESPRITES_H
