/**
 * @file life.c
 * 
 * @brief contains functions for the rules of the game of life.
 * 
 * @details Contains functions for the rules of the game of life. setting up
 * the board, various extra rules, and creating the arrays.
 * 
 * @author Kassidy Maberry
 * 
 * @date 2022/05/03
 * 
 * @todo .05 support
 * 
 * @bug Klien is currently broken
 */


#include "life.h"


/**
 * creates a matrix with a given height and width
 * @param width the width of the array
 * @param height the height of the array
 * @return the newly created array 
 */
unsigned char **init_matrix(int width, int height)
{
        unsigned char  **arr = NULL;
        arr = malloc(width * sizeof(unsigned char *));
        if(!arr){
                return NULL;
        }
        

        int i;
        for(i = 0; i < width; i++){
                arr[i] = malloc(height * sizeof(char));
                if (!arr[i]){
                        for(int j = 0; j < i; j++){
                                free(arr[j]);
                        }
                        return NULL;
                } 
        }

        for(int i = 0; i < width; i++){
                for(int k = 0; k < height; k++){
                        arr[i][k] = 0;
                }
        }

        return arr;
}


/**
 * deletes a given array.
 * @param a the array to delete
 * @param height the height of the array
 */
void del_arr(unsigned char  **a, int width)
{
        for(int i = 0; i < width; i++){
                free(a[i]);
        }
        free(a);
}



/**
 * Coppies array a to array b.
 * @param a the original array.
 * @param b the array to be copied.
 * @param width the width of array a.
 * @param height the height of array b.
 * @return the newly copied array b.
 */
unsigned char **arr_cpy(unsigned char  **a, unsigned char  **b, int width, int height)
{
        for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                        b[i][j] = a[i][j];
                }
        }
        return b;
}


/**
 * Updates the current space based upon the required given information.
 * @param a the current array.
 * @param x the current x cord.
 * @param y the current y cord.
 * @param friends how many neighbors the current cord has.
 * @return the newly updated cell's status.
 */
int genupdate(unsigned char  **a, int x, int y, int friends)
{
        if (friends < 2){
                return 0;
        } else if (friends > 3){
                return 0 ;
        } else if (friends == 3 && a[x][y] == 0){
                return 1;
        } else {
                return a[x][y];
        }


        return a[x][y];
}


/**
 * Creates a random life pattern.
 * @param arr the board provided.
 * @param width the width of the board.
 * @param height the height of the board.
 * @return The board with randomly placed life.
 */
unsigned char **randboard(unsigned char  **arr, int width, int height)
{
        for(int i = 0; i < width; i++){
                for (int j = 0; j < height; j++){
                        arr[i][j] = rand() % 2;
                }
        }


        return arr;
}


/**
 * Runs the game of life using the hedge boundary rules
 * @param arr the array the game is played on
 * @param temp a secondary array where the changes are applied
 * @param width the width of the array
 * @param height the height of the array
 * @return the updated array.
 */
unsigned char **hedgelife(unsigned char  **arr, unsigned char  **temp, int width, int height)
{
        int friends;
        for(int x = 1; x < width - 1; x++){
                for(int y = 1; y < height - 1; y++){
                        friends = 0;
                        friends = hedge_neighbor(arr, x, y);
                        temp[x][y] = genupdate(arr, x, y, friends);
                }
        }
        


        return temp;
}


/**
 * Looks for what neighbors are alive or dead under hedge rules.
 * @param arr the current array.
 * @param x the x cord we are checking.
 * @param y the y cord we are checking.
 * @return the neighbors.
 */
int hedge_neighbor(unsigned char  **arr, int x, int y)
{
        int cont = 0;
        for(int xmod = -1; xmod <= 1; xmod++){
                for(int ymod = -1; ymod <= 1; ymod++){
                        if (arr[x + xmod][y + ymod] == 1 &&
                                !(xmod == 0 && ymod == 0)){
                                cont++;
                        }
                }
        }
        return cont;
}

/**
 * Runs the game of life using the torus boundary rules.
 * @param arr the array the game is played on.
 * @param temp a secondary array where the changes are applied.
 * @param width the width of the array.
 * @param height the height of the array.
 * @return the updated array.
 */
unsigned char **toruslife(unsigned char  **arr, unsigned char  **temp, int width, int height)
{
        int friends;
        for(int x = 0; x < width; x++){
                for(int y = 0; y < height; y++){
                        friends = 0;
                        friends = torus_neighbor(arr, width, height, x, y);
                        temp[x][y] = genupdate(arr, x, y, friends);
                }
        }
        


        return temp;
}


/**
 * Looks for what neighbors are alive or dead under torus rules.
 * @param arr the current array.
 * @param x the x cord we are checking.
 * @param y the y cord we are checking.
 * @return the neighbors.
 */
int torus_neighbor(unsigned char  **arr, int width, int height, int x, int y)
{
        int cont = 0;
         
        for(int xmod = -1; xmod <= 1; xmod++){
                for(int ymod = -1; ymod <= 1; ymod++){
                        if (arr[tsbound(x - xmod, width)][tsbound(y - ymod,
                                 height)] == 1 
                                && !(xmod == 0 && ymod == 0)){
                                cont++;
                        }
                }
        }
        return cont;
}


/**
 * Determines if a given cord is out side of bounds and which one.
 * @param width the width of the array.
 * @param height the height of the array.
 * @param x the x cord being checked.
 * @param y the y cord being checked.
 */
int bounds(int width, int height, int x, int y)
{
        if(x < 0 || x >= width){
                return XOUT;
        } else if (y < 0 || y >= height) {
                return YOUT;
        } else {
                return IN;
        }
}

/**
 * Returns an updated location if something is out of bounds. It reads the 
 * opposite side of the screen rather than trying to read a non existent value.
 * @param val the current value to be checked.
 * @param max the maximum possible value.
 * @return the new cord being checked.
 */
int tsbound(int val, int max)
{
        if (val < 0){
                val = max - 1;
        }
        return val % max;
}


/**
 * Runs the game of life using the klien boundary rules.
 * @param arr the array the game is played on.
 * @param temp a secondary array where the changes are applied.
 * @param width the width of the array.
 * @param height the height of the array.
 * @return the updated array.
 */
unsigned char **klien_life(unsigned char  **arr, unsigned char  **temp, int width, int height)
{
        int friends;
        for(int x = 0; x < width; x++){
                for(int y = 0; y < height; y++){
                        friends = 0;
                        friends = klien_neighbor(arr, width, height, x, y);
                        temp[x][y] = genupdate(arr, x, y, friends);
                }
        }
        return temp;
}


/**
 * Looks for what neighbors are alive or dead under klien rules.
 * @param arr the current array.
 * @param x the x cord we are checking.
 * @param y the y cord we are checking.
 * @return the neighbors.
 */
int klien_neighbor(unsigned char  **arr, int width, int height, int x, int y)
{
        int cont = 0;
        int curx;
        int cury;
        for(int xmod = -1; xmod <= 1; xmod++){
                for(int ymod = -1; ymod <= 1; ymod++){
                        curx = xmod + x;
                        cury = ymod + y;
                        if (curx >= width){
                                curx %= width;
                                cury = height - cury;
                        } else if (curx < 0){
                                curx += width;
                                cury = height - cury;
                        }

                        cury = tsbound(cury, height);
                        if (arr[curx][cury] == 1){
                                cont++;
                        }
                        
                }
        }
        return cont;
}


/**
 * Checks if the size is allowed of a given pixel.
 * @param size the pixel size
 * @return the size if it has been updated
 * @return If the program is an accepted size it returns it.
 */
int sizecheck(int size)
{
        switch (size){
                case 2:
                case 4:
                case 8:
                case 16:
                        return size;
                default:
                        return 2;
        }
}


/**
 * Takes in the user's input and turns it into the requested game type.
 * @param s a string containing the user's input.
 * @return the updated game type.
 */
int game_type(char *s)
{
        char z;
        sscanf(s, "%c", &z);
        switch (z)
        {
                case 't':
                case 'T':
                        return TORUS;
                case 'K':
                case 'k':
                        return KLIEN;
                case 'H':
                case 'h':
                default:
                        return HEDGE;
        }
}


/**
 * Writes a given 1.06 life file into the board.
 * @param board the board we are writing to
 * @param x the initial x cord.
 * @param y the initial y cord.
 * @param fp the file we are reading from.
 * @return the updated board.
 */
unsigned char **board_write(unsigned char **board, int x, int y, FILE *fp)
{
        int modx, mody;
        int prevx, prevy;
        char buff[255];
        fscanf(fp, "%s %s", buff);
        while(1)
        {
                fscanf(fp, "%d %d", &modx, &mody);
                board[y + mody][x + modx] = 1;
                if (prevx == modx && prevy == mody){
                        break;
                }
                prevx = modx;
                prevy = mody;

        }
        return board;
}


/**
 * Writes to a board in .05 file type.
 * @param board the board to write to.
 * @param x the x location we are starting from.
 * @param y the y location we are starting from.
 * @param fp the file we are reading from.
 * @return the updated board that has been written to.
 */
unsigned char **five_board_write(unsigned char **board, int x, int y, FILE *fp)
{       
        char cur, prev;
        cur = '\0';
        while(1){
                prev = cur;
                fscanf(fp, "%c", &cur);
                if (cur == 'P' && prev == '#'){
                        break;
                }
        }
        int modx, mody;
        fscanf(fp, "%d %d", &modx, &mody);
        modx += x;
        mody += y;
        int j = 0;
        int i = 0;
        while(cur != '\0'){
                fscanf(fp, "%c", &cur);
                if (cur == '*'){
                        board[y + j][x + i] = 1;
                } else if (cur == 10){
                        j++;
                        i = -1;
                }
                if (cur == prev && prev == 10){
                        break;
                }
                i++;
                prev = cur;
        }
        return board;
}


