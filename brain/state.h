#ifndef STATE_H
#define STATE_H

enum State {
  FORWARD,
  STOP,
  SPIN,
};

void SetDist(int newDist); 

enum State GetCurrState();

enum State GetPrevState();

void SetCurrState(enum State newState);

#endif
