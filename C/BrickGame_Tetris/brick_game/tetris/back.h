#ifndef BACK_H
#define BACK_H

#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define HIGHT 20
#define WIDTH 10
#define SPEED 800
#define SPEED_STEP 60
#define MAX_LVL 10

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Initial,
  Moving,
  GameOver,
} status_t;

typedef struct {
  status_t status;
  int **field;
  int **block;
  int size;
  int **next_block;
  int next_size;
  int x_pos;
  int y_pos;
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
  bool h;
  unsigned long long start_time;
  unsigned long long time_left;
} state_t;


typedef struct tetrino {
	const int *type;
	const int size;
} tetrino;

int **generateFigure(int *figure_size);
int **createBlock(struct tetrino tetrinos);
int **createMatrix(int h, int w);
void freeMatrix (int **matrix, int size);
void copyMatrix(int **src, int **dst, int h, int w);

void initState();
state_t *getCurrentState();
GameInfo_t *getGameInfo();
GameInfo_t updateCurrentState();
void userInput(UserAction_t action, bool hold);
void startGame();
unsigned long long currTime();
bool processTime();
bool isAttaching(state_t *state, int new_x, int new_y, int **block);
void shiftLeft();
void shiftRight();
void rotateFigure();
void attachBlock();
void spawnNewBlock();
void deleteRow();
void updateScore(int count);
void getMaxScore(int score);
void updateLevel();
void finishGame();

#endif