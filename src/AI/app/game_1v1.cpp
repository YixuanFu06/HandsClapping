#include "../../game/battle_field.h"
#include "../HDP/include/model.h"
#include "../Idiot/include/policy.h"

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::HDP::FindRootPath()
            << std::endl
            << std::endl;

  std::cout << "Enter the type of the model you want to play with (HDP or Idiot): ";
  std::string model_type;
  std::cin >> model_type;
  if (model_type != "HDP" && model_type != "Idiot") {
    std::cout << "Invalid model type!" << std::endl;
    return 0;
  }

  if (model_type == "HDP") {
    std::cout << "Enter the name of the model: ";
    std::string model_name;
    std::cin >> model_name;
    AI::HDP::Model model = AI::HDP::Model(model_name);

    std::string play_again;

    do {
      Game::BattleField battle_field({"player", model.GetName()});

      std::cout << "Game between you and AI " << model.GetName() << " starts."
                << std::endl;
      while (battle_field.GetMemberNum() > 1) {
        std::cout << std::endl;
        float player_health = battle_field.GetPlayerHealth(0);
        float player_energy = battle_field.GetPlayerEnergy(0);
        float AI_health = battle_field.GetPlayerHealth(1);
        float AI_energy = battle_field.GetPlayerEnergy(1);
        Game::Action *AI_action =
            model.GetAction(player_health, AI_health, player_energy, AI_energy);
        std::string player_action;
        std::cout << "Enter your action in the next turn: ";
        std::cin >> player_action;
        battle_field.BattleFieldUpdate(
            {player_action, AI_action->GetFormalName()});
      }

      if (battle_field.GetMemberNum() < 1) {
        std::cout << "Game over! No winner!" << std::endl;
      } else if (battle_field.GetPlayerName(0) == "player") {
        std::cout << "Game over! You win!" << std::endl;
      } else {
        std::cout << "Game over and you lose! You cannot even beat an idiot AI!"
                  << std::endl;
      }

      std::cout << "Play again? (y/n): ";
      std::cin >> play_again;
    } while (play_again == "y");

  } else if (model_type == "Idiot") {
    std::cout << "Enter the name of the model: ";
    std::string model_name;
    std::cin >> model_name;
    AI::Idiot::Policy model = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(model_name));

    std::string play_again;

    do {
      Game::BattleField battle_field({"player", model.GetName()});

      std::cout << "Game between you and AI " << model.GetName() << " starts."
                << std::endl;
      while (battle_field.GetMemberNum() > 1) {
        std::cout << std::endl;
        float player_health = battle_field.GetPlayerHealth(0);
        float player_energy = battle_field.GetPlayerEnergy(0);
        float AI_health = battle_field.GetPlayerHealth(1);
        float AI_energy = battle_field.GetPlayerEnergy(1);
        Game::Action *AI_action =
            model.GetAction(player_health, AI_health, player_energy, AI_energy);
        std::string player_action;
        std::cout << "Enter your action in the next turn: ";
        std::cin >> player_action;
        battle_field.BattleFieldUpdate(
            {player_action, AI_action->GetFormalName()});
      }

      if (battle_field.GetMemberNum() < 1) {
        std::cout << "Game over! No winner!" << std::endl;
      } else if (battle_field.GetPlayerName(0) == "player") {
        std::cout << "Game over! You win!" << std::endl;
      } else {
        std::cout << "Game over and you lose! You cannot even beat an idiot AI!"
                  << std::endl;
      }

      std::cout << "Play again? (y/n): ";
      std::cin >> play_again;
    } while (play_again == "y");
  }

  return 0;
}