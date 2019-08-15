#include "Renderer.hpp"
#include <unistd.h>

int main (void) {
  Renderer renderer;
  State_St gameState;
  bool running = true;

  gameState.win = renderer.drawWin(800, 600, "bomber man");
  renderer.init(gameState);

  gameState.map[0] = 21;
  gameState.map[1] = 11;

  for (int i = 0; i < gameState.map[0]; i++) {
    if (i % 2) {
      for (int j = 0; j < gameState.map[1]; j++) {
        if (j % 2) {
          Environment_St* myEnv = new Environment_St;
          myEnv->coord[0] = i;
          myEnv->coord[1] = j;
          myEnv->envType = unbreakable;
          gameState.environments.push_back(*myEnv);
        }
      }
    }
  }

  for (int i = 0; i < gameState.map[0]; i++) {
    for (int j = 0; j < gameState.map[1]; j++) {
      if ((j % 2 == 0 || i % 2 == 0) && rand()%100 < 40) {
        Environment_St myEnv;
        myEnv.coord[0] = i;
        myEnv.coord[1] = j;
        myEnv.envType = breakable;
        gameState.environments.push_back(myEnv);
      }
    }
  }

  Mob_st myMob;
  myMob.coord[0] = 0;
  myMob.coord[1] = 0;
  myMob.displacement[0] = 0;
  myMob.displacement[1] = 0;
  myMob.mobType = player;
  gameState.mobs.push_back(myMob);

  while (running) {
    int input = renderer.getInput(gameState);
    switch (input)
    {
    case 0:
        gameState.mobs.at(0).displacement[1] -= 0.1;
      break;
    case 1:
        gameState.mobs.at(0).displacement[0] += 0.1;
      break;
    case 2:
        gameState.mobs.at(0).displacement[1] += 0.1;
      break;
    case 3:
        gameState.mobs.at(0).displacement[0] -= 0.1;
      break;
    case 4:
        running = false;
      break;
    
    default:
      break;
    }
    renderer.render(gameState);
    usleep(100);
  }
  renderer.closeWindow(gameState);

  return (0);
}