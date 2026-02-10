#include "back.h"

int **generateFigure(int *figure_size) {
  const int I_type[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  const int J_type[] = {1, 0, 0, 1, 1, 1, 0, 0, 0};
  const int L_type[] = {0, 0, 1, 1, 1, 1, 0, 0, 0};
  const int O_type[] = {1, 1, 1, 1};
  const int S_type[] = {0, 1, 1, 1, 1, 0, 0, 0, 0};
  const int T_type[] = {0, 1, 0, 1, 1, 1, 0, 0, 0};
  const int Z_type[] = {1, 1, 0, 0, 1, 1, 0, 0, 0};

  const struct tetrino tetrinos[] = {{I_type, 4}, {J_type, 3}, {L_type, 3},
                                     {O_type, 2}, {S_type, 3}, {T_type, 3},
                                     {Z_type, 3}};
  srand(time(NULL));
  int type = rand() % 7;
  int **block = createBlock(tetrinos[type]);
  *figure_size = tetrinos[type].size;
  return block;
}

int **createBlock(const struct tetrino tetrinos) {
  int **block;
  int size = tetrinos.size;
  block = createMatrix(size, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      block[i][j] = tetrinos.type[i * size + j];
    }
  }
  return block;
}

int **createMatrix(int h, int w) {
  int **matrix = (int **)malloc(h * sizeof(int *));
  for (int i = 0; i < h; i++) {
    int *row = (int *)malloc(w * sizeof(int));
    matrix[i] = row;
    for (int j = 0; j < w; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

void freeMatrix(int **matrix, int size) {
  for (int i = 0; i < size; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

void copyMatrix(int **src, int **dst, int h, int w) {
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      dst[i][j] = src[i][j];
    }
  }
}