#include "state.h"

#include "utils.h"

typedef struct {
  enum State currState;
  enum State prevState;
  int currDist;
} state;

state s = {
    .currState = FORWARD,
    .prevState = FORWARD,
    .currDist = -1,
};

void SetDist(int newDist) {
  if (s.currState == FORWARD && newDist < 10 && s.currDist >= 10) {
    Logger(DEBUG, "See a wall");
    SetCurrState(STOP);
  }

  if (s.currState == SPIN && newDist > 10 && s.currDist <= 10) {
    Logger(DEBUG, "No wall seen");
    SetCurrState(STOP);
  }

  s.currDist = newDist;
}

enum State GetCurrState() { return s.currState; }

enum State GetPrevState() { return s.prevState; }

void SetCurrState(enum State newState) {
  s.prevState = s.currState;
  s.currState = newState;
}
