/**
 * @file gl.c
 * 
 * @brief Runs the game of life and displays
 * 
 * @details Takes in the user's input and displays various boards. Can take
 * in a file input, change width/height, change the board size, pixel size,
 * color, and game type.
 * 
 * @author Kassidy Maberry
 * 
 * @date 2022/05/03
 * 
 * @todo -H.

 * @bug N/A
 */

#include <stdlib.h>
#include "SDL2/SDL.h" 
#include "sdl.h"
#include "life.h"


int main(int argc, char *argv[])
{
	int sprite_size = 4; /* either 2, 4, 8, or 16 */
        /* colors are RGB model valid values [0, 255] */
	unsigned char red = 140;
	unsigned char green = 145;
	unsigned char blue = 250;
	int width = 800;
	int height = 600;
        struct sdl_info_t sdl_info; /* this is needed to graphically display the game */
        int c; 
	int gtype = HEDGE;
	int placement = RANDOM;
	FILE *fp = NULL;
	int xstart, ystart;
	
	if(argc == 1){
		printf("usage: find [-n -l -a -w] -f filename -p pattern\n");
		exit(EXIT_FAILURE);
	}

	while((c = getopt(argc, argv, ":h:w:r:g:b:e:f:s:o:H")) != -1)
		switch(c) {
		case 'w':
			sscanf(optarg, "%d", &width);
 			break;
		case 'h':
			sscanf(optarg, "%d", &height);
			break;
		case 'e':
			gtype = game_type(optarg);
			printf("%d", gtype);
			break;
		case 'r':
			sscanf(optarg, "%hhu", &red);
			break;
		case 'g':
			sscanf(optarg, "%hhu", &green);
			break;
		case 'b':
			sscanf(optarg, "%hhu", &blue);
			break;
		case 's':
			sscanf(optarg, "%d", &sprite_size);
			sprite_size = sizecheck(sprite_size);
			break;
		case 'f':
			errno = 0; /* set to 0 so can process it if an error occurs */
                        /* assume filename comes after -f */
			/* optarg contains the argument for the option */
			fp = fopen(optarg, "r");
			if (fp == NULL) {
				/* strerror */
				fprintf(stderr, 
				"%s: argument to option '-f' failed: %s\n",
				argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}
			break;
		case 'o':
			placement = DEFINED;
			sscanf(optarg, "%d,%d", &xstart, &ystart);
			break;
		case 'H':
			printf("Help\n-----\n");
			printf("-w (int) modifies the width.\n");
			printf("-h (int) modifies the height\n");
			printf("-e (char) changes the game type t(torus)");
			printf(", h(hedge), k(klien)\n");
			printf("-f (file PW) sets a file to read out\n");
			printf("-s (int) modifies the sprite size\n");
			printf("-o (x,y) changes origin of the sprite placement\n");
			printf("-r (int) changes the red rgb value\n");
			printf("-b changes the blue rgb value\n");
			printf("-g changes the green rgb value\n");
			printf("-H prints out the different inputs and what");
			printf("they do\n");
			printf("-----\n");
			exit(0);
			break;
		default:
			printf("-H for help\n");
			printf("usage: find -w -h -e -f -s -o -r -b -g\n");
			break;
				
		}
	
        /* set up SDL -- works with SDL2 */
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);
	/* your life initialization code here */
	int swidth = width/sprite_size;
	int sheight = height/sprite_size;
	unsigned char  **board = init_matrix(swidth, sheight);
	unsigned char  **misc = init_matrix(swidth, sheight);
	unsigned char **tswap;
	if (placement != DEFINED){
		xstart = (int)(swidth/2);
		ystart = (int)(sheight/2);
	}
	if (fp != NULL){
		board = board_write(board, xstart, ystart, fp);
	}

	board[4][5] = 1;
	board[5][5] = 1;
	board[6][5] = 1;

	board[20][5] = 1;
	board[20][6] = 1;
	board[20][7] = 1;
        /* Main loop: loop forever. */
	// x rows - current y pos.
	while (1)
	{
		sdl_render_life(&sdl_info, board);
		/* your game of life code goes here  */	
		if (SDL_GetTicks() % 600 == 0){
			printf("%p %p\n", board, misc);
			tswap = board;
			if(gtype == HEDGE){
				board = hedgelife(board, misc, swidth, sheight);
			} else if (gtype == TORUS) {
				board = toruslife(board, misc, swidth, sheight);
			} else if (gtype == KLIEN){
				board = klien_life(board, misc, swidth, sheight);
			}
			misc = tswap;
		}
		

		/* change the  modulus value to slow the rendering */
		// if (SDL_GetTicks() % 1 == 0)
                 /* Poll for events, and handle the ones we care about. 
                  * You can click the X button to close the window
                  */
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
                        /* If escape is pressed, return (and thus, quit) */
				if (event.key.keysym.sym == SDLK_ESCAPE){
					del_arr(board, swidth);
					return 0;
				}
				break;
			case SDL_QUIT:
				del_arr(board, swidth);
				return(0);
			}
		}
	}

	return 0;
}
