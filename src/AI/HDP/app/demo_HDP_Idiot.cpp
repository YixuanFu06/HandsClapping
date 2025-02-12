#include "../../Idiot/include/policy.h"
#include "../include/model.h"
#include "define_actions.h"

const std::string HDP_MODEL = "HDP-alpha";
const std::string IDIOT_MODEL = "Idiot-gamma";

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::HDP::FindRootPath()
            << std::endl
            << std::endl;

  AI::HDP::Model hdp_model(HDP_MODEL);
  AI::Idiot::Policy idiot_policy(AI::Idiot::GetPolicyPath(IDIOT_MODEL));
  std::cout << "Enter the rounds of battle: ";
  uint32_t total_rounds;
  std::cin >> total_rounds;

  uint32_t hdp_win = 0, idiot_win = 0;
  for (uint32_t i = 0; i < total_rounds; i++) {
    Game::BattleField battle_field(
        {hdp_model.GetName(), idiot_policy.GetName()});
    while (battle_field.GetMemberNum() > 1) {
      float health1 = battle_field.GetPlayerHealth(0);
      float energy1 = battle_field.GetPlayerEnergy(0);
      float health2 = battle_field.GetPlayerHealth(1);
      float energy2 = battle_field.GetPlayerEnergy(1);
      Game::Action *action1 =
          hdp_model.GetAction(health2, health1, energy2, energy1);
      Game::Action *action2 =
          idiot_policy.GetAction(health1, health2, energy1, energy2);
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
    } else if (battle_field.GetPlayerName(0) == hdp_model.GetName()) {
      hdp_win++;
      if (total_rounds == 1) {
        std::cout << "Game over! " << hdp_model.GetName() << " wins!"
                  << std::endl;
      }
    } else {
      idiot_win++;
      if (total_rounds == 1) {
        std::cout << "Game over! " << idiot_policy.GetName() << " wins!"
                  << std::endl;
      }
    }
  }

  std::cout << "HDP wins " << hdp_win << " rounds." << std::endl;
  std::cout << "Idiot wins " << idiot_win << " rounds." << std::endl;

  return 0;
}