/*
* Author: Thomas Moreno Cooper
* Date Created: 02/01/2022
* Copyright 2022 under MIT License
*
* UserInterface class definition
*/

#include <Blackjack.h>
#include <BoardRenderer.h>
#include <UserInterface.h>
#include <Log.h>
#include <Window.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


UserInterface::UserInterface() {}

UserInterface& UserInterface::get()
{
  static UserInterface ui;
  return ui;
}

void UserInterface::set(Blackjack* blackjack, BoardRenderer* renderer)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
    
  Vector2 windowSize = blackjack->window.getWindowSize();
  ImGui::SetNextWindowSize({ windowSize[0], windowSize[1] });
  ImGui::SetNextWindowPos({ 0.0f, 0.0f });
  ImGui::Begin("Main", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Game"))
    {
      if (ImGui::MenuItem("Start New"))
      {
        blackjack->newGame = true;
      }

      if (blackjack->showHands)
      {
        if (ImGui::MenuItem("Hide all cards"))
        {
          blackjack->showHands = false;
          renderer->drawBoard(blackjack, false);
        }
      }
      else if (ImGui::MenuItem("View all cards"))
      {
        blackjack->showHands = true;
        renderer->drawBoard(blackjack, true);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  static Player& playerOne = blackjack->players.front();
  ImVec2 region = ImGui::GetContentRegionAvail();
  if (ImGui::Button("Hit", { region.x * 0.5f, region.y * 0.2f }))
  {
    playerOne.action = Action::HIT;
  }
  ImGui::SameLine();
  if (ImGui::Button("Stand", { region.x * 0.5f, region.y * 0.2f }))
  {
    playerOne.action = Action::STAND;
  }
  ImGui::Separator();

  region = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("Log", { region.x * 0.3f, region.y });

  Log::draw();
  
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
  {
    ImGui::SetScrollHereY(1.0f);
  }

  ImGui::EndChild();
  
  ImGui::SameLine();

  ImGui::BeginChild("Board", { region.x * 0.7f, region.y});

  float width = ImGui::GetWindowWidth();
  float numPlayers = (float)blackjack->players.size();
  ImGui::InvisibleButton("Padding1", ImVec2(width / numPlayers * 0.5f, 0.5f));
  
  ImGui::SameLine();

  ImGui::PushItemWidth(width * 0.9f);
  if (ImGui::BeginTable("player names", (int)blackjack->players.size()-1))
  {
    ImGui::TableNextRow();
    for (int i = 1; i < blackjack->players.size(); ++i)
    {
      ImGui::TableSetColumnIndex(i - 1);
      ImGui::Text("P%i", i + 1);
    }
    ImGui::EndTable();
  }
  ImGui::Image((void*)(intptr_t)renderer->boardFramebuffer.colourBuffer,
    { region.x * 0.7f, region.y * 0.34f }, { 0.0f, 0.0f }, { 1.0f, -1.0f / 3.0f});

  // dealer
  ImGui::InvisibleButton("Padding2", ImVec2(width * 0.45f, 0.5f));
  ImGui::SameLine();
  ImGui::Text("Dealer");
  ImGui::Image((void*)(intptr_t)renderer->boardFramebuffer.colourBuffer, 
    { region.x * 0.7f, region.y * 0.24f }, { 0.0f, -1.15f / 3.0f }, { 1.0f, -1.85f / 3.0f });

  // Player
  ImGui::InvisibleButton("Padding3", ImVec2(width * 0.45f, 0.5f));
  ImGui::SameLine();
  ImGui::Text("Player 1");
  ImGui::Image((void*)(intptr_t)renderer->boardFramebuffer.colourBuffer,
    { region.x * 0.7f, region.y * 0.3f }, { 0.0f, -2.0f / 3.0f }, { 1.0f, -1.0f });

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