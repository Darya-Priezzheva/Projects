#include <check.h>

#include "brick_game/tetris/back.h"
#include "brick_game/tetris/figures.c"
#include "brick_game/tetris/logic.c"

START_TEST(initState_test) {
  initState();

  state_t *state = getCurrentState();
  GameInfo_t *game = getGameInfo();
  initState();

  ck_assert_ptr_nonnull(state->field);
  ck_assert_ptr_nonnull(state->block);
  ck_assert_ptr_nonnull(state->next_block);
  ck_assert_int_eq(state->pause, false);

  ck_assert_ptr_nonnull(game->field);
  ck_assert_ptr_nonnull(game->next);
  ck_assert_int_eq(game->pause, 0);
}
END_TEST

START_TEST(updateCurrentState_test) {
  state_t *state = getCurrentState();
  GameInfo_t *game = getGameInfo();
  initState();
  state->level = 3;
  state->pause = true;
  *game = updateCurrentState();
  ck_assert_int_eq(state->level, game->level);
  ck_assert_int_eq(state->pause, game->pause);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(startGame_test) {
  state_t *state = getCurrentState();
  initState();
  startGame();
  ck_assert_int_eq(state->status, Moving);
  ck_assert_int_eq(state->time_left, state->speed);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(isAttaching_false_test) {
  state_t *state = getCurrentState();
  initState();
  int size = 0;
  state->block = generateFigure(&size);
  state->size = size;
  state->x_pos = 3;
  state->y_pos = 3;

  bool att = isAttaching(state, state->x_pos + 1, state->y_pos, state->block);
  ck_assert_int_eq(att, false);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(isAttaching_true_test) {
  state_t *state = getCurrentState();
  initState();
  int size = 0;
  state->block = generateFigure(&size);
  state->size = size;
  state->x_pos = 19;
  state->y_pos = 3;

  bool att = isAttaching(state, state->x_pos + 1, state->y_pos, state->block);
  ck_assert_int_eq(att, true);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(shiftLeft_test) {
  state_t *state = getCurrentState();
  initState();
  state->x_pos = 13;
  state->y_pos = 5;
  shiftLeft();
  ck_assert_int_eq(state->y_pos, 4);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(shiftRight_test) {
  state_t *state = getCurrentState();
  initState();
  state->x_pos = 13;
  state->y_pos = 5;
  shiftRight();
  ck_assert_int_eq(state->y_pos, 6);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(rotateFigure_test) {
  state_t *state = getCurrentState();
  initState();
  state->size = 3;
  state->block = createMatrix(3, 3);

  int temp[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      state->block[i][j] = temp[i][j];
    }
  }
  rotateFigure();
  int rot[3][3] = {{0, 1, 0}, {0, 1, 1}, {0, 1, 0}};
  for (int i = 0; i < state->size; i++) {
    for (int j = 0; j < state->size; j++) {
      ck_assert_int_eq(state->block[i][j], rot[i][j]);
    }
  }
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(attachBlock_test) {
  state_t *state = getCurrentState();
  initState();
  state->block = generateFigure(&state->size);
  int **f = createMatrix(state->size, state->size);
  copyMatrix(state->block, f, state->size, state->size);
  state->x_pos = 11;
  state->y_pos = 2;
  attachBlock();

  for (int i = 0; i < state->size; i++) {
    for (int j = 0; j < state->size; j++) {
      if (f[i][j]) {
        ck_assert_int_eq(state->field[i + 11][j + 2], 1);
      }
    }
  }
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
  freeMatrix(f, state->size);
}
END_TEST

START_TEST(spawnNewBlock_test) {
  state_t *state = getCurrentState();
  initState();
  spawnNewBlock();
  ck_assert_ptr_nonnull(state->next_block);
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(deleteRow_test) {
  state_t *state = getCurrentState();
  initState();

  for (int i = 0; i < 10; i++) {
    state->field[19][i] = 1;
  }
  deleteRow();
  for (int i = 0; i < 10; i++) {
    ck_assert_int_eq(state->field[19][i], 0);
  }
  freeMatrix(state->block, state->size);
  freeMatrix(state->next_block, state->next_size);
  freeMatrix(state->field, HIGHT);
}
END_TEST

START_TEST(updateScore_test) {
  state_t *state = getCurrentState();
  initState();
  updateScore(3);
  ck_assert_int_eq(state->score, 700);
  ck_assert_int_eq(state->level, 2);
  finishGame();
}
END_TEST

START_TEST(userInput_test) {
  state_t *state = getCurrentState();
  initState();
  userInput(Start, false);
  ck_assert_int_eq(state->status, 1);
  userInput(Pause, false);
  ck_assert_int_eq(state->pause, 1);
  userInput(Pause, false);
  ck_assert_int_eq(state->pause, 0);
  state->x_pos = 10;
  state->y_pos = 4;
  state->status = Moving;
  userInput(Left, false);
  ck_assert_int_eq(state->y_pos, 3);
  userInput(Right, false);
  ck_assert_int_eq(state->y_pos, 4);
  finishGame();
}
END_TEST

Suite *TestGmeSuite() {
  Suite *s = suite_create("Game_test");
  TCase *tc = tcase_create("CoreGame_test");

  tcase_add_test(tc, initState_test);
  tcase_add_test(tc, updateCurrentState_test);
  tcase_add_test(tc, startGame_test);
  tcase_add_test(tc, isAttaching_false_test);
  tcase_add_test(tc, isAttaching_true_test);

  tcase_add_test(tc, shiftLeft_test);
  tcase_add_test(tc, shiftRight_test);
  tcase_add_test(tc, rotateFigure_test);
  tcase_add_test(tc, attachBlock_test);
  tcase_add_test(tc, spawnNewBlock_test);
  tcase_add_test(tc, deleteRow_test);
  tcase_add_test(tc, updateScore_test);
  tcase_add_test(tc, userInput_test);
  suite_add_tcase(s, tc);
  return s;
}

int main() {
  SRunner *sr = srunner_create(TestGmeSuite());
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  int number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}