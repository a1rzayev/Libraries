#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 6
#define COLUMNS 7
#define EMPTY_SLOT ' '
#define RED_DISC '*'
#define YELLOW_DISC 'o'

char slots[] = {YELLOW_DISC, RED_DISC, EMPTY_SLOT};
bool wantToRestart;
char grid[ROWS][COLUMNS];

void clearConsole() { //clears console
    printf("\033[2J\033[1;1H");
}

void initializeGrid() { //initializes grid at the start of game
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) grid[i][j] = EMPTY_SLOT;
    }
}

void displayGrid() {  //displays grid after each turn
    //print top column numbers
    printf("  1 2 3 4 5 6 7\n");
    for (int i = 0; i < ROWS; ++i) {
        printf("%d ", i + 1); //print row number
        for (int j = 0; j < COLUMNS; ++j) printf("%c ", grid[i][j]); //print grid content
        printf("%d\n", i + 1); //print row number
    }
    //print bottom column numbers
    printf("  1 2 3 4 5 6 7\n\n");
}

bool isMovePossible(int column) { //checks if move is possible
    return column >= 1 && column <= COLUMNS && grid[0][column - 1] == EMPTY_SLOT;
}

void dropDisc(int column, char disc) { //drops disk each turn
    int row = ROWS - 1;
    while (grid[row][column - 1] != EMPTY_SLOT) {
        --row;
    }
    
    grid[row][column - 1] = disc;
}

int countAlignedDisc(char wGrid[ROWS][COLUMNS], int row, int column, char disc) { //count aligned disks(for checking victory)
    int count = 0;
    //check horizontally
    for (int i = column - 3; i <= column + 3; ++i) {
        if (i >= 0 && i + 3 < COLUMNS &&
            wGrid[row][i] == disc &&
            wGrid[row][i + 1] == disc &&
            wGrid[row][i + 2] == disc &&
            wGrid[row][i + 3] == disc) {
            ++count;
        }
    }
    //check vertically
    for (int i = row - 3; i <= row + 3; ++i) {
        if (i >= 0 && i + 3 < ROWS &&
            wGrid[i][column] == disc && 
            wGrid[i + 1][column] == disc &&
            wGrid[i + 2][column] == disc && 
            wGrid[i + 3][column] == disc) {
            ++count;
        }
    }
    //check diagonally (top-left to bottom-right)
    for (int i = -3; i <= 3; ++i) {
        if (row + i >= 0 && row + i + 3 < ROWS &&
            column + i >= 0 && 
            column + i + 3 < COLUMNS &&
            wGrid[row + i][column + i] == disc &&
            wGrid[row + i + 1][column + i + 1] == disc &&
            wGrid[row + i + 2][column + i + 2] == disc && 
            wGrid[row + i + 3][column + i + 3] == disc) {
            ++count;
        }
    }
    //check diagonally (top-right to bottom-left)
    for (int i = -3; i <= 3; ++i) {
        if (row - i >= 0 && row - i - 3 < ROWS && 
            column + i >= 0 && 
            column + i + 3 < COLUMNS &&
            wGrid[row - i][column + i] == disc && 
            wGrid[row - i - 1][column + i + 1] == disc &&
            wGrid[row - i - 2][column + i + 2] == disc && 
            wGrid[row - i - 3][column + i + 3] == disc) {
            ++count;
        }
    }
    return count;
}

int recommendColumn() { //recommends column for bot playing
    srand(time(NULL));
    int column;
    do {
        column = rand() % COLUMNS;
    } while (!isMovePossible(column));
    return column;
}

bool isGameOver(char wGrid[ROWS][COLUMNS]) { //checks if there is a victory(if draw return false)
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            if (wGrid[i][j] != EMPTY_SLOT) {
                char disc = wGrid[i][j];
                if (countAlignedDisc(wGrid, i, j, disc) > 0) return true;
            }
        }
    }
    return false;
}

bool isGridFull() { //checks if grid is full
    for (int i = 0; i < COLUMNS; ++i) {
        if (grid[0][i] == EMPTY_SLOT) return false;
    }
    return true;
}

void setTurn(int *turn){ //sets player after each turn and each game
    *turn = 1 - *turn;
}

int winningMove(char playerSign) { //send the player winning move
    int row;
    char testGrid[ROWS][COLUMNS];
    for (int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) testGrid[i][j] = grid[i][j];
    }
    for (int i = 0; i < COLUMNS; ++i)
    {
        row = ROWS - 1;
        while (testGrid[row][i] != EMPTY_SLOT) {
            --row;
        }
        testGrid[row][i] = playerSign;
        if(isGameOver(testGrid)) return (i + 1);
        testGrid[row][i] = EMPTY_SLOT;
    }
    return 0;
}