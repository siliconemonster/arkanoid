/*
 * teste22_SDL.c
 * 
 * Copyright 2016 Tomaz Cuber Guimarães  	<DRE: 116057692>
 * 				  Aline Freire de Rezende	<DRE: 116110571>
 * 				  Mario Ferreira de Barros  <DRE: 116058428>
 * 
 * Sobre: O programa imprime duas imagens do tipo ".png" na tela e, caso o usuário clique com o botão esquerdo, direito ou central
 * do mouse em cima de uma delas, as imagens trocam de posição. O programa é encerrado quando a tecla ESC, a tecla Q ou o botão de 
 * fechar janela forem apertados. Foram utilizadas as bibliotecas SDL2, SDL2_IMAGE, biblioteca padrão C (stdio.h) e 
 * string.h	
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

typedef int bool;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int true = 1;
const int false = 0;

const int IMAGE_HEIGHT = 240;
const int IMAGE_WIDTH  = 320;
	
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

PNG png1;
PNG png2;

SDL_Window*  gWindow = NULL ;
SDL_Surface* gScreenSurface = NULL ;
SDL_Surface* gPNGSurface1 = NULL;
SDL_Surface* gPNGSurface2 = NULL;

int main (int argc, char** argv)
{
	SDL_Rect srcRect, dst1Rect, dst2Rect;
    int mouseX, mouseY, postempX, postempY;
    
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
            
            
			png1 = createPNG(0, 0, gPNGSurface1);
            
            png2 = createPNG(320, 240, gPNGSurface2); 
            
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
                        break;
                        
                        case SDL_MOUSEBUTTONDOWN:
							SDL_GetMouseState( &mouseX, &mouseY);
							//printf("X: %d Y: %d\n", mouseX, mouseY);
							if( ((mouseX < 320) && (mouseY < 240)) || 
								((mouseX > 320) && (mouseY > 240)) )
							{		
								postempX = png1.posX;
								postempY = png1.posY;
								png1.posX = png2.posX;
								png1.posY = png2.posY;
								png2.posX = postempX;
								png2.posY = postempY;
							}	
						break;
                    }
                }
            
                
                SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0x00, 0xFF ) );
				
				srcRect.x = 0; srcRect.y = 0;
                srcRect.w = IMAGE_WIDTH;
                srcRect.h = IMAGE_HEIGHT;
                dst1Rect.x = png1.posX;
                dst1Rect.y = png1.posY;
				dst2Rect.x = png2.posX;
				dst2Rect.y = png2.posY;
				
                if( SDL_BlitSurface( png1.image, &srcRect, gScreenSurface, &dst1Rect ) < 0 )
                {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                }
                if( SDL_BlitSurface( png2.image, &srcRect, gScreenSurface, &dst2Rect ) < 0 )
                {
                    printf( "SDL could not blit! SDL Error: %s\n", SDL_GetError() );
                    quit = true;
                }
                
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
        
        gWindow = SDL_CreateWindow( "Aline, Mario & Tomaz", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

    
    gPNGSurface1 = loadSurface( "./agua.png" );
    if( gPNGSurface1 == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }
	
	gPNGSurface2 = loadSurface( "./hidrate.png" );
    if( gPNGSurface2 == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }
    
    return success;
}

void close_program()
{
    
    SDL_FreeSurface( gPNGSurface1 );
    gPNGSurface1 = NULL;
    
    SDL_FreeSurface( gPNGSurface2 );
    gPNGSurface2 = NULL;

    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    
    IMG_Quit();
    SDL_Quit();
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
