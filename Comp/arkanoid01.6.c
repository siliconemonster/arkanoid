//Projeto Arkanoid v.0.2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef int bool;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int true = 1;
const int false = 0;

typedef struct _player
{
    int posX;
    int posY;
    SDL_Surface* image;
    int imgW;
    int imgH;
} player;

typedef struct _ball
{
  int posX;
  int posY;
  int velX;
  int velY;
  int imgW;
  int imgH;
  bool launch;
  SDL_Surface* image;
} ball;


typedef struct _GameState
{

  player player;
  ball ball;
  SDL_Rect level;

} GameState;

bool init(void); //inicializa as funções do SDL2
void close_program(void); //fecha o programa
bool loadMedia(void);// carrega as imagens
SDL_Surface* loadSurface( char *path ); //carrega as imagens nas superfícies destino
player createPlayer( int posX, int posY, SDL_Surface *image); //cria a struct do tipo "player"
ball createBall(int posX, int posY, SDL_Surface *image);
SDL_Rect createLevel();
bool processEvents(SDL_Window* gWindow, GameState *game); //processa a fila de eventos, criei separada pra organizar melhor a main
void drawPlayer(GameState *game, int quit); //carrega e blita a imagem do jogador na superficie da mesma
void drawBall(GameState *game, int quit);
void movePlayer(GameState *game);
void moveBall(GameState *game, int modulo, int velX);
void collisionBall(GameState *game);
void launchBall(GameState *game);

SDL_Window*  gWindow = NULL ;
SDL_Surface* gScreenSurface = NULL ;
SDL_Surface* gLevelSurface = NULL;
SDL_Surface* gBarraSurface = NULL ;
SDL_Surface* gBallSurface = NULL ;
GameState gameState;


int main(int argc, char **argv)
{
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

        gameState.player = createPlayer(288, 420, gBarraSurface);
        gameState.ball = createBall ((gameState.player.posX + 24), (gameState.player.posY - 15), gBallSurface);
        gameState.level = createLevel();
        int quit = false;

        while( !quit )
        {
              quit = processEvents(gWindow, &gameState);

              SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );
              SDL_FillRect( gScreenSurface, &gameState.level, SDL_MapRGB( gScreenSurface->format, 0x35, 0xD7, 0xF0 ));

              drawPlayer(&gameState, quit);
              drawBall(&gameState, quit);

              SDL_UpdateWindowSurface( gWindow );

              SDL_Delay(1000/50);
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
                gLevelSurface = SDL_CreateRGBSurface(0, 600, 440, 32, 0x00, 0x00, 0xFF, 0);


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

    gBallSurface = loadSurface( "./ball.png" );
    if( gBallSurface == NULL )
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

player createPlayer( int posX, int posY, SDL_Surface *image)
{
    player p;

    p.posX = posX;
    p.posY = posY;
	  p.image = image;
    p.imgW = image->w;
    p.imgH = image->h;
    return p;
}

ball createBall(int posX, int posY, SDL_Surface *image)
{
    ball b;
    b.posX = posX;
    b.posY = posY;
    b.image = image;
    b.imgW = image->w;
    b.imgH = image->h;
    b.launch = false;
    b.velX = 5;
    b.velY = -5;
    return b;
}

SDL_Rect createLevel()
{
    SDL_Rect level;
    level.x = 32;
    level.y = 24;
    level.w = 480;
    level.h = 456;
    return level;
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


  movePlayer(game);
  launchBall(game);


  return quit;
}

void drawPlayer (GameState *game, int quit)
{
  SDL_Rect srcRect, dstRect;
  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = game->player.imgW;
  srcRect.h = game->player.imgH;
  dstRect.x = game->player.posX;
  dstRect.y = game->player.posY;

  if( SDL_BlitSurface( game->player.image, &srcRect, gScreenSurface, &dstRect ) < 0 )
  {
      printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
      quit = true;
  }
}

void drawBall (GameState *game, int quit)
{
  SDL_Rect srcRect, dstRect;
  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = game->ball.imgW;
  srcRect.h = game->ball.imgH;
  dstRect.x = game->ball.posX;
  dstRect.y = game->ball.posY;

  if( SDL_BlitSurface( game->ball.image, &srcRect, gScreenSurface, &dstRect ) < 0 )
  {
      printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
      quit = true;
  }
}

void movePlayer(GameState *game)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

    if  (state[SDL_SCANCODE_LEFT])
    {
      game->player.posX -= 12;
      if(!(game->ball.launch))
      {
        game->ball.posX = game->player.posX + 24;
      }
      if(game->player.posX <= game->level.x)
      {
        game->player.posX = game->level.x;
        if(!(game->ball.launch))
        {
          game->ball.posX = game->player.posX + 24;
        }
      }

    }

    if  (state[SDL_SCANCODE_RIGHT])
    {
      game->player.posX += 12;
      if(!(game->ball.launch))
      {
        game->ball.posX = game->player.posX + 24;
      }
      if(game->player.posX >= game->level.x + game->level.w - game->player.imgW)
      {
        game->player.posX = game->level.x + game->level.w - game->player.imgW;
        if(!(game->ball.launch))
        {
          game->ball.posX = game->player.posX + 24;
        }
      }
    }
}
void moveBall(GameState *game, int modulo, int velX)
{
    if(modulo == 1){
      game->ball.velX = velX;
    }
    else if(modulo == 0){
      game->ball.velX = -velX;
    }
    game->ball.posX += game->ball.velX;
    game->ball.posY += game->ball.velY;
    printf("%d\n", game->ball.velX );
    collisionBall(game);

}

void collisionBall(GameState *game)
{
  if(game->ball.posX + game->ball.imgW > game->level.w + game->level.x){
    game->ball.velX = -(game->ball.velX);
    game->ball.posX += game->ball.velX;
  //  printf("%d\n", game->ball.velX );
  }
  if(game->ball.posX < game->level.x){
    game->ball.velX = -(game->ball.velX);
    game->ball.posX += game->ball.velX;
  }
  if(game->ball.posY < game->level.y){
    game->ball.velY = -(game->ball.velY);
    game->ball.posY += game->ball.velY;
  }

  if((game->ball.posY + game->ball.imgH > game->player.posY ) &&
      (game->ball.posY < game->player.posY + game->player.imgH) &&
     (game->ball.posX  > game->player.posX )  &&
     (game->ball.posX  < game->player.posX + game->player.imgW))
  {
     game->ball.velY = -(game->ball.velY);
     game->ball.posY += game->
     ball.velY;
     printf("%d\n", game->ball.velY );
   }
}
void launchBall(GameState *game)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  int modulo;

    if  (state[SDL_SCANCODE_SPACE])
    {
      srand(time(NULL));
      modulo = rand() % 2;
      game->ball.launch = true;
    }
    if (game->ball.launch == true)
    {
      moveBall(game, modulo, 5);
    }
}
