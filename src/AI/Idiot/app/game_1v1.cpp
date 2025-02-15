#include "../../../game/battle_field.h"
#include "../include/policy.h"

const std::string POLICY = "Idiot-init";

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::Idiot::FindRootPath()
            << std::endl
            << std::endl;
  AI::Idiot::Policy policy =
      AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY));

  std::string play_again;

  do {
    Game::BattleField battle_field({"player", policy.GetName()});

    AI::Idiot::Reward strategy_reward(policy);
    AI::Idiot::Reward action_reward(policy);

    std::cout << "Game between you and AI " << policy.GetName() << " starts."
              << std::endl;
    while (battle_field.GetMemberNum() > 1) {
      std::cout << std::endl;
      float player_health = battle_field.GetPlayerHealth(0);
      float player_energy = battle_field.GetPlayerEnergy(0);
      float AI_health = battle_field.GetPlayerHealth(1);
      float AI_energy = battle_field.GetPlayerEnergy(1);
      Game::Action *AI_action =
          policy.GetAction(player_health, AI_health, player_energy, AI_energy);
      std::string player_action_name;
      std::cout << "Enter your action in the next turn: ";
      std::cin >> player_action_name;
      Game::Action *player_action;
      bool IsFound = false;
      for (Game::Action &it : Game::actions) {
        std::vector<std::string> nicknames = it.GetNicknames();
        for (std::string &nickname : nicknames) {
          if (nickname == player_action_name) {
            IsFound = true;
            player_action = &it;
          }
        }
      }
      if (!IsFound) {
        for (Game::Action &it : Game::actions) {
          if (it.GetId() == Game::PRODUCE) {
            std::cout << "Unknown action " << player_action_name
                      << " received, set as PRODUCE.\n";
            player_action = &it;
          }
        }
      }

      strategy_reward.StrategyUpdate(player_health, AI_health, player_energy,
                                     AI_energy, player_action, AI_action);
      battle_field.BattleFieldUpdate(
          {player_action->GetFormalName(), AI_action->GetFormalName()});

      if (battle_field.GetMemberNum() == 2) {
        float current_player_health = battle_field.GetPlayerHealth(0);
        float current_AI_health = battle_field.GetPlayerHealth(1);
        float current_player_energy = battle_field.GetPlayerEnergy(0);
        float current_AI_energy = battle_field.GetPlayerEnergy(1);
        if (AI_action->GetType() == Game::ATTACK) {
          action_reward.ActionUpdate_Health(
              player_health, AI_health, player_energy, AI_energy, AI_action,
              current_AI_health - AI_health + player_health -
                  current_player_health);
        }
        action_reward.ActionUpdate_Energy(
            player_health, AI_health, player_energy, AI_energy, AI_action,
            current_AI_energy - AI_energy + player_energy -
                current_player_energy);
      }
    }

    if (battle_field.GetMemberNum() == 0) {
      policy.Update(action_reward);
    } else if (battle_field.GetPlayerName(0) == "player") {
      policy.Update(action_reward - strategy_reward);
    } else {
      policy.Update(action_reward + strategy_reward);
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

  char confirm;
  std::cout << std::endl << "Do you want to store the policy? (y/n): ";
  std::cin >> confirm;
  while (confirm != 'y' && confirm != 'n') {
    std::cout << "Invalid input. Please input again." << std::endl;
    std::cin >> confirm;
  }
  if (confirm == 'y') {
    policy.Store();
    std::cout << "Policy change is stored to "
              << AI::Idiot::GetPolicyPath(POLICY).string() << std::endl;
  } else if (confirm == 'n') {
    std::cout << "Policy change is discarded." << std::endl;
  }
  return 0;
}