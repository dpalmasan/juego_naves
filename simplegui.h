#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <math.h>
#include <windows.h>

class framerate
{
public:
  float targetfps;
  float fps;
  LARGE_INTEGER tickspersecond;
  LARGE_INTEGER currentticks;
  LARGE_INTEGER framedelay;

  float speedfactor;

  void Init(float tfps);
  void SetSpeedFactor();
};


class Point2D
{
private:
    double obj_x;
    double obj_y;

public:
    Point2D() { obj_x = 0; obj_y = 0;}
    Point2D(double x, double y) { obj_x = x; obj_y = y;}
    void setX(double x) { obj_x = x; }
    void setY(double y) { obj_y = y; }
    double getX() { return obj_x; }
    double getY() { return obj_y; }
    void addX(double c) { obj_x += c; }
    void addY(double c) { obj_y += c; }
    double dist(Point2D B);
    double angle(Point2D B);
    Point2D operator+ (const Point2D &B) {return Point2D(obj_x + B.obj_x, obj_y + B.obj_y);}
    Point2D operator- (const Point2D &B) {return Point2D(obj_x - B.obj_x, obj_y - B.obj_y);}
    Point2D operator* (const double factor) {return Point2D(obj_x * factor, obj_y * factor);}
};


/**
 * Not implemented yet
*/
SDL_Surface* SDL_ScaleSurface(SDL_Surface* Surface, Uint16 Width, Uint16 Height);

/**
 * Loads an image, there are still issues in alpha blending
*/
SDL_Surface *load_image( std::string filename, bool alpha = false, SDL_Color *key = NULL );

/**
 * Resize needs to be implemented, in order to add the last parameter
*/
void draw_image(SDL_Surface* screen, SDL_Surface *img, Point2D center_source, Point2D width_height_source,
                Point2D center_dest, Point2D width_height_dest = Point2D());

void draw_text(SDL_Surface* screen, std::string text, Point2D point, int font_size, SDL_Color textColor,
               std::string font_face = "C:/Windows/Fonts/arial.ttf");


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
#endif // SIMPLEGUI_H
