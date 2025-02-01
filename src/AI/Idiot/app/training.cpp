#include "../../../game/battle_field.h"
#include "../include/policy.h"

const std::string POLICY1 = "Idiot-gamma";
const std::string POLICY2 = "Idiot-beta1";

void PrintProgressBar(int current, int total) {
  int bar_width = 70;
  float progress = static_cast<float>(current) / total;

  std::cout << "[";
  int pos = bar_width * progress;
  for (int i = 0; i < bar_width; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";
  std::cout.flush();
}

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::Idiot::FindRootPath() << std::endl << std::endl;
  AI::Idiot::Policy policy1 = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY1));
  AI::Idiot::Policy policy2 = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY2));

  uint32_t total_rounds = 200;
  std::cout << "Enter the rounds of training: ";
  std::cin >> total_rounds;
  std::cout << "Train on model " << policy1.GetName() << "[" << policy1.GetId()
            << "] and " << policy2.GetName() << "[" << policy2.GetId()
            << "] for " << total_rounds << " rounds:" << std::endl;

  uint32_t player1_win = 0, player2_win = 0;

  for (uint32_t i = 0; i < total_rounds; i++) {
    Game::BattleField battle_field({policy1.GetName(), policy2.GetName()});
    AI::Idiot::Reward strategy_reward1 = AI::Idiot::Reward(policy1);
    AI::Idiot::Reward strategy_reward2 = AI::Idiot::Reward(policy2);
    AI::Idiot::Reward action_reward1 = AI::Idiot::Reward(policy1);
    AI::Idiot::Reward action_reward2 = AI::Idiot::Reward(policy2);
    while (battle_field.GetMemberNum() > 1) {
      float health1 = battle_field.GetPlayerHealth(0);
      float energy1 = battle_field.GetPlayerEnergy(0);
      float health2 = battle_field.GetPlayerHealth(1);
      float energy2 = battle_field.GetPlayerEnergy(1);
      Game::Action *action1 =
          policy1.GetAction(health2, health1, energy2, energy1);
      Game::Action *action2 =
          policy2.GetAction(health1, health2, energy1, energy2);
      strategy_reward1.StrategyUpdate(health2, health1, energy2, energy1,
                                      action2, action1);
      strategy_reward2.StrategyUpdate(health1, health2, energy1, energy2,
                                      action1, action2);
      battle_field.BattleFieldUpdate(
          {action1->GetFormalName(), action2->GetFormalName()}, 1);

      if (battle_field.GetMemberNum() == 2) {
        float current_health1 = battle_field.GetPlayerHealth(0);
        float current_energy1 = battle_field.GetPlayerEnergy(0);
        float current_health2 = battle_field.GetPlayerHealth(1);
        float current_energy2 = battle_field.GetPlayerEnergy(1);
        if (action1->GetType() == Game::ATTACK) {
          action_reward1.ActionUpdate_Health(
              health2, health1, energy2, energy1, action1,
              current_health1 - health1 + health2 - current_health2);
        }
        action_reward1.ActionUpdate_Energy(
            health2, health1, energy2, energy1, action1,
            current_energy1 - energy1 + energy2 - current_energy2);

        if (action2->GetType() == Game::ATTACK) {
          action_reward2.ActionUpdate_Health(
              health1, health2, energy1, energy2, action2,
              current_health2 - health2 + health1 - current_health1);
        }
        action_reward2.ActionUpdate_Energy(
            health1, health2, energy1, energy2, action2,
            current_energy2 - energy2 + energy1 - current_energy1);
      }
    }
    if (battle_field.GetMemberNum() == 0) {
      policy1.Update(action_reward1);
      policy2.Update(action_reward2);
    } else if (battle_field.GetPlayerName(0) == policy1.GetName()) {
      policy1.Update(action_reward1 + strategy_reward1);
      policy2.Update(action_reward2 - strategy_reward2);
      player1_win++;
    } else {
      policy1.Update(action_reward1 - strategy_reward1);
      policy2.Update(action_reward2 + strategy_reward2);
      player2_win++;
    }

    PrintProgressBar(i + 1, total_rounds);
  }

  char confirm;
  std::cout << std::endl
            << policy1.GetName() << " win: " << player1_win
            << " times, " << policy2.GetName() << " win: " << player2_win << " times.";
  std::cout << std::endl << "Do you want to store the policy? (y/n): ";
  std::cin >> confirm;
  while (confirm != 'y' && confirm != 'n') {
    std::cout << "Invalid input. Please input again." << std::endl;
    std::cin >> confirm;
  }
  if (confirm == 'y') {
    policy1.Store();
    policy2.Store();
    std::cout << "Policy change is stored to " << AI::Idiot::GetPolicyPath(POLICY1).string() << " and " << AI::Idiot::GetPolicyPath(POLICY2).string() << std::endl;
  } else if (confirm == 'n') {
    std::cout << "Policy change is discarded." << std::endl;
  }

  return 0;
}