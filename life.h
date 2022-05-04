/**
 * @file life.h
 * 
 * @brief contains the various functions from life.c to be imported to other
 * programs.
 * 
 * @author Kassidy Maberry
 * 
 * @date 2022/05/03
 */
#ifndef LIFE_H_
#define LIFE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

enum bounds{IN = 2, XOUT, YOUT};
enum game_type {HEDGE = 98, TORUS, KLIEN};
enum placement {RANDOM = 49, DEFINED};



unsigned char **init_matrix(int width, int height);
void del_arr(unsigned char **a, int width);
unsigned char **arr_cpy(unsigned char **a, unsigned char **b, int width, int height);
int genupdate(unsigned char  **a, int x, int y, int friends);
void **print_array(unsigned char  **arr, int width, int height);
int hedge_neighbor(unsigned char  **arr, int x, int y);
unsigned char **hedgelife(unsigned char  **arr, unsigned char  **temp, int width, int height);
int bounds(int width, int height, int x, int y);
int torus_neighbor(unsigned char  **arr, int width, int height, int x, int y);
int tsbound(int val, int max);
unsigned char **toruslife(unsigned char  **arr, unsigned char  **temp, int width, int height);
int klybound(int y, int ymax, int x, int xmax);
unsigned char **klien_life(unsigned char  **arr, unsigned char  **temp, int width, int height);
int klien_neighbor(unsigned char  **arr, int width, int height, int x, int y);
int sizecheck(int size);
unsigned char **randboard(unsigned char  **arr, int width, int height);
int game_type(char *s);
unsigned char **board_write(unsigned char **board, int x, int y, FILE *fp);
unsigned char **five_board_write(unsigned char **board, int x, int y, FILE *fp);
// unsigned char **fp_type(unsigned char **board, int x, int y, FILE *fp, char *s);

#endif
