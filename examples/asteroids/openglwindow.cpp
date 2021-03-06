#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the stars
  m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                        getAssetsPath() + "stars.frag");
  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_starLayers.initializeGL(m_starsProgram, 25);
  m_ship.initializeGL(m_objectsProgram);
  m_asteroids.initializeGL(m_objectsProgram, 3);
  m_bullets.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_ship.update(m_gameData, deltaTime);
  m_starLayers.update(m_ship, deltaTime);
  m_asteroids.update(m_ship, deltaTime);
  m_bullets.update(m_ship, m_gameData, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }

}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_starLayers.paintGL();
  m_asteroids.paintGL();
  m_bullets.paintGL();
  m_ship.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    if (m_gameData.m_state == State::GameOver) {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);  

    ImGui::Text("  Perdeu!");
    ImGui::PopFont();
    }

    else if (m_gameData.m_state == State::Win) {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);  

    ImGui::Text("  Venceu!");
    ImGui::PopFont();


    }

    else {
    const auto size{ImVec2(470, 600)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    //ImGui::PushFont(m_font);  

    //Validacao para exibicao do nome da velocidade
    float validacao_vel =  m_ship.m_velocity.y;
    if (validacao_vel < 0.10f) {
      validacao_vel = 0.0f;
    } else {
      validacao_vel = m_ship.m_velocity.y;   
    }
      ImGui::Text("Velocidade %.2f Km/s", validacao_vel );
    }



    
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_starsProgram);
  abcg::glDeleteProgram(m_objectsProgram);

  m_asteroids.terminateGL();
  m_bullets.terminateGL();
  m_ship.terminateGL();
  m_starLayers.terminateGL();
  
}






void OpenGLWindow::checkCollisions() {
  // Check collision between ship and asteroids
  for (const auto &asteroid : m_asteroids.m_asteroids) {
    const auto asteroidTranslation{asteroid.m_translation};
   // float razao = m_ship.m_scale/asteroid.m_scale;
    //float razao = 0.125/0.25;
    glm::vec2 naveAjustada = glm::vec2(0);
    naveAjustada.x =  m_ship.m_position.x;
    naveAjustada.y =  m_ship.m_position.y;
    glm::vec2 asteroidAjustado = asteroid.m_translation * (m_ship.m_scale/asteroid.m_scale);
    const auto distance{
      
        glm::distance(naveAjustada, asteroidAjustado)};
    if(m_restartWaitTimer.elapsed() > 6)
    {
      if (distance < m_ship.m_scale * 48.20f) 
      {
        m_gameData.m_state = State::GameOver;
        m_restartWaitTimer.restart();
      }
    }
   
  }

  // Check collision between bullets and asteroids
  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_dead) continue;

    for (auto &asteroid : m_asteroids.m_asteroids) {
      for (const auto i : {-2, 0, 2}) {
        for (const auto j : {-2, 0, 2}) {
          const auto asteroidTranslation{asteroid.m_translation +
                                         glm::vec2(i, j)};
          const auto distance{
              glm::distance(bullet.m_translation, asteroidTranslation)};

          if (distance < m_bullets.m_scale + asteroid.m_scale * 1.50f) {
            asteroid.m_hit = true;
            bullet.m_dead = true;
          }
        }
      }
    }

    // Break asteroids marked as hit
    for (const auto &asteroid : m_asteroids.m_asteroids) {
      if (asteroid.m_hit && asteroid.m_scale > 0.10f) {
        std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
        std::generate_n(std::back_inserter(m_asteroids.m_asteroids), 3, [&]() {
          const glm::vec2 offset{m_randomDist(m_randomEngine),
                                 m_randomDist(m_randomEngine)};
          return m_asteroids.createAsteroid(
              asteroid.m_translation + offset * asteroid.m_scale * 0.5f,
              asteroid.m_scale * 0.5f);
        });
      }
    }

    m_asteroids.m_asteroids.remove_if(
        [](const Asteroids::Asteroid &a) { return a.m_hit; });
  }
}

void OpenGLWindow::checkWinCondition() {
  if (m_asteroids.m_asteroids.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}