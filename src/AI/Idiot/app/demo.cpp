#include "../../../game/battle_field.h"
#include "../include/policy.h"

const std::string POLICY1 = "Idiot-alpha";
const std::string POLICY2 = "init";

int main() {
  Game::InitActions();
  AI::Idiot::Policy policy1 = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY1).string());
  AI::Idiot::Policy policy2 = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY2).string());

  Game::BattleField battle_field({policy1.GetName(), policy2.GetName()});
  while (battle_field.GetMemberNum() > 1) {
    float health1 = battle_field.GetPlayerHealth(0);
    float energy1 = battle_field.GetPlayerEnergy(0);
    float health2 = battle_field.GetPlayerHealth(1);
    float energy2 = battle_field.GetPlayerEnergy(1);
    Game::Action *action1 =
        policy1.GetAction(health2, health1, energy2, energy1);
    Game::Action *action2 =
        policy2.GetAction(health1, health2, energy1, energy2);
    battle_field.BattleFieldUpdate(
        {action1->GetFormalName(), action2->GetFormalName()});
  }
  if (battle_field.GetMemberNum() < 1) {
    std::cout << "Game over! No winner!" << std::endl;
  } else {
    std::cout << "Game over! " << battle_field.GetPlayerName(0) << " wins!"
              << std::endl;
  }

  return 0;
}