#include "simplegui.h"

SDL_Surface *load_image( std::string filename, bool alpha, SDL_Color* key)
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        if (!alpha)
        {
            optimizedImage = SDL_DisplayFormat( loadedImage );

            if (optimizedImage != NULL && key != NULL)
            {
                SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, key->r, key->g, key->b));
            }
        }
        else
        {
            optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        }

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    // Return the optimized image
    return optimizedImage;
}

void draw_image(SDL_Surface* screen, SDL_Surface* img, Point2D center_source, Point2D width_height_source,
                Point2D center_dest, Point2D width_height_dest)
{
    SDL_Rect offset;
    SDL_Rect clip;

    // Clip parameters
    clip.x = center_source.getX() - width_height_source.getX() / 2 + (int) width_height_source.getX() % 2;
    clip.y = center_source.getY() - width_height_source.getY() / 2 + (int) width_height_source.getY() % 2;
    clip.w = width_height_source.getX();
    clip.h = width_height_source.getY();

    //Get the offsets
    offset.x = center_dest.getX() - width_height_source.getX() / 2;
    offset.y = center_dest.getY() - width_height_source.getY() / 2;

    //Blit the surface
    SDL_BlitSurface( img, &clip, screen, &offset );
}

void draw_text(SDL_Surface* screen, std::string text, Point2D point, int font_size, SDL_Color textColor, std::string font_face)
{

    TTF_Font* font = TTF_OpenFont( font_face.c_str(), font_size );

    if (font == NULL)
    {
        abort();
    }

    SDL_Surface* message = TTF_RenderText_Solid( font, text.c_str(), textColor );

    if (message == NULL)
    {
        abort();
    }

    SDL_Rect offset;
    offset.x = point.getX();
    offset.y = point.getY();
    SDL_BlitSurface( message, NULL, screen, &offset );
    SDL_FreeSurface( message );
    TTF_CloseFont( font );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void framerate::Init(float tfps)
{
    targetfps = tfps;
    QueryPerformanceCounter(&framedelay);
    QueryPerformanceFrequency(&tickspersecond);
}

void framerate::SetSpeedFactor()
{
    QueryPerformanceCounter(&currentticks);
    //This frame's length out of desired length
    speedfactor = (float)(currentticks.QuadPart-framedelay.QuadPart)/((float)tickspersecond.QuadPart/targetfps);
    fps = targetfps/speedfactor;
    if (speedfactor <= 0)
    speedfactor = 1;

    framedelay = currentticks;
}

double Point2D::dist(Point2D B)
{
    return sqrt( pow(obj_x - B.obj_x, 2) + pow(obj_y - B.obj_y, 2) );
}

double Point2D::angle(Point2D B)
{
    return atan((obj_y - B.obj_y) / (obj_x - B.obj_x));
}
