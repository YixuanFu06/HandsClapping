#include "../../../game/battle_field.h"
#include "../include/policy.h"

const uint32_t MAX_TRAINERS = 10;

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

  std::cout << "Enter the number of trainers (at most " << MAX_TRAINERS
            << "): ";
  uint32_t trainer_num;
  std::cin >> trainer_num;
  if (trainer_num > MAX_TRAINERS) {
    std::cout << "Too much trainers! The number of trainers is at most "
              << MAX_TRAINERS << ". More will be ignored." << std::endl;
    trainer_num = MAX_TRAINERS;
  }
  std::vector<AI::Idiot::Policy> trainers;
  for (uint32_t i = 0; i < trainer_num; i++) {
    std::string trainer_name;
    std::cin >> trainer_name;
    trainers.push_back(AI::Idiot::GetPolicyPath(trainer_name));
  }

  std::cout << "Enter the name of the trainee: ";
  std::string trainee_name;
  std::cin >> trainee_name;
  AI::Idiot::Policy trainee(AI::Idiot::GetPolicyPath(trainee_name));

  uint32_t total_rounds = 200;
  std::cout << "Enter the rounds of training: ";
  std::cin >> total_rounds;
  std::cout << "Train on model " << trainee.GetName() << "[" << trainee.GetId()
            << "] and hybrid model of ";
  for (AI::Idiot::Policy &trainer : trainers) {
    std::cout << trainer.GetName() << "[" << trainer.GetId() << "], ";
  }
  std::cout << "for " << total_rounds << " rounds:" << std::endl;

  uint32_t trainee_win = 0, trainers_win = 0;
  uint32_t single_trainer_attend[MAX_TRAINERS] = {0};
  uint32_t single_trainer_win[MAX_TRAINERS] = {0};

  for (uint32_t i = 0; i < total_rounds; i++) {
    // choose a policy from trainers uniformly randomly
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, trainers.size() - 1);
    uint32_t tmp_trainer_index = dis(gen);
    AI::Idiot::Policy &tmp_trainer = trainers[tmp_trainer_index];
    single_trainer_attend[tmp_trainer_index]++;

    bool IsSameName = false;
    if (tmp_trainer.GetName() == trainee.GetName()) {
      tmp_trainer.SetName(tmp_trainer.GetName() + "-trainer");
      IsSameName = true;
    }

    Game::BattleField battle_field({trainee.GetName(), tmp_trainer.GetName()});
    AI::Idiot::Reward strategy_reward1(trainee);
    AI::Idiot::Reward action_reward1(trainee);
    while (battle_field.GetMemberNum() > 1) {
      float health1 = battle_field.GetPlayerHealth(0);
      float energy1 = battle_field.GetPlayerEnergy(0);
      float health2 = battle_field.GetPlayerHealth(1);
      float energy2 = battle_field.GetPlayerEnergy(1);
      Game::Action *action1 =
          trainee.GetAction(health2, health1, energy2, energy1);
      Game::Action *action2 =
          tmp_trainer.GetAction(health1, health2, energy1, energy2);
      strategy_reward1.StrategyUpdate(health2, health1, energy2, energy1,
                                      action2, action1);
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
      }
    }
    if (battle_field.GetMemberNum() == 0) {
      trainee.Update(action_reward1);
    } else if (battle_field.GetPlayerName(0) == trainee.GetName()) {
      trainee.Update(action_reward1 + strategy_reward1);
      trainee_win++;
    } else {
      trainee.Update(action_reward1 - strategy_reward1);
      trainers_win++;
      single_trainer_win[tmp_trainer_index]++;
    }

    if (IsSameName) {
      tmp_trainer.SetName(trainee.GetName());
    }

    PrintProgressBar(i + 1, total_rounds);
  }

  char confirm;
  std::cout << std::endl
            << trainee.GetName() << " wins: " << trainee_win
            << " times. Trainers win: " << trainers_win << " times."
            << std::endl;
  std::cout << "In which ";
  for (uint32_t i = 0; i < trainers.size(); i++) {
    std::cout << trainers[i].GetName() << " wins " << single_trainer_win[i]
              << " out of " << single_trainer_attend[i] << " times. ";
  }
  std::cout << std::endl;
  std::cout << std::endl << "Do you want to store the policy? (y/n): ";
  std::cin >> confirm;
  while (confirm != 'y' && confirm != 'n') {
    std::cout << "Invalid input. Please input again." << std::endl;
    std::cin >> confirm;
  }
  if (confirm == 'y') {
    trainee.Store();
    std::cout << "Policy change is stored to "
              << AI::Idiot::GetPolicyPath(trainee_name).string() << std::endl;
  } else if (confirm == 'n') {
    std::cout << "Policy change is discarded." << std::endl;
  }

  std::cout << "Press any key to continue..." << std::endl;
  std::cin.get();

  return 0;
}