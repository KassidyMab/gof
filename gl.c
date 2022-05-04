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
 * @todo multiple files/.05

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
	FILE *fpsix = NULL;
	FILE *fpfive = NULL;
	int xstart, ystart, xp, yp, xq, yq;
	xstart = xp = xq = -1;
	
	if(argc == 1){
		printf("No arguements provided use -H\n.");
		exit(EXIT_FAILURE);
	}

	while((c = getopt(argc, argv, ":h:w:r:g:b:e:f:s:o:q:Q:p:P:H")) != -1)
		switch(c) {
		case 'w':
			sscanf(optarg, "%d", &width);
 			break;
		case 'h':
			sscanf(optarg, "%d", &height);
			break;
		case 'e':
			gtype = game_type(optarg);
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
		case 'P':
			errno = 0; /* set to 0 so can process it if an error occurs */
                        /* assume filename comes after -P */
			/* optarg contains the argument for the option */
			fpsix = fopen(optarg, "r");
			
			if (fpsix == NULL) {
				/* strerror */
				fprintf(stderr, 
				"%s: argument to option '-f' failed: %s\n",
				argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}
			break;
		case 'Q':
			errno = 0; /* set to 0 so can process it if an error occurs */
                        /* assume filename comes after -Q */
			/* optarg contains the argument for the option */
			fpfive = fopen(optarg, "r");
			if (fpfive == NULL) {
				/* strerror */
				fprintf(stderr, 
				"%s: argument to option '-f' failed: %s\n",
				argv[0], strerror(errno));
				exit(EXIT_FAILURE);
			}
			break;
		case 'o':
			sscanf(optarg, "%d,%d", &xstart, &ystart);
			break;
		case 'p':
			sscanf(optarg, "%d,%d", &xp, &yp);
			break;
		case 'q':
			sscanf(optarg, "%d,%d", &xq, &yq);
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
			printf("-p (x,y) changes grid P's inital placement\n");
			printf("-P 1.06 file to write to grid\n");
			printf("-q (x,y) changes grid Q's inital placement\n");
			printf("-Q 1.05 file to write to grid\n");
			printf("-----\n");
			exit(0);
			break;
		default:
			printf("Use -H for help\n");
			printf("Usage: -w -h -e -f -s -o -r -b -g -p -P");
			printf(" -q -Q\n");
			break;
				
		}
	
        /* set up SDL -- works with SDL2 */
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);
	//life initialization code here
	int swidth = width/sprite_size;
	int sheight = height/sprite_size;
	unsigned char  **board = init_matrix(swidth, sheight);
	unsigned char  **misc = init_matrix(swidth, sheight);
	unsigned char **tswap;
	if (xstart == -1){
		xstart = (int)(swidth/2);
		ystart = (int)(sheight/2);
	}
	if (xp == -1){
		xp = (int)(swidth/2);
		yp = (int)(sheight/2);
	}
	if (xq == -1){
		xq = (int)(swidth/2);
		yq = (int)(sheight/2);
	}
	if (fp != NULL){
		board = board_write(board, xstart, ystart, fp);
	}
	if (fpfive != NULL){
		board = five_board_write(board, xq, yq, fpfive);
	}
	if (fpsix != NULL){
		board = board_write(board, xp, yp, fpsix);
	}	

	//Loop for playing the game.
	while (1)
	{
		sdl_render_life(&sdl_info, board);
		if (SDL_GetTicks() % 30 == 0){
			// Swaps the boards location while iterating.
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
					del_arr(misc, swidth);
					return 0;
				}
				break;
			case SDL_QUIT:
				del_arr(board, swidth);
				del_arr(misc, swidth);
				return(0);
			}
		}
	}
	del_arr(board, swidth);
	del_arr(misc, swidth);
	return 0;
}
