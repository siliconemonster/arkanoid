//Projeto Arkanoid v.0.2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

typedef int bool;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int true = 1;
const int false = 0;

typedef struct _player
{
    int posX;
    int posY;
    SDL_Texture* image;
    int imgW;
    int imgH;
} player;

typedef struct _GameState
{
  player player;
} GameState;

bool init(void); //inicializa as funções do SDL2
void close_program(void); //fecha o programa
bool loadMedia(GameState *game);// carrega as imagens
SDL_Surface* loadSurface( char *path ); //carrega as imagens nas superfícies destino
player createPlayer( int posX, int posY); //cria a struct do tipo "player"
bool processEvents(SDL_Window* gWindow, GameState *game); //processa a fila de eventos, criei separada pra organizar melhor a main
void doRenderer (GameState *game);



SDL_Window*  gWindow = NULL ;
SDL_Renderer* gRenderer = NULL;
SDL_Surface* gScreenSurface = NULL ;
SDL_Surface* gBarraSurface = NULL;
GameState gameState;

int main(int argc, char **argv)
{


  if( !init() )
  {
      printf( "Failed to initialize!\n" );
  }
  else
  {

      if( !loadMedia(&gameState) )
      {
          printf( "Failed to load media!\n" );
      }
      else
      {

        gameState.player = createPlayer(288, 420);

        int quit = false;

        while( !quit )
        {
              quit = processEvents(gWindow, &gameState);

              SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0xFF ) );

              doRenderer(&gameState);

              SDL_UpdateWindowSurface( gWindow );

              SDL_Delay(50);
          }
        }
    }
  close_program();
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
          gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
          if( gRenderer == NULL )
          {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
          }
          else
          {
              SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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
    }
    return success;
}

bool loadMedia(GameState *game)
{

    bool success = true;


    gBarraSurface = loadSurface( "./barra.png" );
    if( gBarraSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    game->player.image = SDL_CreateTextureFromSurface(gRenderer, gBarraSurface);
    SDL_FreeSurface(gBarraSurface);

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
player createPlayer( int posX, int posY)
{
    player p;

    p.posX = posX;
    p.posY = posY;

    return p;
}
bool processEvents(SDL_Window *gWindow, GameState *game)
{
  bool quit = false;
  SDL_Event e;
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
          break;
        }
  }
  //movimento da barrinha (se quiserem podem por favor tentar deixá-lo mais suave.)
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if  (state[SDL_SCANCODE_LEFT])
  {
    game->player.posX -= 24;
  }
  if  (state[SDL_SCANCODE_RIGHT])
  {
    game->player.posX += 24;
  }

  return quit;
}

void doRenderer (GameState *game)
{
  SDL_RenderClear(gRenderer);

  SDL_Rect playerRect = {game->player.posX, game->player.posY, game->player.imgW, game->player.imgH};
  SDL_RenderCopy( gRenderer, game->player.image, NULL, &playerRect);

  SDL_RenderPresent(gRenderer);
}

void close_program(void)
{
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;

  IMG_Quit();
	SDL_Quit();
}
