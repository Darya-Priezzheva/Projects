#include <ncurses.h>

#include "interface.h"

int main() {
  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, true);
  noecho();
  timeout(100);

  Interface_t interface = initWindows();
  initState();
  GameInfo_t currInfo = updateCurrentState();
  render(currInfo, interface);
  while (1) {
    UserAction_t action;
    bool hold;
    getAction(&action, &hold);
    if (action == Terminate) {
      userInput(action, hold);
      break;
    }
    userInput(action, hold);
    currInfo = updateCurrentState();
    if (currInfo.pause == 2) {
      timeout(-1);
    }
    render(currInfo, interface);
  }
  finishNcurses(interface);

  endwin();

  return 0;
}

void finishNcurses(Interface_t interface) {
  delwin(interface.game);
  delwin(interface.info);
}

Interface_t initWindows() {
  Interface_t ui;
  ui.game = newwin(HIGHT + 2, 2 * WIDTH + 2, 0, 0);
  ui.info = newwin(HIGHT + 2, WIDTH + 3, 0, 2 * WIDTH + 2);

  if (ui.game == NULL || ui.info == NULL) {
    endwin();
    fprintf(stderr, "Error: Failed to create ncurses windows.\n");
    exit(1);
  }
  return ui;
}

void getAction(UserAction_t *action, bool *hold) {
  int ch = getch();

  *hold = false;
  *action = ERR;

  switch (ch) {
    case '\n':
      *action = Start;
      break;
    case 'p':
      *action = Pause;
      break;
    case 'q':
      *action = Terminate;
      break;
    case KEY_LEFT:
      *action = Left;
      break;
    case KEY_RIGHT:
      *action = Right;
      break;
    case KEY_DOWN:
      *action = Down;
      break;
    case 32:
      *action = Action;
      break;
    default:
      *action = ERR;
      break;
  }
}

void render(GameInfo_t game, Interface_t ui) {
  box(ui.game, 0, 0);
  box(ui.info, 0, 0);

  for (int i = 0; i < HIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int wy = i + 1;
      int wx = j * 2 + 1;
      if (game.field[i][j]) {
        wattron(ui.game, A_REVERSE);
        mvwaddch(ui.game, wy, wx, ' ');
        mvwaddch(ui.game, wy, wx + 1, ' ');
        wattroff(ui.game, A_REVERSE);
      } else {
        mvwaddch(ui.game, wy, wx, ' ');
        mvwaddch(ui.game, wy, wx + 1, ' ');
      }
    }
  }

  mvwprintw(ui.info, 1, 2, "SCORE");
  mvwprintw(ui.info, 2, 2, "%05d", game.score);
  mvwprintw(ui.info, 4, 2, "HI-SCORE");
  mvwprintw(ui.info, 5, 2, "%05d", game.high_score);
  mvwprintw(ui.info, 7, 2, "NEXT");

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int wy = 9 + i;
      int wx = 2 + j * 2;
      if (game.next[i][j]) {
        wattron(ui.info, A_REVERSE);
        mvwaddch(ui.info, wy, wx, ' ');
        mvwaddch(ui.info, wy, wx + 1, ' ');
        wattroff(ui.info, A_REVERSE);
      } else {
        mvwaddch(ui.info, wy, wx, ' ');
        mvwaddch(ui.info, wy, wx + 1, ' ');
      }
    }
  }

  mvwprintw(ui.info, 13, 2, "LEVEL");
  mvwprintw(ui.info, 14, 2, "%02d", game.level);
  if (game.pause == 1) {
    mvwprintw(ui.info, 17, 2, "PAUSE");
  } else if (game.pause == 2) {
    mvwprintw(ui.info, 17, 2, "GAMEOVER");
  }
  wrefresh(ui.game);
  wrefresh(ui.info);
}
