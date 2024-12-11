#include "state.h"

typedef struct {
  enum State curr_state;
  int curr_dist;
} state;

state s = {
    .curr_state = FORWARD,
    .curr_dist = -1,
};

void SetDist(int new_dist) {
  if (new_dist < 10 && s.curr_dist > 10) {
    s.curr_state = STOP;
  }

  s.curr_dist = new_dist;
}

enum State GetState() { return s.curr_state; }
