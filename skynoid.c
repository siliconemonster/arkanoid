//Projeto Arkanoid v.02.1
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define SPEED_MODIFIER 2
#define playerInitX 208
#define playerInitY 420
#define ballInitX 44
#define ballInitY 10
#define ballVelocidadeX 2
#define ballVelocidadeY -6
#define velMax 8
#define velMin 1
#define ballDiametro 8
#define gTile_imgW  32
#define gTile_imgH  24
#define mapSize 20
#define levelWidth 448
#define levelHeight 456
#define quit 0
#define menu 1
#define playing 2
#define gameOver 3
#define gameWin 4

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
    int vida;
    bool active;
} player;

typedef struct _ball
{
  double posX;
  double posY;
  float velX;
  float velY;
  int imgW;
  int imgH;
  bool launch;
  SDL_Surface* image;
} ball;

typedef struct _tile
{
	int value;
	int posX;
	int posY;
	int imgW;
	int imgH;
	SDL_Surface* image;
}tile;

typedef struct _GameState
{
	player player;
	ball ball;
	SDL_Rect level;
	tile tile[mapSize][mapSize];
  int status;
  bool music;

} GameState;


bool init(void); //inicializa as funções do SDL2
void close_program(void); //fecha o programa
bool loadMedia(void);// carrega as imagens
SDL_Surface* loadSurface( char *path ); //carrega as imagens nas superfícies destino
player createPlayer( int posX, int posY, SDL_Surface *image); //cria a struct do tipo "player"
ball createBall(int posX, int posY, SDL_Surface *image);
SDL_Rect createLevel();
void createTile (GameState *game);
void processGameEvents(GameState *game); //processa a fila de eventos, criei separada pra organizar melhor a main
void processMenuEvents(GameState *game);
void drawPlayer(GameState *game); //carrega e blita a imagem do jogador na superficie da mesma
void drawBall(GameState *game);
void drawMap(GameState *game);
void drawBG(GameState *game);
void drawMenu(GameState *game);
void drawLife ( GameState *game);
//void drawPontos(GameState *game);
void movePlayer(GameState *game);
void moveBall(GameState *game, int modulo);
void testCollisionBallLevel(GameState *game);
bool testCollisionBallPlayer(GameState *game);
void testCollisionBallBlock(GameState *game);
void doBlockCollision(GameState *game, int i, int j);
void launchBall(GameState *game);
void Restart(GameState *game);
void GameOver(GameState *game);
void GameWin (GameState *game);

SDL_Window*  gWindow = NULL ;
SDL_Surface* gScreenSurface = NULL ;
SDL_Surface* gMenuSurface[7];
SDL_Surface* gLevelSurface = NULL;
SDL_Surface* gBarraSurface = NULL ;
SDL_Surface* gBallSurface = NULL ;
SDL_Surface* gTileBlueSurface = NULL;
SDL_Surface* gTileRedSurface = NULL;
SDL_Surface* gTileGreenSurface = NULL;
SDL_Surface* gTileYellowSurface = NULL;
SDL_Surface* gTilePinkSurface = NULL;
SDL_Surface* gTileGreySurface = NULL;
SDL_Surface* gLifeDisplay[4];
SDL_Surface* gGameOverDisplay = NULL;
SDL_Surface* gGameWinDisplay = NULL;
Mix_Music *gMMenu = NULL;
Mix_Music *gMGame = NULL;
Mix_Chunk *gHit1 = NULL;
Mix_Chunk *gHit2 = NULL;
/*TTF_Font *gFont = NULL;
SDL_Surface *gTextSurface = NULL;
SDL_Color branco = {255,255,255};*/
GameState gameState;

int map1[mapSize][mapSize] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
	{1, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 0, 0, 0, 1},
	{1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 1},
	{1, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 1, 0, 0, 0, 1},
	{1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1}
	};


int gMenuPointer = 0;
int gLifePointer = 0;

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


        gameState.player = createPlayer(playerInitX , playerInitY , gBarraSurface);
        gameState.ball = createBall ((gameState.player.posX + ballInitX ), (gameState.player.posY - ballInitY ), gBallSurface);
        gameState.level = createLevel();
        createTile(&gameState);
        gameState.status = menu;
        gameState.music = true;

        while( gameState.status != quit )
        {
                Mix_PlayMusic( gMGame, -1 );
                Mix_VolumeMusic(MIX_MAX_VOLUME/10);

              while (gameState.status == menu)
              {
                processMenuEvents(&gameState);
                drawMenu(quit);
                SDL_UpdateWindowSurface( gWindow );
              }

              while (gameState.status == playing)
              {
                int numBlocks = 70;
                int i, j ;
                processGameEvents(&gameState);

                SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );
                drawBG(&gameState);
			          drawMap(&gameState);
			          drawLife(&gameState);
                //drawPontos(&gameState);

                if(gameState.player.active == true )
                {
					           drawPlayer(&gameState);
				             drawBall(&gameState);
                     for (i = 2; i < 7; i++){
                       for (j = 1; j < 15; j++){
                         if((i != 3 && j != 1) || (i != 5 && j != 1) || (i != 3 && j != 14) || (i != 5 && j != 14))
                         {
                           if (gameState.tile[i][j].value == 0)
                           {
                             numBlocks--;
                           }
                         }
                       }
                     }
                     if (numBlocks == 0)
                     {
                       gameState.status = gameWin;
                     }
			          }
                SDL_UpdateWindowSurface( gWindow );

                SDL_Delay(1000/40);
              }

              while (gameState.status == gameWin)
              {

                GameWin(&gameState);
              }

              while (gameState.status == gameOver)
              {
                GameOver(&gameState);
              }
          }

         close_program();
        }
    }
    return 0;
}

bool init()
{
	int success = true ;

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {

        gWindow = SDL_CreateWindow( "Skynoid" , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
            if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            if (TTF_Init() < 0 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

    gMenuSurface[0] = loadSurface("./Menus/menu0.png");
    if( gMenuSurface[0] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gMenuSurface[1] = loadSurface("./Menus/menu1.png");
    if( gMenuSurface[1] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gMenuSurface[2] = loadSurface("./Menus/menu2.png");
    if( gMenuSurface[2] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gMenuSurface[3] = loadSurface("./Menus/menu3.png");
    if( gMenuSurface[3] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gMenuSurface[4] = loadSurface("./Menus/menu4.png");
    if( gMenuSurface[4] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gMenuSurface[5] = loadSurface("./Menus/instrucoes.png");
    if( gMenuSurface[0] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gMenuSurface[6] = loadSurface("./Menus/creditos.png");
    if( gMenuSurface[6] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gLifeDisplay[3] = loadSurface("./Sprites/3Life.png");
    if( gLifeDisplay[3] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gLifeDisplay[2] = loadSurface("./Sprites/2Life.png");
    if( gLifeDisplay[2] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gLifeDisplay[1] = loadSurface("./Sprites/1Life.png");
    if( gLifeDisplay[1] == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gGameOverDisplay = loadSurface("./Menus/gameover.png");
    if( gGameOverDisplay == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gGameWinDisplay = loadSurface("./Menus/gamewin.png");
    if( gGameWinDisplay == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gLevelSurface = loadSurface("./Sprites/galaxybg.png");
    if( gLevelSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }
    gBarraSurface = loadSurface( "./Sprites/barra.png" );
    if( gBarraSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gBallSurface = loadSurface( "./Sprites/ball.png" );
    if( gBallSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

  	gTileBlueSurface = loadSurface("./Sprites/tileblue.png");
    if( gTileBlueSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gTileGreenSurface = loadSurface("./Sprites/tilegreen.png");
	  if( gTileGreenSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gTileRedSurface = loadSurface("./Sprites/tilered.png");
	  if( gTileRedSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

	  gTileYellowSurface = loadSurface("./Sprites/tileyellow.png");
	  if( gTileYellowSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gTilePinkSurface = loadSurface("./Sprites/tilepink.png");
	  if( gTilePinkSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    gTileGreySurface = loadSurface("./Sprites/borda.png");
	  if( gTileGreySurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }
    gHit1 = Mix_LoadWAV( "./Music/Hit1.wav" );
    if( gHit1 == NULL )
    {
        printf( "Failed to load hit1 sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    gHit2 = Mix_LoadWAV( "./Music/Hit2.wav" );
    if( gHit2 == NULL )
    {
        printf( "Failed to load hit2 sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gMMenu = Mix_LoadMUS( "./Music/MMenu.wav" );
    if( gMMenu == NULL )
    {
        printf( "Failed to load beat musicMENU! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gMGame = Mix_LoadMUS( "./Music/MGame.wav" );
    if( gMGame == NULL )
    {
        printf( "Failed to load beat mGAME! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    return success;

    /*gFont = TTF_OpenFont("./CrazyPixel.ttf", 48);
    if( gFont == NULL )
    {
        printf( "Failed to load font! SDL_TTF Error: %s\n", TTF_GetError() );
        success = false;
    }

    gTextSurface = TTF_RenderText_Solid(gFont, "Hello World!", branco);
    if( gTextSurface == NULL )
    {
      printf( "SDL could not write text! SDL Error: %s\n", TTF_GetError() );
    }*/

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
    p.vida = 3;
    p.active = true;

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
    b.velX = ballVelocidadeX;
    b.velY = ballVelocidadeY;
    return b;
}

SDL_Rect createLevel()
{
    SDL_Rect level;
    level.x = gTile_imgW;
    level.y = gTile_imgH;
    level.w = levelWidth;
    level.h = levelHeight;
    return level;
}

void createTile (GameState *game)
{
	int  i ,  j;
	for (i = 0; i < mapSize; i++)
	{
		for( j = 0; j < mapSize; j++)
		{

			game->tile[i][j].value = map1[i][j];

			game->tile[i][j].posX  = j*gTile_imgW;
			game->tile[i][j].posY = i*gTile_imgH;
			game->tile[i][j].imgW = gTile_imgW;
			game->tile[i][j].imgH = gTile_imgH;

			if ( game->tile[i][j].value == 1)
			{
				game->tile[i][j].image = gTileGreySurface;
			}
			if ( game->tile[i][j].value == 2)
			{
				game->tile[i][j].image = gTileRedSurface;
			}
			if ( game->tile[i][j].value == 3)
			{
				game->tile[i][j].image = gTileYellowSurface;
			}
			if ( game->tile[i][j].value == 4)
			{
				game->tile[i][j].image = gTilePinkSurface;
			}
			if ( game->tile[i][j].value == 5)
			{
				game->tile[i][j].image = gTileGreenSurface;
			}
			if ( game->tile[i][j].value == 6)
			{
				game->tile[i][j].image = gTileBlueSurface;
			}
		}
	}
}

void processGameEvents(GameState *game)
{

  SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
        switch (e.type)
        {
            case SDL_QUIT:
                game->status = quit;
            break;

            case SDL_KEYDOWN:
                if ((e.key.keysym.sym == SDLK_ESCAPE) || (e.key.keysym.sym == SDLK_q))
                {
                    game->status = quit;
                }

            break;
          }
    }
  	movePlayer(game);
  	launchBall(game);


  }

void processMenuEvents(GameState *game)
{

    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
      switch (e.type)
      {
          case SDL_QUIT:
              game->status = quit;
          break;

          case SDL_KEYDOWN:
              if ((e.key.keysym.sym == SDLK_ESCAPE) || (e.key.keysym.sym == SDLK_q))
              {
                  game->status = quit;
              }
              if((e.key.keysym.sym == SDLK_UP))
              {
                if(gMenuPointer > 0 && gMenuPointer <= 4)
                {
                  gMenuPointer--;
                }
              }
              if((e.key.keysym.sym == SDLK_DOWN))
              {
                if(gMenuPointer < 4)
                {
                  gMenuPointer++;
                }
              }
              if((e.key.keysym.sym) == SDLK_RETURN)
              {
                switch (gMenuPointer)
                {
                    case 0:
                      game->status = playing;
                    break;
                    case 1:
                      gMenuPointer = 5;
                    break;
                    case 2:
                      game->music = false;
                    case 3:
                      gMenuPointer = 6;
                    break;
                    case 4:
                      game->status = quit;
                    break;
                    case 5:
                      gMenuPointer = 0;
                    break;
                    case 6:
                      gMenuPointer = 0;
                    break;
                }
              }
      }
    }
}

void drawMenu (GameState *game)
{
  SDL_Rect srcRect, dstRect;
  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = SCREEN_WIDTH;
  srcRect.h = SCREEN_HEIGHT;
  dstRect.x = 0;
  dstRect.y = 0;

  if(SDL_BlitSurface (gMenuSurface[gMenuPointer], &srcRect, gScreenSurface, &dstRect) < 0)
  {
    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
      game->status = quit;
  }
}

void drawLife (GameState *game)
{
  SDL_Rect srcRect, dstRect;
  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = 96;
  srcRect.h = 72;
  dstRect.x = 512;
  dstRect.y = 72;

  if(SDL_BlitSurface (gLifeDisplay[game->player.vida], &srcRect, gScreenSurface, &dstRect) < 0)
  {
    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
    game->status = quit;
  }
}

void drawPlayer (GameState *game)
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
        game->status = quit;;
    }
  }

void drawBall (GameState *game)
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
        game->status = quit;
    }
  }

void drawMap(GameState *game)
{
  	int i , j;
  	SDL_Rect srcRect, dstRect;
  	srcRect.x = 0; srcRect.y = 0;

  	for( i = 0; i < mapSize; i++ )
  	{
  		for (j = 0; j < mapSize; j++)
  		{
  				if (game->tile[i][j].value != 0)
  				{
  					srcRect.w = game->tile[i][j].imgW;
  					srcRect.h = game->tile[i][j].imgH;
  					dstRect.x = game->tile[i][j].posX;
  					dstRect.y = game->tile[i][j].posY;

  					if( SDL_BlitSurface( game->tile[i][j].image, &srcRect, gScreenSurface, &dstRect ) < 0 )
  					{
  						printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
  						game->status = quit;
  					}
  				}
  	    }
  	}
}

void drawBG(GameState *game)
{
  SDL_Rect srcRect;
  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = game->level.w;
  srcRect.h = game->level.h;
  if( SDL_BlitSurface( gLevelSurface, &srcRect, gScreenSurface, &game->level ) < 0 )
  {
    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
    game->status = quit;
  }
}

/*void drawPontos(GameState *game)
{
  SDL_Rect srcRect, dstRect;

  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = 96;
  srcRect.h = 72;
  dstRect.x = 512;
  dstRect.y = 168;


  if( SDL_BlitSurface(gTextSurface, &srcRect , gScreenSurface , &dstRect) < 0)
  {
    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
    game->status = quit;
  }

}*/

void movePlayer(GameState *game)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

    if  (state[SDL_SCANCODE_LEFT])
    {
      game->player.posX -= 12;
      if(!(game->ball.launch))
      {
        game->ball.posX = game->player.posX + ballInitX;
      }
      if(game->player.posX <= game->level.x)
      {
        game->player.posX = game->level.x;
        if(!(game->ball.launch))
        {
          game->ball.posX = game->player.posX + ballInitX;
        }
      }

    }

    if  (state[SDL_SCANCODE_RIGHT])
    {
      game->player.posX += 12;
      if(!(game->ball.launch))
      {
        game->ball.posX = game->player.posX + ballInitX;
      }
      if(game->player.posX >= game->level.x + game->level.w - game->player.imgW)
      {
        game->player.posX = game->level.x + game->level.w - game->player.imgW;
        if(!(game->ball.launch))
        {
          game->ball.posX = game->player.posX + ballInitX;
        }
      }
    }
}

void moveBall(GameState *game, int modulo)
{
    if(modulo == 1){
      game->ball.velX = game->ball.velX;
    }
    else if(modulo == 0){
      game->ball.velX = -game->ball.velX;
    }
    game->ball.posX += game->ball.velX;
    game->ball.posY += game->ball.velY;

    testCollisionBallLevel(game);

    if (testCollisionBallPlayer(game) && game->ball.launch == true)
    {

      int playerCentro = game->player.posX + (game->player.imgW / 2)  ;
      int ballCentro = game->ball.posX + (game->ball.imgW / 2);

      int hitLocation = ballCentro - playerCentro;

      if(hitLocation < 0)
      {
        if(game->ball.velX > velMin )
        {
          game->ball.velX /= SPEED_MODIFIER;
        }
        if(game->ball.velX < 0 && game->ball.velX > -velMax)
        {
          game->ball.velX *= SPEED_MODIFIER;
        }
      }
      if(hitLocation > 0)
      {
        if(game->ball.velX > 0 && game->ball.velX < velMax)
        {
          game->ball.velX *= SPEED_MODIFIER;
        }
        if(game->ball.velX < -velMin)
        {
          game->ball.velX /= SPEED_MODIFIER;
        }
      }
      game->ball.velY = -game->ball.velY;
    }

    if(game->ball.posY - 4 > SCREEN_HEIGHT)
    {
      if(game->player.vida > 1)
      {
          game->player.vida--;
          game->ball.launch = false;
          Restart(game);
          SDL_Delay(1000);
          game->player.active = true;
          game->ball.velX = ballVelocidadeX;
      }
      else if (game->player.vida == 1)
      {
        game->status = gameOver;
      }
    }

    testCollisionBallBlock(game);
}

void testCollisionBallLevel(GameState *game)
{
    if(game->ball.posX + game->ball.imgW > game->level.w + game->level.x) //Caso 1: Colisão com a parede direita.
    {
      game->ball.velX = -(game->ball.velX);
      game->ball.posX += game->ball.velX;
    }

    if(game->ball.posX < game->level.x) //Caso 2: Colisão com a parede esquerda.
    {
      game->ball.velX = -(game->ball.velX);
      game->ball.posX += game->ball.velX;
    }

    if(game->ball.posY < game->level.y) //Caso 3: Colisão com teto.
    {
      game->ball.velY = -(game->ball.velY);
      game->ball.posY += game->ball.velY;
    }
}

bool testCollisionBallPlayer(GameState *game)
{
  	double ballPosX = game->ball.posX, ballPosY = game->ball.posY;
    double ballVel= game->ball.velY;
    int ballW = game->ball.imgW, ballH = game->ball.imgH;

    int playerPosX = game->player.posX, playerPosY = game->player.posY;
    int playerW = game->player.imgW, playerH = game->player.imgH;

    if ( (ballVel > 0) && (ballPosY + ballH >= playerPosY) &&
         (ballPosY + ballH <= playerPosY + playerH) )
    {
      if ( (ballPosX <= playerPosX + playerW) && (ballPosX + ballW >= playerPosX))
      {
        return true;
      }
    }
    return false;
}

void testCollisionBallBlock(GameState *game)
{
		//Valores temporários
    int esquerdaX = game->ball.posX, esquerdaY = game->ball.posY + (game->ball.imgH / 2);
    int direitaX = game->ball.posX + game->ball.imgW, direitaY = game->ball.posY + (game->ball.imgH / 2);
    int topoX = game->ball.posX + (game->ball.imgW / 2), topoY = game->ball.posY;
    int baixoX = game->ball.posX + (game->ball.imgW / 2), baixoY = game->ball.posY + game->ball.imgH;
    bool topo = false, baixo = false, esquerda = false, direita = false;
    int i, j;


      for( i = 0; i < mapSize; i++){
        for(j = 0; j < mapSize; j++){
          if(game->tile[i][j].value != 0 && game->tile[i][j].value != 1)
          {
            //Colisão Superior
            if( (topoX >= game->tile[i][j].posX) && (topoX <= game->tile[i][j].posX + game->tile[i][j].imgW) &&
                (topoY >= game->tile[i][j].posY) && (topoY <= game->tile[i][j].posY + game->tile[i][j].imgH) )
                {
                  topo = true;
                  doBlockCollision(game, i, j);
                  ;
                }
            //Colisão Inferior
            if( (baixoX >= game->tile[i][j].posX) && (baixoX <= game->tile[i][j].posX + game->tile[i][j].imgW) &&
                (baixoY >= game->tile[i][j].posY) && (baixoY <= game->tile[i][j].posY + game->tile[i][j].imgH) )
                {
                  baixo = true;
                  doBlockCollision(game,i, j);

                }

            //Colisão com lado esquerdo.
            if( (esquerdaX >= game->tile[i][j].posX) && (esquerdaX <= game->tile[i][j].posX + game->tile[i][j].imgW) &&
                (esquerdaY >= game->tile[i][j].posY) && (esquerdaY <= game->tile[i][j].posY + game->tile[i][j].imgH) )
                {
                  esquerda = true;
                  doBlockCollision(game, i, j);

                }
            //Colisão com lado direito.
            if( (direitaX >= game->tile[i][j].posX) && (direitaX <= game->tile[i][j].posX + game->tile[i][j].imgW) &&
                (direitaY >= game->tile[i][j].posY) && (direitaY <= game->tile[i][j].posY + game->tile[i][j].imgH) )
                {
                  direita = true;
                  doBlockCollision(game, i, j);

                }
        }
      }
    }
    if(topo)
    {
      game->ball.velY = -game->ball.velY;

    }
    if(baixo)
    {
      game->ball.velY = -game->ball.velY;

    }
    if(esquerda)
    {
      game->ball.velX = -game->ball.velX;

    }
    if(direita)
    {
      game->ball.velX = -game->ball.velX;

    }

}

void doBlockCollision(GameState *game, int i, int j)
{
    game->tile[i][j].value = 0;
}

void launchBall(GameState *game)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  int modulo;

    if  ((state[SDL_SCANCODE_SPACE]) && (game->ball.launch == false))
    {
      srand(time(NULL));
      modulo = rand() % 2;
      game->ball.launch = true;
    }
    if (game->ball.launch == true)
    {
      moveBall(game, modulo);
    }
}

void  Restart(GameState *game)
{
		game->player.active = false;
		game->player.posX = playerInitX;
		game->player.posY = playerInitY;
		game->ball.posX = game->player.posX + ballInitX;
		game->ball.posY = game->player.posY - ballInitY;
}

void GameWin (GameState *game)
{
  SDL_Rect srcRect, dstRect;
  srcRect.x = 0; srcRect.y = 0;
  srcRect.w = SCREEN_WIDTH;
  srcRect.h = SCREEN_HEIGHT;
  dstRect.x = 0;
  dstRect.y = 0;

  if(SDL_BlitSurface (gGameWinDisplay, &srcRect, gScreenSurface, &dstRect) < 0)
  {
    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
  }
  SDL_UpdateWindowSurface( gWindow );

  SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
      switch (e.type)
      {
          case SDL_QUIT:
              game->status = quit;
          break;

          case SDL_KEYDOWN:
              if ((e.key.keysym.sym == SDLK_ESCAPE) || (e.key.keysym.sym == SDLK_q))
              {
                  game->status = quit;
              }
              if((e.key.keysym.sym == SDLK_RETURN))
              {
                  gameState.status = menu;
              }
    }
  }


  game->player.vida = 3;
  game->player = createPlayer(playerInitX , playerInitY , gBarraSurface);
  game->ball = createBall ((gameState.player.posX + ballInitX ), (gameState.player.posY - ballInitY ), gBallSurface);
  game->level = createLevel();
  createTile(&gameState);
}

void GameOver(GameState *game)
{


	SDL_Rect srcRect, dstRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = SCREEN_WIDTH;
	srcRect.h = SCREEN_HEIGHT;
	dstRect.x = 0;
  dstRect.y = 0;

	if(SDL_BlitSurface (gGameOverDisplay, &srcRect, gScreenSurface, &dstRect) < 0)
  {
    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
  }
  SDL_UpdateWindowSurface( gWindow );

	SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
      switch (e.type)
      {
          case SDL_QUIT:
              game->status = quit;
          break;

          case SDL_KEYDOWN:
              if ((e.key.keysym.sym == SDLK_ESCAPE) || (e.key.keysym.sym == SDLK_q))
              {
                  game->status = quit;
              }
              if((e.key.keysym.sym == SDLK_RETURN))
              {
				          gameState.status = menu;
			        }
		}
	}


  game->player.vida = 3;
  game->player = createPlayer(playerInitX , playerInitY , gBarraSurface);
  game->ball = createBall ((gameState.player.posX + ballInitX ), (gameState.player.posY - ballInitY ), gBallSurface);
  game->level = createLevel();
  createTile(&gameState);

}

void close_program(void)
{

  	 SDL_FreeSurface(gScreenSurface);
  	 gScreenSurface = NULL ;
  	 SDL_FreeSurface(gLevelSurface );
  	 gLevelSurface = NULL;
  	 SDL_FreeSurface(gBarraSurface);
  	 gBarraSurface = NULL ;
  	 SDL_FreeSurface(gBallSurface );
  	 gBallSurface = NULL ;
  	 SDL_FreeSurface( gTileBlueSurface);
  	 gTileBlueSurface = NULL;
  	 SDL_FreeSurface( gTileRedSurface );
  	 gTileRedSurface = NULL;
  	 SDL_FreeSurface(gTileGreenSurface );
  	 gTileGreenSurface = NULL;
  	 SDL_FreeSurface(gTileYellowSurface );
  	 gTileYellowSurface = NULL;
  	 SDL_FreeSurface(gTilePinkSurface );
     gTilePinkSurface = NULL;
     SDL_FreeSurface(gTileGreySurface);
     gTileGreySurface = NULL;

    Mix_FreeChunk( gHit1 );
    Mix_FreeChunk( gHit2 );
    gHit1 = NULL;
    gHit2 = NULL;

  	Mix_FreeMusic( gMMenu );
    Mix_FreeMusic( gMGame);
    gMMenu = NULL;
  	gMGame = NULL;

    /*TTF_CloseFont(gFont);
    gFont = NULL;*/

  	SDL_DestroyWindow(gWindow);
  	gWindow = NULL ;

  	Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
