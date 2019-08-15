#ifndef RENDERER_HPP
#define RENDERER_HPP

// #include <SDL2/SDL.h>
#include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "GameState.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"

struct Model_st {
  Model* model;
  std::string type;
  MobType mobType;
  EnvType envType;
};

class Renderer
{
private:
  std::vector<Model_st> _models;
  Shader* _shader;
  Camera* _camera;
  int _width, _height;
public:
  Renderer(/* args */);
  ~Renderer();
  sf::Window* drawWin(int width, int height, std::string title);
  void init(State_St gameState);
  void render(State_St gameState);
  void closeWindow(State_St gameState);
  int getInput(State_St gameState);
};


#endif