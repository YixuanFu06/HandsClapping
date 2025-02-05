#include "../../../game/battle_field.h"
#include "../include/policy.h"

const uint32_t MAX_POLICIES = 10;
const std::vector<std::string> POLICIES = {"Idiot-alpha", "Idiot-beta",
                                           "Idiot-gamma", "Idiot-init"};

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
  std::cout << "HandsClapping directory found at: " << AI::Idiot::FindRootPath()
            << std::endl
            << std::endl;
  std::vector<AI::Idiot::Policy> policies;
  for (const std::string &policy_name : POLICIES) {
    if (policies.size() == MAX_POLICIES) {
      std::cout << "Too much trainers! The number of trainers is at most "
                << MAX_POLICIES << ". More will be ignored." << std::endl;
      break;
    }
    policies.push_back(AI::Idiot::GetPolicyPath(policy_name));
  }

  uint32_t total_rounds = 200;
  std::cout << "Enter the rounds of training: ";
  std::cin >> total_rounds;
  std::cout << "Train on models ";
  for (AI::Idiot::Policy &policy : policies) {
    std::cout << policy.GetName() << "[" << policy.GetId() << "], ";
  }
  std::cout << "for " << total_rounds << " rounds:" << std::endl;

  std::cout
      << "Enter the initial state of battle field in the order of "
         "<player1_health> <player2_health> <player1_energy> <player2_energy>:"
      << std::endl;
  float player1_init_health;
  float player2_init_health;
  float player1_init_energy;
  float player2_init_energy;
  std::cin >> player1_init_health >> player2_init_health >>
      player1_init_energy >> player2_init_energy;

  uint32_t single_policy_attend[MAX_POLICIES] = {0};
  uint32_t single_policy_win[MAX_POLICIES] = {0};

  for (uint32_t i = 0; i < total_rounds; i++) {
    for (uint32_t j = 0; j < policies.size(); j++) {
      uint32_t policy1_index = j % policies.size();
      AI::Idiot::Policy &policy1 = policies[policy1_index];

      // choose a policy from policies uniformly randomly
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(0, policies.size() - 1);
      uint32_t policy2_index = dis(gen);
      AI::Idiot::Policy &policy2 = policies[policy2_index];
      if (policy1.GetName() == policy2.GetName()) {
        j--;
        continue;
      }
      single_policy_attend[policy1_index]++;
      single_policy_attend[policy2_index]++;

      Game::BattleField battle_field({policy1.GetName(), policy2.GetName()});
      battle_field.SetPlayerHealth(0, player1_init_health);
      battle_field.SetPlayerHealth(1, player2_init_health);
      battle_field.SetPlayerEnergy(0, player1_init_energy);
      battle_field.SetPlayerEnergy(1, player2_init_energy);

      AI::Idiot::Reward strategy_reward1(policy1);
      AI::Idiot::Reward strategy_reward2(policy2);
      AI::Idiot::Reward action_reward1(policy1);
      AI::Idiot::Reward action_reward2(policy2);
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
        single_policy_win[policy1_index]++;
      } else {
        policy1.Update(action_reward1 - strategy_reward1);
        policy2.Update(action_reward2 + strategy_reward2);
        single_policy_win[policy2_index]++;
      }
    }

    PrintProgressBar(i + 1, total_rounds);
  }

  char confirm;
  for (uint32_t i = 0; i < policies.size(); i++) {
    std::cout << policies[i].GetName() << " wins " << single_policy_win[i]
              << " out of " << single_policy_attend[i] << " times. ";
  }
  std::cout << std::endl;
  std::cout << std::endl << "Do you want to store the policy? (y/n): ";
  std::cin >> confirm;
  while (confirm != 'y' && confirm != 'n') {
    std::cout << "Invalid input. Please input again." << std::endl;
    std::cin >> confirm;
  }
  if (confirm == 'y') {
    for (AI::Idiot::Policy &policy : policies) {
      policy.Store();
      std::cout << "Policy change is stored to "
                << AI::Idiot::GetPolicyPath(policy.GetName()).string()
                << std::endl;
    }
  } else if (confirm == 'n') {
    std::cout << "Policy change is discarded." << std::endl;
  }

  return 0;
}