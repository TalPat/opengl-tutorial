#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
// #include <SDL2/SDL.h>
// #include <GLFW/glfw3.h>
#include <SFML/Window.hpp>

enum MobType {player, balloon, robot, bomb};
enum EnvType {breakable, unbreakable, levelExit};

struct Mob_st {
  int coord[2];
  float displacement[2];
  float orientation;
  MobType mobType;
};

struct Environment_St {
  int coord[2];
  EnvType envType;
};

struct State_St {
  int map[2];
  std::vector<Mob_st> mobs;
  std::vector<Environment_St> environments;
  sf::Window *win;
};

#endif