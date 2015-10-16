#include "gamesprites.h"
#include "globals.h"
#include <list>

ImageInfo::ImageInfo(Point2D center, Point2D size, double radius, bool animated)
{
    obj_center = center;
    obj_size = size;
    obj_radius = radius;
    obj_animated = animated;
}

Ship::Ship(Point2D pos, Point2D vel, int state, SDL_Surface* img, ImageInfo info)
{
    setPos(pos);
    setVel(vel);
    setImg(img);
    setInfo(info);
    s_state = state;
    is_moving = false;
    s_buffs = 0;
    s_ammo = 0;
    s_delay = DELAY;
    s_aura = false;
}

void Ship::draw(SDL_Surface* screen)
{
    switch (s_state)
    {
    case SHIP_RIGHT:
        draw_image(screen, getImg(), getCenter(), getSize(), getPos(), getSize());
        break;
    case SHIP_CENTER:
        draw_image(screen, getImg(), Point2D(getCenter().getX() + getSize().getX(), getCenter().getY()), getSize()
                   , getPos(), getSize());
        break;
    case SHIP_LEFT:
        draw_image(screen, getImg(), Point2D(getCenter().getX() + 2* getSize().getX(), getCenter().getY()), getSize()
                   , getPos(), getSize());
    }

}

void Ship::update()
{
    setPos(getPos() + getVel() * fpst.speedfactor);
    setVel(getVel() * 0.95);

    if (!is_moving )
    {
        s_state = SHIP_CENTER;
    }

    if (getPos().getX() <= 20)
    {
        changePosX(20);
    }

    else if (getPos().getX() >= SCREEN_WIDTH - 105)
    {
        changePosX(SCREEN_WIDTH - 105);
    }

    if (getPos().getY() <= 20)
    {
        changePosY(20);
    }

    else if (getPos().getY() >= 460)
    {
        changePosY(460);
    }
}

void Ship::shot(Sprite turret, std::list<Sprite*> &missile_group)
{
    Point2D missile_pos = getPos();
    Point2D missile_vel;

    switch (turret.getCurFrame())
    {
    case 0:
        {
            missile_pos.addY(-15);
            missile_vel.setX(0);
            missile_vel.setY(- MISSILE_ABS_VEL );
            break;
        }
    case 1:
        {
            missile_pos.addX(-15);
            missile_vel.setX(- MISSILE_ABS_VEL );
            missile_vel.setY(0);
            break;
        }
    case 2:
        {
            missile_pos.addY(15);
            missile_vel.setX(0);
            missile_vel.setY( MISSILE_ABS_VEL );
            break;
        }
    case 3:
        {
            missile_pos.addX(15);
            missile_vel.setX( MISSILE_ABS_VEL );
            missile_vel.setY(0);
            break;
        }
    case 4:
        {
            missile_pos.addX( - 15.0 * cos(M_PI_4) );
            missile_pos.addY( - 15.0 * sin(M_PI_4) );
            missile_vel.setX( - MISSILE_ABS_VEL * cos(M_PI_4) );
            missile_vel.setY( - MISSILE_ABS_VEL * sin(M_PI_4) );
            break;
        }
    case 5:
        {
            missile_pos.addX(- 15.0 * cos(M_PI_4) );
            missile_pos.addY( 15.0 * sin(M_PI_4) );
            missile_vel.setX( - MISSILE_ABS_VEL * cos(M_PI_4) );
            missile_vel.setY(  MISSILE_ABS_VEL * sin(M_PI_4) );
            break;
        }
    case 6:
        {
            missile_pos.addX( 15.0 * cos(M_PI_4) );
            missile_pos.addY( 15.0 * sin(M_PI_4) );
            missile_vel.setX( MISSILE_ABS_VEL * cos(M_PI_4) );
            missile_vel.setY( MISSILE_ABS_VEL * sin(M_PI_4) );
            break;
        }
    case 7:
        {
            missile_pos.addX( 15.0 * cos(M_PI_4) );
            missile_pos.addY( - 15.0 * sin(M_PI_4) );
            missile_vel.setX( MISSILE_ABS_VEL * cos(M_PI_4) );
            missile_vel.setY( - MISSILE_ABS_VEL * sin(M_PI_4) );
            break;
        }
    }

    switch (s_buffs)
    {
    case 0:
        missile_group.push_back(new Sprite(missile_pos, missile_vel, missile_image, missile_info));
        break;
    case 1:
    {
        Sprite* missile_center = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        Sprite* missile_left;
        Sprite* missile_right;
        int tmp = turret.getCurFrame();
        if (tmp == 0 || tmp == 2)
        {
            missile_vel.setX(- MISSILE_ABS_VEL / cos(M_PI_4));
            missile_left = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
            missile_vel.setX( MISSILE_ABS_VEL / cos(M_PI_4));
            missile_right = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        }
        else if (tmp == 1 || tmp == 3)
        {
            missile_vel.setY(- MISSILE_ABS_VEL / cos(M_PI_4));
            missile_left = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
            missile_vel.setY( MISSILE_ABS_VEL / cos(M_PI_4));
            missile_right = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        }
        else if (tmp == 4)
        {
            missile_vel = missile_vel * 0;
            missile_vel.setY(- MISSILE_ABS_VEL );
            missile_left = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
            missile_vel = missile_vel * 0;
            missile_vel.setX( - MISSILE_ABS_VEL );
            missile_right = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        }
        else if (tmp == 5)
        {
            missile_vel = missile_vel * 0;
            missile_vel.setX(- MISSILE_ABS_VEL );
            missile_left = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
            missile_vel = missile_vel * 0;
            missile_vel.setY( MISSILE_ABS_VEL );
            missile_right = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        }
        else if (tmp == 6)
        {
            missile_vel = missile_vel * 0;
            missile_vel.setX( MISSILE_ABS_VEL );
            missile_left = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
            missile_vel = missile_vel * 0;
            missile_vel.setY( MISSILE_ABS_VEL );
            missile_right = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        }
        else if (tmp == 7)
        {
            missile_vel = missile_vel * 0;
            missile_vel.setX( MISSILE_ABS_VEL );
            missile_left = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
            missile_vel = missile_vel * 0;
            missile_vel.setY( - MISSILE_ABS_VEL );
            missile_right = new Sprite(missile_pos, missile_vel, missile_image, missile_info);
        }

        missile_group.push_back(missile_left);
        missile_group.push_back(missile_center);
        missile_group.push_back(missile_right);
        s_ammo--;
        if (s_ammo == 0)
        {
            setBuff( 0 );
        }
        break;
    }
    case 2:
        missile_group.push_back(new Sprite(missile_pos, missile_vel, missile_image, missile_info));
        s_ammo--;
        if (s_ammo == 0)
        {
            setBuff( 0 );
            setDelay( DELAY );
        }
        break;
    }
}

Kamikaze::Kamikaze(Point2D pos, Point2D vel, int state, SDL_Surface* img, ImageInfo info)
{
    setPos(pos);
    setVel(vel);
    setImg(img);
    setInfo(info);
    setAnimated(false);
    s_state = state;
    wait = 0;
    wait_frames = rand() % 101 + 40;
    angle = 0;
    s_state = 0;
}

void Kamikaze::update( std::list<Sprite*> &bullets )
{
    Sprite* player = my_ship;
    if (wait < wait_frames)
    {
        setPos(getPos() + getVel() * fpst.speedfactor);
        wait++;

        if (abs(getPos().getX() - player->getPos().getX()) < player->getRadius() )
            angle = M_PI / 2;
        else
        {
            angle = getPos().angle(player->getPos());
            if (getPos().getX() > player->getPos().getX() )
                s_state = 1;
        }
    }
    else
    {
        switch (s_state)
        {
        case 0:
            if (sin(angle) < 0)
                setPos(getPos() + Point2D(K_VEL * cos(angle) , -K_VEL * sin(angle)) * fpst.speedfactor );
            else
                setPos(getPos() + Point2D(K_VEL * cos(angle), K_VEL * sin(angle)) * fpst.speedfactor );
            break;
        case 1:
            if (sin(angle) < 0)
                setPos(getPos() + Point2D(-K_VEL * cos(angle), -K_VEL * sin(angle)) * fpst.speedfactor);
            else
                setPos(getPos() + Point2D(K_VEL * cos(angle), K_VEL * sin(angle))* fpst.speedfactor);
        }

    }
}

Shooter::Shooter(Point2D pos, Point2D vel, int state, SDL_Surface* img, ImageInfo info)
{
    setPos(pos);
    setVel(vel);
    setImg(img);
    setInfo(info);
    setAnimated(false);
    s_state = state;
    wait = 0;
    wait_frames = 10;
    angle = 0;
    s_state = 0;
}

void Shooter::update( std::list<Sprite*> &bullets )
{
    Sprite* player = my_ship;

    setPos(getPos() + getVel() * fpst.speedfactor);

    if (abs(getPos().getX() - player->getPos().getX()) < player->getRadius() &&  player->getPos().getY() > getPos().getY())
    {
        if (wait % wait_frames == 0 && wait > 0)
        {
            shot( bullets );
        }
    }
    else
    {
        int var = rand() % 2;
        if(player->getPos().getX() > getPos().getX())
        {
            if (var)
                changeVelX( 2 );
            else
                changeVelX( 0 );
        }
        else
        {
            if (var)
                changeVelX( -2 );
            else
                changeVelX( 0 );
        }
    }

    wait++;
}

void Shooter::shot( std::list<Sprite*> &bullets )
{
    Point2D missile_pos = getPos();
    Point2D missile_vel = Point2D(0, 15);
    if( Mix_PlayChannel( -1, shot_sound, 0 ) == -1 )
    {
        //return 1;
    }

    bullets.push_back(new Sprite(missile_pos, missile_vel, missile_image, missile_info));
}

Buff::Buff(Point2D pos, Point2D vel, SDL_Surface* img, ImageInfo info, bool animated, int frames)
{
    setPos(pos);
    setVel(vel);
    setImg(img);
    setInfo(info);
    setAnimated(animated);
    setFrames( frames );
}

SeekerMissile::SeekerMissile(Point2D pos, Point2D vel, SDL_Surface* img, ImageInfo info, bool animated, int frames)
{
    setPos(pos);
    setVel(vel);
    setImg(img);
    setInfo(info);
    setAnimated(animated);
    setFrames( frames );
}

void SeekerMissile::update( std::list<Sprite*> &bullets )
{
    Point2D relPos = getPos() - my_ship->getPos();
    Point2D missile_vel = Point2D(0, 0);

    if (abs(relPos.getX()) > my_ship->getRadius() )
    {
        if (relPos.getX() > 0)
        {
            missile_vel.setX( -SEEKER_MISSILE_VEL );
        }
        else
            missile_vel.setX( SEEKER_MISSILE_VEL );
    }
    else
    {
        missile_vel.setX( 0 );
    }

    if (abs(relPos.getY()) > my_ship->getRadius() )
    {
        if (relPos.getY() > 0)
        {
            missile_vel.setY( -SEEKER_MISSILE_VEL );
        }
        else
            missile_vel.setY( SEEKER_MISSILE_VEL );
    }
    else
    {
        missile_vel.setY( 0 );
    }

    int velX = (int) missile_vel.getX();
    int velY = (int) missile_vel.getY();

    if (velX > 0 && velY > 0)
    {
        setCurFrame( 6 );
    }
    else if (velX > 0 && velY < 0)
    {
        setCurFrame( 7 );
    }
    else if (velX < 0 && velY > 0)
    {
        setCurFrame( 5 );
    }
    else if (velX < 0 && velY < 0)
    {
        setCurFrame( 4 );
    }
    else if (velX > 0)
    {
        setCurFrame( 3 );
    }
    else if (velX < 0)
    {
        setCurFrame( 1 );
    }
    else if (velY > 0)
    {
        setCurFrame( 2 );
    }
    else
    {
        setCurFrame( 0 );
    }

    setPos(getPos() + missile_vel * fpst.speedfactor);

}

MissileShip::MissileShip(Point2D pos, Point2D vel, SDL_Surface* img, ImageInfo info, bool animated, int frames)
{
    setPos(pos);
    setVel(vel);
    setImg(img);
    setInfo(info);
    setAnimated(animated);
    setFrames( frames );
    life = 50;
    wait = 0;
    intro = true;
    s_intro = false;
    intro_wait = 0;
    can_get_damaged = false;
}

void MissileShip::update( std::list<Sprite*> &seekerMissiles )
{
    if ( intro )
    {
        if (out_of_screen(getPos() - Point2D(0, 15)) || s_intro )
        {
            s_intro = true;
            if (intro_wait > 100)
            {
                setCurFrame( 0 );
                setPos(getPos() + Point2D(0, -3) * fpst.speedfactor );
            }

            if (getPos().getY() <= SCREEN_HEIGHT - 60)
            {
                setPos(Point2D(getPos().getX(), SCREEN_HEIGHT - 60));
                intro = false;
                can_get_damaged = true;
            }
            intro_wait++;
        }
        else if (intro_wait == 0)
        {
            setPos(getPos() + Point2D(0, 10) * fpst.speedfactor );
        }
    }
    else
    {
        /** Moverse horizontalmente a través de la pantalla */
        if (getPos().getX() >= MAX_OFFSET )
        {
            changePosX(MAX_OFFSET);
            changeVelX( getVel().getX() * -1);
        }
        else if ( getPos().getX() <= 20 )
        {
            changePosX( 20 );
            changeVelX( getVel().getX() * -1);
        }
        else if ( abs(getPos().getX() - SCREEN_WIDTH / 2) < 5)
        {
            if (rand() % 101 <= 50)
            {
                changeVelX( getVel().getX() * -1);
            }
        }
        /** Actualizar posición y regular framerate */
        setPos(getPos() + getVel() * fpst.speedfactor );

        /** Disparo (temporal, se debe mejorar) */

        /** 30% de probabilidad de disparara cada 60 game-loops */
        int chance = rand() % 100;
        if (chance <= 30 && wait % 60 == 0 && wait != 0)
        {
            shot(seekerMissiles);
        }
        wait++;
    }
}

void MissileShip::shot( std::list<Sprite*> &seekerMissiles )
{
    int x, y;
    x = getPos().getX();
    y = getPos().getY();

    seekerMissiles.push_back(new SeekerMissile(Point2D(x - 20, y), Point2D(0, 0), seeker_image, seeker_info, true, 7));
    seekerMissiles.push_back(new SeekerMissile(Point2D(x + 20, y), Point2D(0, 0), seeker_image, seeker_info, true, 7));
}

Sprite::Sprite(Point2D pos, Point2D vel, SDL_Surface *img, ImageInfo info, bool animated, int frames)
{
    sp_pos = pos;
    sp_vel = vel;
    sp_img = img;
    sp_center = info.getCenter();
    sp_size = info.getSize();
    sp_radius = info.getRadius();
    sp_animated = animated;
    sp_frames = frames;
    sp_cur_frame = 0;
}

void Sprite::draw(SDL_Surface* screen)
{
    if (sp_img != NULL)
    {
        if (sp_animated)
        {
            int i = getCurFrame();

            draw_image(screen, sp_img, Point2D(sp_center.getX() + i * sp_size.getX(), sp_center.getY()),
                        sp_size, sp_pos, sp_size);
        }
        else
        {
            draw_image(screen, sp_img, sp_center, sp_size, sp_pos, sp_size);
        }
    }
}

void Sprite::update( std::list<Sprite*> &bullets )
{
    sp_pos = sp_pos + sp_vel* fpst.speedfactor;
}

void Sprite::setInfo(ImageInfo info)
{
    sp_center = info.getCenter();
    sp_size = info.getSize();
    sp_radius = info.getRadius();
}

bool Sprite::collide(Sprite &another_sprite)
{
    double d = getPos().dist(another_sprite.getPos());

    if ( d < ( getRadius() + another_sprite.getRadius() ) )
        return true;
    else
        return false;
}

bool group_collide(std::list<Sprite*> &group, Sprite &other_object)
{
   bool collision = false;

   for (std::list<Sprite*>::iterator sprite = group.begin(), end = group.end(); sprite != end; ++sprite)
   {
       if ((*sprite)->collide(other_object))
       {
            Sprite exp = Sprite((*sprite)->getPos(), Point2D(0, 0), exp_image, exp_info, true, 7);
            exp_group.push_back(exp);
            if( Mix_PlayChannel( -1, explosion, 0 ) == -1 )
            {
                //abort();
            }
            delete *sprite;
            sprite = group.erase(sprite);
            collision = true;
       }
   }
   return collision;
}


int group_group_collide(std::list<Sprite*> &group, std::list<Sprite*> &other_group)
{
   int scored = 0;

   for (std::list<Sprite*>::iterator it1 = group.begin(), end1 = group.end(); it1 != end1; ++it1)
   {
        if (group_collide(other_group, *(*it1)))
        {
            delete *it1;
            it1 = group.erase(it1);
            scored += 10;
        }

   }
   return scored;
}

void enemy_spawner(std::list<Sprite*> &enemy_group)
{
    Point2D enemy_pos = Point2D(rand() % (MAX_OFFSET), -20);
    Point2D enemy_vel = Point2D(0, 1);

    int var = rand() % 2;
    if (enemy_group.size() < MAX_ENEMIES)
    {
        if (var)
            enemy_group.push_back(new Shooter(enemy_pos, Point2D(0, 3), 0, shooter_image, shooter_info));
        else
            enemy_group.push_back(new Kamikaze(enemy_pos, enemy_vel, 0, enemy_image, enemy_info));
    }

}

bool out_of_screen(Point2D point)
{
    double x, y;
    x = point.getX();
    y = point.getY();
    if (x > SCREEN_WIDTH + 30 || x < -30 || y > SCREEN_HEIGHT + 30 || y < -30)
    {
        return true;
    }
    else
    {
        return false;
    }
}
