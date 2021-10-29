#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Asteroids;
class Bullets;
class OpenGLWindow;
class StarLayers;

class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);
  void setRotation(float rotation) { m_rotation = rotation; }
  void setPosition(glm::vec2 position) { m_position = position; }

 private:
  friend Asteroids;
  friend Bullets;
  friend OpenGLWindow;
  friend StarLayers;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};
  GLint m_positionLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{1};
  float m_rotation{};
  float m_scale{0.125f};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};
  glm::vec2 m_position {glm::vec2{0, -6.5}};

  abcg::ElapsedTimer m_trailBlinkTimer;
  abcg::ElapsedTimer m_bulletCoolDownTimer;
};
#endif