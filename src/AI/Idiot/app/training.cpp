#include <filesystem>

#include "../../../game/battle_field.h"
#include "../include/policy.h"

const std::string DATA_PATH = "./data/AI/Idiot/policy.txt";

void PrintProgressBar(int current, int total) {
  int barWidth = 70;
  float progress = static_cast<float>(current) / total;

  std::cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
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
  std::filesystem::path current_path = std::filesystem::current_path();
  std::filesystem::path data_path = current_path / DATA_PATH;
  std::filesystem::create_directories(data_path.parent_path());

  Game::InitActions();
  AI::Idiot::Policy policy = AI::Idiot::Policy(data_path.string());
  uint32_t total_rounds = 200;
  std::cout << "Enter the rounds of training: ";
  std::cin >> total_rounds;
  std::cout << "Train on model " << policy.GetName() << "[" << policy.GetId()
            << "] for " << total_rounds << " rounds:" << std::endl;
  for (uint32_t i = 0; i < total_rounds; i++) {
    Game::BattleField battle_field({"player1", "player2"});
    AI::Idiot::Reward reward1 = AI::Idiot::Reward(policy.GetId());
    AI::Idiot::Reward reward2 = AI::Idiot::Reward(policy.GetId());
    while (battle_field.GetMemberNum() > 1) {
      float health1 = battle_field.GetPlayerHealth(0);
      float energy1 = battle_field.GetPlayerEnergy(0);
      float health2 = battle_field.GetPlayerHealth(1);
      float energy2 = battle_field.GetPlayerEnergy(1);
      Game::Action *action1 =
          policy.GetAction(health2, energy2, health1, energy1);
      Game::Action *action2 =
          policy.GetAction(health1, energy1, health2, energy2);
      reward1.Update(health2, energy2, health1, energy1, action1);
      reward2.Update(health1, energy1, health2, energy2, action2);
      battle_field.BattleFieldUpdate(
          {action1->GetFormalName(), action2->GetFormalName()}, 1);
    }
    if (battle_field.GetMemberNum() == 0) {
      continue;
    } else if (battle_field.GetPlayerName(0) == "player1") {
      AI::Idiot::Reward reward = reward1 - reward2;
      policy.Update(reward);
    } else {
      AI::Idiot::Reward reward = reward2 - reward1;
      policy.Update(reward);
    }

    PrintProgressBar(i + 1, total_rounds);
  }

  char confirm;
  std::cout << std::endl
            << "Do you want to store the policy? (y/n)" << std::endl;
  std::cin >> confirm;
  while (confirm != 'y' && confirm != 'n') {
    std::cout << "Invalid input. Please input again." << std::endl;
    std::cin >> confirm;
  }
  if (confirm == 'y') {
    policy.Store(data_path.string());
    std::cout << "Policy change is stored to " << data_path.string()
              << std::endl;
  } else if (confirm == 'n') {
    std::cout << "Policy change is discard." << std::endl;
  }
  return 0;
}