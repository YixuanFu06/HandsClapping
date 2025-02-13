#include "../HDP/include/model.h"
#include "../Idiot/include/policy.h"
#include "HDP/include/model.h"
#include "Idiot/include/policy.h"
#include "define_actions.h"

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::HDP::FindRootPath()
            << std::endl
            << std::endl;

  std::cout << "Enter the type of the first model (HDP or Idiot): ";
  std::string model1_type;
  std::cin >> model1_type;
  if (model1_type != "HDP" && model1_type != "Idiot") {
    std::cout << "Invalid model type!" << std::endl;
    return 0;
  }
  std::cout << "Enter the type of the second model (HDP or Idiot): ";
  std::string model2_type;
  std::cin >> model2_type;
  if (model2_type != "HDP" && model2_type != "Idiot") {
    std::cout << "Invalid model type!" << std::endl;
    return 0;
  }

  if (model1_type == "HDP" && model2_type == "HDP") {
    std::cout << "Enter the name of the first model: ";
    std::string model1_name;
    std::cin >> model1_name;
    AI::HDP::Model model1(model1_name);

    std::cout << "Enter the name of the second model: ";
    std::string model2_name;
    std::cin >> model2_name;
    AI::HDP::Model model2(model2_name);

    std::cout << "Enter the rounds of battle (1 round will give out the "
                 "specific battle information): ";
    uint32_t total_rounds;
    std::cin >> total_rounds;

    uint32_t model1_win = 0, model2_win = 0;
    for (uint32_t i = 0; i < total_rounds; i++) {
      if (model1_name == model2_name) {
        model2_name += "-copy";
      }
      Game::BattleField battle_field({model1_name, model2_name});
      while (battle_field.GetMemberNum() > 1) {
        float health1 = battle_field.GetPlayerHealth(0);
        float energy1 = battle_field.GetPlayerEnergy(0);
        float health2 = battle_field.GetPlayerHealth(1);
        float energy2 = battle_field.GetPlayerEnergy(1);
        Game::Action *action1 =
            model1.GetAction(health2, health1, energy2, energy1);
        Game::Action *action2 =
            model2.GetAction(health1, health2, energy1, energy2);
        if (total_rounds == 1) {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()});
        } else {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()}, 1);
        }
      }
      if (battle_field.GetMemberNum() < 1) {
        if (total_rounds == 1) {
          std::cout << "Game over! No winner!" << std::endl;
        }
      } else if (battle_field.GetPlayerName(0) == model1.GetName()) {
        model1_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model1_name << " wins!" << std::endl;
        }
      } else {
        model2_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model2_name << " wins!" << std::endl;
        }
      }
    }

    std::cout << model1_name << " wins " << model1_win << " rounds."
              << std::endl;
    std::cout << model2_name << " wins " << model2_win << " rounds."
              << std::endl;

  } else if (model1_type == "HDP" && model2_type == "Idiot") {
    std::cout << "Enter the name of the first model: ";
    std::string model1_name;
    std::cin >> model1_name;
    AI::HDP::Model model1(model1_name);

    std::cout << "Enter the name of the second model: ";
    std::string model2_name;
    std::cin >> model2_name;
    AI::Idiot::Policy model2(AI::Idiot::GetPolicyPath(model2_name));

    std::cout << "Enter the rounds of battle (1 round will give out the "
                 "specific battle information): ";
    uint32_t total_rounds;
    std::cin >> total_rounds;

    uint32_t model1_win = 0, model2_win = 0;
    for (uint32_t i = 0; i < total_rounds; i++) {
      Game::BattleField battle_field({model1.GetName(), model2.GetName()});
      while (battle_field.GetMemberNum() > 1) {
        float health1 = battle_field.GetPlayerHealth(0);
        float energy1 = battle_field.GetPlayerEnergy(0);
        float health2 = battle_field.GetPlayerHealth(1);
        float energy2 = battle_field.GetPlayerEnergy(1);
        Game::Action *action1 =
            model1.GetAction(health2, health1, energy2, energy1);
        Game::Action *action2 =
            model2.GetAction(health1, health2, energy1, energy2);
        if (total_rounds == 1) {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()});
        } else {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()}, 1);
        }
      }
      if (battle_field.GetMemberNum() < 1) {
        if (total_rounds == 1) {
          std::cout << "Game over! No winner!" << std::endl;
        }
      } else if (battle_field.GetPlayerName(0) == model1.GetName()) {
        model1_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model1.GetName() << " wins!"
                    << std::endl;
        }
      } else {
        model2_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model2.GetName() << " wins!"
                    << std::endl;
        }
      }
    }

    std::cout << model1_name << " wins " << model1_win << " rounds."
              << std::endl;
    std::cout << model2_name << " wins " << model2_win << " rounds."
              << std::endl;

  } else if (model1_type == "Idiot" && model2_type == "HDP") {
    std::cout << "Enter the name of the first model: ";
    std::string model1_name;
    std::cin >> model1_name;
    AI::Idiot::Policy model1(AI::Idiot::GetPolicyPath(model1_name));

    std::cout << "Enter the name of the second model: ";
    std::string model2_name;
    std::cin >> model2_name;
    AI::HDP::Model model2(model2_name);

    std::cout << "Enter the rounds of battle (1 round will give out the "
                 "specific battle information): ";
    uint32_t total_rounds;
    std::cin >> total_rounds;

    uint32_t model1_win = 0, model2_win = 0;
    for (uint32_t i = 0; i < total_rounds; i++) {
      Game::BattleField battle_field({model1.GetName(), model2.GetName()});
      while (battle_field.GetMemberNum() > 1) {
        float health1 = battle_field.GetPlayerHealth(0);
        float energy1 = battle_field.GetPlayerEnergy(0);
        float health2 = battle_field.GetPlayerHealth(1);
        float energy2 = battle_field.GetPlayerEnergy(1);
        Game::Action *action1 =
            model1.GetAction(health2, health1, energy2, energy1);
        Game::Action *action2 =
            model2.GetAction(health1, health2, energy1, energy2);
        if (total_rounds == 1) {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()});
        } else {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()}, 1);
        }
      }
      if (battle_field.GetMemberNum() < 1) {
        if (total_rounds == 1) {
          std::cout << "Game over! No winner!" << std::endl;
        }
      } else if (battle_field.GetPlayerName(0) == model1.GetName()) {
        model1_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model1.GetName() << " wins!"
                    << std::endl;
        }
      } else {
        model2_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model2.GetName() << " wins!"
                    << std::endl;
        }
      }
    }

    std::cout << model1_name << " wins " << model1_win << " rounds."
              << std::endl;
    std::cout << model2_name << " wins " << model2_win << " rounds."
              << std::endl;

  } else if (model1_type == "Idiot" && model2_type == "Idiot") {
    std::cout << "Enter the name of the first model: ";
    std::string model1_name;
    std::cin >> model1_name;
    AI::Idiot::Policy model1(AI::Idiot::GetPolicyPath(model1_name));

    std::cout << "Enter the name of the second model: ";
    std::string model2_name;
    std::cin >> model2_name;
    AI::Idiot::Policy model2(AI::Idiot::GetPolicyPath(model2_name));

    std::cout << "Enter the rounds of battle (1 round will give out the "
                 "specific battle information): ";
    uint32_t total_rounds;
    std::cin >> total_rounds;

    uint32_t model1_win = 0, model2_win = 0;
    for (uint32_t i = 0; i < total_rounds; i++) {
      if (model1_name == model2_name) {
        model2_name += "-copy";
      }
      Game::BattleField battle_field({model1_name, model2_name});
      while (battle_field.GetMemberNum() > 1) {
        float health1 = battle_field.GetPlayerHealth(0);
        float energy1 = battle_field.GetPlayerEnergy(0);
        float health2 = battle_field.GetPlayerHealth(1);
        float energy2 = battle_field.GetPlayerEnergy(1);
        Game::Action *action1 =
            model1.GetAction(health2, health1, energy2, energy1);
        Game::Action *action2 =
            model2.GetAction(health1, health2, energy1, energy2);
        if (total_rounds == 1) {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()});
        } else {
          battle_field.BattleFieldUpdate(
              {action1->GetFormalName(), action2->GetFormalName()}, 1);
        }
      }
      if (battle_field.GetMemberNum() < 1) {
        if (total_rounds == 1) {
          std::cout << "Game over! No winner!" << std::endl;
        }
      } else if (battle_field.GetPlayerName(0) == model1.GetName()) {
        model1_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model1.GetName() << " wins!"
                    << std::endl;
        }
      } else {
        model2_win++;
        if (total_rounds == 1) {
          std::cout << "Game over! " << model2.GetName() << " wins!"
                    << std::endl;
        }
      }
    }

    std::cout << model1_name << " wins " << model1_win << " rounds."
              << std::endl;
    std::cout << model2_name << " wins " << model2_win << " rounds."
              << std::endl;
  }

  return 0;
}