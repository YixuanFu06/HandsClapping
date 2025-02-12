#include "../../../game/battle_field.h"
#include "../include/model.h"

const std::string MODEL = "HDP-alpha";

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::HDP::FindRootPath()
            << std::endl
            << std::endl;
  AI::HDP::Model model = AI::HDP::Model(MODEL);

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

  return 0;
}