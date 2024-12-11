#ifndef STATE_H
#define STATE_H

enum State {
  FORWARD,
  STOP,
  SPIN,
};

void SetDist(int new_dist); 

enum State GetState(); 

#endif
