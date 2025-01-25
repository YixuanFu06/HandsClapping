#include <filesystem>

#include "../../../game/battle_field.h"
#include "../include/policy.h"

const std::string DATA_PATH = "./data/AI/Idiot/policy.txt";

int main() {
  std::filesystem::path current_path = std::filesystem::current_path();
  std::filesystem::path data_path = current_path / DATA_PATH;
  std::filesystem::create_directories(data_path.parent_path());
  Game::InitActions();
  AI::Idiot::Policy policy = AI::Idiot::Policy(data_path.string());

  Game::BattleField battle_field({"player", "AI"});
  std::cout << "Game between you and AI " << policy.GetName() << " starts."
            << std::endl;
  while (battle_field.GetMemberNum() > 1) {
    std::cout << std::endl;
    float player_health = battle_field.GetPlayerHealth(0);
    float player_energy = battle_field.GetPlayerEnergy(0);
    float AI_health = battle_field.GetPlayerHealth(1);
    float AI_energy = battle_field.GetPlayerEnergy(1);
    Game::Action *AI_action =
        policy.GetAction(player_health, player_energy, AI_health, AI_energy);
    std::string player_action;
    std::cout << "Enter your action in the next turn: ";
    std::cin >> player_action;
    battle_field.BattleFieldUpdate({player_action, AI_action->GetFormalName()});
  }
  if (battle_field.GetMemberNum() < 1) {
    std::cout << "Game over! No winner!" << std::endl;
  } else if (battle_field.GetPlayerName(0) == "player") {
    std::cout << "Game over! You win!" << std::endl;
  } else {
    std::cout << "Game over and you lose! You cannot even beat an idiot AI!"
              << std::endl;
  }

  return 0;
}