/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* UserInterface class definition
*/

#include <Blackjack.h>
#include <UserInterface.h>
#include <Log.h>
#include <Window.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


UserInterface::UserInterface() { }

UserInterface& UserInterface::get()
{
  static UserInterface ui;
  return ui;
}

void UserInterface::set(Blackjack* blackjack)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  float width = (float)blackjack->window.width;
  float height = (float)blackjack->window.height;
  ImVec2 windowSize = { width, height };
  ImGui::SetNextWindowSize(windowSize);
  ImGui::SetNextWindowPos({ 0.0f, 0.0f });
  ImGui::Begin("Main", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Game"))
    {
      if (ImGui::MenuItem("Start New"))
      {
        // reset black jack
        blackjack->newGame = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  static Player& playerOne = blackjack->players.front();
  
  if (ImGui::Button("Hit", { width * 0.485f, height * 0.2f }))
  {
    playerOne.action = Action::HIT;
  }
  ImGui::SameLine(0.0f);
  if (ImGui::Button("Stand", { width * 0.485f, height * 0.2f }))
  {
    playerOne.action = Action::STAND;
  }
  ImGui::Separator();

  ImGui::BeginChild("Log", { width * 0.485f, height * 0.7f });
  Log::draw();
  
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
  {
    ImGui::SetScrollHereY(1.0f);
  }

  ImGui::EndChild();

  // ImGui::ShowDemoWindow();

  ImGui::End();
}

void UserInterface::draw()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::init(Window* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window->pWinGLFW, true);
  ImGui_ImplOpenGL3_Init();
}

void UserInterface::terminate()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}