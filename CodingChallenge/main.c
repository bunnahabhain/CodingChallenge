#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DEAD 0
#define ALIVE 1

#define NUM_ROWS 9
#define NUM_COLUMNS 9

#define BOUNDS_LOW 0
#define BOUNDS_HIGH 8

//#define DEBUG_ITERATIONS
//#define DEBUG_NEIGHBOURS
//#define DEBUG_VALID

// Function prototypes:
int number_of_alive_neighbours(int row, int column);
bool is_valid(int row, int column, int neighbour_row, int neighbour_column);
void print_board(int iteration, int board[NUM_ROWS][NUM_COLUMNS]);

// NOTE: will have to change the seed data if number or rows or columns changes.
int input_board[NUM_ROWS][NUM_COLUMNS] = {
  { 0,0,1,0,0,0,0,0,0 },
  { 0,0,0,1,0,0,0,0,0 },
  { 0,1,1,1,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,0,0 }
  };

int output_board[NUM_ROWS][NUM_COLUMNS] = {0};

int main(int argc, const char * argv[]) {
  int num_alive_neighbours = 0;

  print_board(0, input_board);

  for (int iterations = 1; iterations <= 10; iterations++) {
    for (int row = 0; row < NUM_ROWS; row++) {
      #ifdef DEBUG_ITERATIONS
      printf("row = %d\n", row);
      #endif
      for (int column = 0; column < NUM_COLUMNS; column++) {
        #ifdef DEBUG_ITERATIONS
        printf("column = %d\n", column);
        #endif

        num_alive_neighbours = number_of_alive_neighbours(row, column);
        #ifdef DEBUG_ITERATIONS
        printf("number of alive neighbours = %d, for cell at row: %d, column: %d\n", num_alive_neighbours, row, column);
        #endif

        if (input_board[row][column] == ALIVE) {
          #ifdef DEBUG_ITERATIONS
          printf("Cell at row: %d, column %d was alive!\n", row, column);
          #endif
          if (num_alive_neighbours < 2) {
            // cell dies due to underpopulation
            output_board[row][column] = DEAD;
            #ifdef DEBUG_ITERATIONS
            printf("Cell at row: %d, column: %d died\n", row, column);
            #endif
          }
          else if (num_alive_neighbours == 2 || num_alive_neighbours == 3) {
            // cell survives due to 2 or 3 live neighbours
            output_board[row][column] = ALIVE;
            #ifdef DEBUG_ITERATIONS
            printf("Cell at row: %d, column: %d survives\n", row, column);
            #endif
          }
          else if (num_alive_neighbours > 3) {
            // cell dies due to overpopulation
            output_board[row][column] = DEAD;
            #ifdef DEBUG_ITERATIONS
            printf("Cell at row: %d, column: %d died due to overpopulation\n", row, column);
            #endif
          }
          else {
            #ifdef DEBUG_ITERATIONS
            printf("Cell continues to live!\n");
            #endif
          }
        }
        else {
          #ifdef DEBUG_ITERATIONS
          printf("Cell at row: %d, column %d was dead.\n", row, column);
          #endif
          if (num_alive_neighbours == 3) {
            output_board[row][column] = ALIVE;
            #ifdef DEBUG_ITERATIONS
            printf("Cell at row: %d, column: %d reincarnated\n", row, column);
            #endif
          }
          #ifdef DEBUG_ITERATIONS
          printf("Cell remains dead.\n");
          #endif
        }
        #ifdef DEBUG_ITERATIONS
        printf("Cell at row: %d, column: %d has value: %d\n\n", row, column, output_board[row][column]);
        #endif
      }
    }
    memcpy(input_board, output_board, (sizeof(input_board)));
    print_board(iterations, output_board);
  }

  return 0;
}

void print_board(int iteration, int board[NUM_ROWS][NUM_COLUMNS]) {
  printf("Iteration %d\n", iteration);
  printf("[\n");
  for (int row = 0; row < NUM_ROWS; row++) {
    printf("  [");
    for (int column = 0; column < NUM_COLUMNS; column++) {
      printf("%d", board[row][column]);
      if (column != 8) printf(",");
    }
    printf("],\n");
  }
  printf("]\n");
}

int number_of_alive_neighbours(int row, int column)
{
  int alive = 0;

  for (int neighbour_row = row - 1; neighbour_row <= row + 1; neighbour_row++) {

    #ifdef DEBUG_NEIGHBOURS
    printf("\nneighbour_row = %d\n", neighbour_row);
    #endif
    
    for (int neighbour_column = column - 1; neighbour_column <= column + 1; neighbour_column++) {
      
      #ifdef DEBUG_NEIGHBOURS
      printf("neighbour_column = %d\n\n", neighbour_column);
      #endif
      
      if (is_valid(row, column, neighbour_row, neighbour_column)) {
        #ifdef DEBUG_NEIGHBOURS
        printf("neighbour is valid\n");
        printf("input_board neighbour_row: %d, neighbour_column: %d == %d\n", neighbour_row, neighbour_column, input_board[neighbour_row][neighbour_column]);
        #endif
        if (input_board[neighbour_row][neighbour_column] == ALIVE) {
          alive++;
          #ifdef DEBUG_NEIGHBOURS
          printf("Incremented alive\n");
          #endif
        }
      }
      else {
        #ifdef DEBUG_NEIGHBOURS
        printf("neighbour is invalid\n");
        #endif
        continue;
      }
    }
  }

  return alive;
}

bool is_valid(int row, int column, int neighbour_row, int neighbour_column) {
  bool result = true;

  if (neighbour_row == row && neighbour_column == column) {
      // This is the current cell, skip it.
      #ifdef DEBUG_VALID
      printf("skip yourself\n");
      #endif
      result = false;
  }

  else if ((neighbour_row < BOUNDS_LOW) || (neighbour_column < BOUNDS_LOW)) {
      // This is out of bounds
      #ifdef DEBUG_VALID
      printf("skip out of bounds -- low side\n");
      #endif
      result = false;
  }

  else if ((neighbour_row > BOUNDS_HIGH) || (neighbour_column > BOUNDS_HIGH)) {
      // This is out of bounds
      #ifdef DEBUG_VALID
      printf("skip out of bounds -- high side\n");
      #endif
      result = false;
  }

  return (result);
}
