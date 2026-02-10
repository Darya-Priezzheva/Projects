#include "back.h"

state_t *getCurrentState() {
  static state_t state;
  return &state;
}
GameInfo_t *getGameInfo() {
  static GameInfo_t game;
  return &game;
}

void initState() {
  state_t *state = getCurrentState();
  GameInfo_t *info = getGameInfo();
  state->status = Initial;

  state->field = createMatrix(HIGHT, WIDTH);
  info->field = createMatrix(HIGHT, WIDTH);
  int size;
  state->block = generateFigure(&size);
  state->size = size;
  int next_size;
  state->next_block = generateFigure(&next_size);
  state->next_size = next_size;
  info->next = createMatrix(4, 4);
  state->x_pos = 1;
  state->y_pos = 4;
  state->score = 0;
  state->level = 1;
  state->speed = SPEED;
  state->pause = false;
  state->h = false;
  state->start_time = currTime();
  state->time_left = state->speed;

  int max_score = 0;
  const char *filename = "score.txt";
  FILE *fl = fopen(filename, "r");
  if (fl) {
    fscanf(fl, "%d", &max_score);
    fclose(fl);
  }
  state->high_score = max_score;
}

GameInfo_t updateCurrentState() {
  state_t *state = getCurrentState();
  GameInfo_t *info = getGameInfo();
  unsigned long long now = currTime();
  if (processTime()) {
    if (!isAttaching(state, state->x_pos + 1, state->y_pos, state->block)) {
      state->x_pos++;
    } else {
      attachBlock();
    }
    state->start_time = now;
  }
  copyMatrix(state->field, info->field, HIGHT, WIDTH);
  if (state->status == Moving) {
    for (int i = 0; i < state->size; i++) {
      for (int j = 0; j < state->size; j++) {
        if (state->block[i][j]) {
          int x = state->x_pos + i;
          int y = state->y_pos + j;
          if (x >= 0 && x < HIGHT && y >= 0 && y < WIDTH) {
            info->field[x][y] = 1;
          }
        }
      }
    }
  }
  freeMatrix(info->next, 4);
  info->next = createMatrix(4, 4);
  copyMatrix(state->next_block, info->next, state->next_size, state->next_size);
  info->level = state->level;
  info->score = state->score;
  info->high_score = state->high_score;
  info->speed = state->speed;
  info->pause = state->pause;
  if (state->status == GameOver) info->pause = 2;
  return *info;
}

void userInput(UserAction_t action, bool hold) {
  state_t *state = getCurrentState();
  state->h = hold;
  switch (action) {
    case Start:
      startGame();
      break;
    case Pause:
      state->pause = !state->pause;
      break;
    case Left:
      if (state->status == Moving && !state->pause) shiftLeft();
      break;
    case Right:
      if (state->status == Moving && !state->pause) shiftRight();
      break;
    case Down:
      if (!state->pause) {
        while (
            !isAttaching(state, state->x_pos + 1, state->y_pos, state->block)) {
          state->x_pos++;
        }
        attachBlock();
      }
      break;
    case Action:
      if (state->status == Moving && !state->pause) rotateFigure();
      break;
    case Terminate:
      finishGame();
  }
}

void startGame() {
  state_t *state = getCurrentState();
  if (state->status == Initial) {
    state->status = Moving;
    state->start_time = currTime();
    state->time_left = state->speed;
  }
  spawnNewBlock();
}

unsigned long long currTime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long long)(tv.tv_sec) * 1000 +
         (unsigned long long)(tv.tv_usec) / 1000;
}

bool processTime() {
  state_t *state = getCurrentState();
  if (state->status == Initial || state->pause || state->status == GameOver)
    return false;

  int dif = currTime() - state->start_time;
  return dif >= state->speed;
}

bool isAttaching(state_t *state, int new_x, int new_y, int **block) {
  for (int i = 0; i < state->size; i++) {
    for (int j = 0; j < state->size; j++) {
      if (block[i][j]) {
        int x = new_x + i;
        int y = new_y + j;
        if (x >= HIGHT || y < 0 || y >= WIDTH) return true;
        if (x >= 1 && state->field[x][y]) return true;
      }
    }
  }
  return false;
}

void shiftLeft() {
  state_t *state = getCurrentState();
  int new_y = state->y_pos - 1;
  if (!isAttaching(state, state->x_pos, new_y, state->block)) {
    state->y_pos--;
  }
}

void shiftRight() {
  state_t *state = getCurrentState();
  if (!isAttaching(state, state->x_pos, state->y_pos + 1, state->block)) {
    state->y_pos++;
  }
}

void rotateFigure() {
  state_t *state = getCurrentState();
  int **new_block = createMatrix(state->size, state->size);
  copyMatrix(state->block, new_block, state->size, state->size);
  for (int i = 0; i < state->size; i++) {
    for (int j = 0; j < state->size; j++) {
      int size = state->size;
      new_block[j][size - i - 1] = state->block[i][j];
    }
  }

  if (!isAttaching(state, state->x_pos, state->y_pos, new_block)) {
    copyMatrix(new_block, state->block, state->size, state->size);
  }
  freeMatrix(new_block, state->size);
}

void attachBlock() {
  state_t *state = getCurrentState();
  for (int i = 0; i < state->size; i++) {
    for (int j = 0; j < state->size; j++) {
      if (state->block[i][j]) {
        int x = state->x_pos + i;
        int y = state->y_pos + j;
        if (x >= 0 && x < HIGHT && y >= 0 && y < WIDTH) {
          state->field[x][y] = 1;
        }
        if (x == 1) {
          state->status = GameOver;
        }
      }
    }
  }

  deleteRow();
  spawnNewBlock();
}

void spawnNewBlock() {
  state_t *state = getCurrentState();
  freeMatrix(state->block, state->size);
  state->block = createMatrix(state->next_size, state->next_size);
  state->size = state->next_size;
  copyMatrix(state->next_block, state->block, state->size, state->size);
  int new_size;
  freeMatrix(state->next_block, state->next_size);
  state->next_block = generateFigure(&new_size);
  state->next_size = new_size;
  state->x_pos = 0;
  state->y_pos = WIDTH / 2 - state->size / 2;
}

void deleteRow() {
  state_t *state = getCurrentState();
  int count = 0;
  for (int i = HIGHT - 1; i >= 0; i--) {
    bool full = true;
    for (int j = 0; j < WIDTH && full; j++) {
      if (state->field[i][j] == 0) {
        full = false;
      }
    }
    if (full) {
      count++;
      int k = i;
      for (; k >= 1; k--) {
        for (int j = 0; j < WIDTH; j++) {
          state->field[k][j] = state->field[k - 1][j];
        }
      }
    }
  }
  if (count > 0) updateScore(count);
}

void updateScore(int count) {
  state_t *state = getCurrentState();
  if (count == 1) state->score += 100;
  if (count == 2) state->score += 300;
  if (count == 3) state->score += 700;
  if (count >= 4) state->score += 1500;

  getMaxScore(state->score);
}

void getMaxScore(int score) {
  state_t *state = getCurrentState();
  int max_score = 0;
  const char *filename = "score.txt";
  FILE *fl = fopen(filename, "r");
  if (fl) {
    fscanf(fl, "%d", &max_score);
    fclose(fl);
  }
  max_score = max_score > score ? max_score : score;
  fl = fopen(filename, "w");
  if (fl) {
    fprintf(fl, "%d", max_score);
    fclose(fl);
  }
  state->high_score = max_score;
  updateLevel();
}

void updateLevel() {
  state_t *state = getCurrentState();
  int level = state->score / 600 + 1;
  if (level != 0 && level != state->level && level <= MAX_LVL) {
    state->level = level;
    state->speed -= SPEED_STEP;
    state->time_left -= SPEED_STEP;
  }
}

void finishGame() {
  state_t *state = getCurrentState();
  GameInfo_t *game = getGameInfo();
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
  freeMatrix(game->field, HIGHT);
  freeMatrix(game->next, 4);
}
