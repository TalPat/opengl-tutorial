#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer(/* args */) {
}

Renderer::~Renderer() {

}

sf::Window* Renderer::drawWin(int width, int height, std::string title) {
  // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
  //   std::cout << "SDL could not initialize. Error: " << SDL_GetError() << std::endl;
  // } else {
  //   return (SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL));
  // }
  // return (NULL);

  // glfwInit();
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // #ifdef __APPLE__
  //   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // #endif

  //CREATE WINDOW OBJECT
  _width = width;
  _height = height;

  sf::ContextSettings settings;
  settings.depthBits = 32;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.antialiasingLevel = 4;
  settings.attributeFlags = sf::ContextSettings::Core;

  sf::Window* window = new sf::Window(sf::VideoMode(width, height), title, sf::Style::Default, settings);//glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cout << "failed to create window" << std::endl;
    // glfwTerminate();
    throw ("window initialization error");
  }
  window->setActive();

  // glfwMakeContextCurrent(window);
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cout << "failed to init GLEW: " << glewGetErrorString(err) << std::endl;
    // glfwTerminate();
    throw ("GLEW failed");
  }
  std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, width, height);

  return (window);
}

void Renderer::init(State_St gameState) {
  //Load objects into vram
  Model_st testModel1;
  Model_st testModel2;
  Model_st testModel3;
  // testModel.model = new Model("../res/models/IronMan/IronMan.obj");
  testModel2.model = new Model("../res/models/nanosuit/nanosuit.obj");
  testModel1.model = new Model("../res/models/wall/wall.obj");
  testModel3.model = new Model("../res/models/box/wall.obj");
  testModel1.envType = unbreakable;
  testModel1.type = "environment";
  testModel3.envType = breakable;
  testModel3.type = "environment";

  _models.push_back(testModel1);

  _models.push_back(testModel3);
  testModel2.mobType = player;
  testModel2.type = "mob";
  _models.push_back(testModel2);

  //compile shader programs
  _shader = new Shader("../res/shaders/vertexShader.glsl", "../res/shaders/fragmentShader.glsl");

  //build camera
  _camera = new Camera(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f);

  //Wireframing
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::render(State_St gameState) {

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _shader->use();
  glm::mat4 projection = glm::perspective(glm::radians(_camera->getZoom()), (float)_width/(float)_height, 0.1f,100.0f);
  glm::mat4 view = _camera->getViewMatrix();
  _shader->setMat4("projection", projection);
  _shader->setMat4("view", view);


  //Draw environments
  for (auto& it : gameState.environments) {
    glm::mat4 model = glm::mat4(1.0f);
    switch (it.envType) {
      case breakable:
        //do translations and scaling
        /**/ model = glm::translate(model, glm::vec3((float)it.coord[0], -0.8f, (float)it.coord[1])); // translate it down so it's at the center of the scene
        /**/ model = glm::scale(model, glm::vec3(0.5));	// it's a bit too big for our scene, so scale it down
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        _shader->setMat4("model", model);
        _models[1].model->draw(*_shader);
      break;
      case unbreakable:
        //do translations and scaling
        /**/ model = glm::translate(model, glm::vec3((float)it.coord[0], -1.75f, (float)it.coord[1])); // translate it down so it's at the center of the scene
        /**/ model = glm::scale(model, glm::vec3(0.5));	// it's a bit too big for our scene, so scale it down
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        _shader->setMat4("model", model);
        _models[0].model->draw(*_shader);
      break;
      case levelExit:
      break;
      default:
      break;
    }

  }

  glm::mat4 model = glm::mat4(1.0f);
  /**/ model = glm::translate(model, glm::vec3(gameState.mobs[0].displacement[0], -1.5f, gameState.mobs[0].displacement[1])); // translate it down so it's at the center of the scene
  /**/ model = glm::scale(model, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
  // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  _shader->setMat4("model", model);
  _models[2].model->draw(*_shader);

  // _shader->setMat4("model", model);
  // _models[0].model->draw(*_shader);
  _camera->setPosition(glm::vec3(gameState.mobs[0].displacement[0], 4.0f, gameState.mobs[0].displacement[1] + 4.0f));
  _camera->setYaw(270.0f);
  _camera->setPitch(-45.0f);

  gameState.win->display();// glfwSwapBuffers(gameState.win);
}

void Renderer::closeWindow(State_St gameState) {
  std::cout << "Window Closed!" << std::endl;
  // SDL_DestroyWindow(gameState.win);
  // SDL_Quit();
  // glfwTerminate();
}

int Renderer::getInput(State_St gameState) {

  // glfwPollEvents();
  // if (glfwGetKey(gameState.win, GLFW_KEY_W) == GLFW_PRESS) {
  //   return (0);
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_S) == GLFW_PRESS) {
  //   return (2);
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_A) == GLFW_PRESS) {
  //   _camera->setYaw(_camera->getYaw()-1);
  //   return (3);
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_D) == GLFW_PRESS) {
  //   _camera->setYaw(_camera->getYaw()+1);
  //   return (1);
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
  //   return (4);
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_UP) == GLFW_PRESS) {
  //   _camera->setPosition(_camera->getPosition() + glm::vec3(-0.1f, 0.0f, 0.0f));
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_DOWN) == GLFW_PRESS) {
  //   _camera->setPosition(_camera->getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_LEFT) == GLFW_PRESS) {
  //   _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, 0.0f, 0.1f));
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
  //   _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, 0.0f, -0.1f));
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_1) == GLFW_PRESS) {
  //   _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
  // } else if (glfwGetKey(gameState.win, GLFW_KEY_2) == GLFW_PRESS) {
  //   _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, -0.1f, 0.0f));
  // } else
  // return (-1);
  sf::Event event;
  gameState.win->pollEvent(event);
    // check the type of the event...
    switch (event.type)
    {

      // key pressed
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::W) {
          return (0);
        } else if (event.key.code == sf::Keyboard::S) {
          return (2);
        } else if (event.key.code == sf::Keyboard::A) {
          // _camera->setYaw(_camera->getYaw()-1);
          return (3);
        } else if (event.key.code == sf::Keyboard::D) {
          // _camera->setYaw(_camera->getYaw()+1);
          return (1);
        } else if (event.key.code == sf::Keyboard::Escape) {
          return (4);
        } else if (event.key.code == sf::Keyboard::Up) {
          _camera->setPosition(_camera->getPosition() + glm::vec3(-0.1f, 0.0f, 0.0f));
        } else if (event.key.code == sf::Keyboard::Down) {
          _camera->setPosition(_camera->getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
        } else if (event.key.code == sf::Keyboard::Left) {
          _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, 0.0f, 0.1f));
        } else if (event.key.code == sf::Keyboard::Right) {
          _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, 0.0f, -0.1f));
        } else if (event.key.code == sf::Keyboard::Num1) {
          _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
        } else if (event.key.code == sf::Keyboard::Num2) {
          _camera->setPosition(_camera->getPosition() + glm::vec3(0.0f, -0.1f, 0.0f));
        } else
        return (-1);
          break;

      // we don't process other types of events
      default:
          return (-1);
        break;
    }
}
