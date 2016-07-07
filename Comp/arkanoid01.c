//Projeto Arkanoid v.0.1
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

typedef int bool;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int true = 1;
const int false = 0;

typedef struct _PNG
{
    int posX;
    int posY;
    SDL_Surface* image;
    int imgW;
    int imgH;
} PNG;

bool init(void);
void close_program(void);
bool loadMedia(void);
SDL_Surface* loadSurface( char *path );
PNG createPNG( int posX, int posY, SDL_Surface *image);

PNG barra;

SDL_Window*  gWindow = NULL ;
SDL_Surface* gScreenSurface = NULL ;
SDL_Surface* gBarraSurface = NULL;

int main(int argc, char **argv)
{

  SDL_Rect srcRect, dstRect;
  if( !init() )
  {
      printf( "Failed to initialize!\n" );
  }
  else
  {

      if( !loadMedia() )
      {
          printf( "Failed to load media!\n" );
      }
      else
      {

        barra = createPNG(288, 440, gBarraSurface);

        int quit = false;


        SDL_Event e;


        while( !quit )
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                switch (e.type)
                {
                    case SDL_QUIT:
                        quit = true;
                    break;

                    case SDL_KEYDOWN:
                        if ((e.key.keysym.sym == SDLK_ESCAPE) || (e.key.keysym.sym == SDLK_q))
                        {
                            quit = true;
                        }
                      /*  if ((e.key.keysym.sym == SDLK_LEFT))
                        {

                        }*/
                    break;
                  }
                }
              SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0xFF, 0xFF ) );

              srcRect.x = 0; srcRect.y = 0;
              srcRect.w = 64;
              srcRect.h = 16;
              dstRect.x = barra.posX;
              dstRect.y = barra.posY;

              if( SDL_BlitSurface( barra.image, &srcRect, gScreenSurface, &dstRect ) < 0 )
              {
                  printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                  quit = true;
              }

              SDL_UpdateWindowSurface( gWindow );

              SDL_Delay(50);
          }
        }
    }

  return 0;
}

bool init()
{
	int success = true ;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {

        gWindow = SDL_CreateWindow( "Arkanoid" , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {

            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else
            {

                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
    }

    return success;
}
bool loadMedia()
{

    bool success = true;


    gBarraSurface = loadSurface( "./barra.png" );
    if( gBarraSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }
    return success;
}
SDL_Surface* loadSurface( char *path )
{

    SDL_Surface* optimizedSurface = NULL;


    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {

        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }


        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}
PNG createPNG( int posX, int posY, SDL_Surface *image)
{
    PNG p;

    p.posX = posX;
    p.posY = posY;
	  p.image = image;
    return p;
}
