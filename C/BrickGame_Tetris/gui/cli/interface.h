#ifndef INTERFACE_H
#define INTERFACE_H

#include "../../brick_game/tetris/back.h"

typedef struct {
  WINDOW *game;
  WINDOW *info;
} Interface_t;

Interface_t initWindows();
void getAction (UserAction_t *action, bool *hold);
void render(GameInfo_t game, Interface_t ui);
void finishNcurses(Interface_t interface);

#endif