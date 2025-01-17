#include "../game/battle_field.h"

int main() {
  Game::InitActions();
  Game::BattleField battle_field({"fyx", "zzr"});
  while (battle_field.GetMemberNum() > 1) {
    battle_field.BattleFieldUpdate();
  }
  std::cout << "Game Over! " << battle_field.GetPlayerName(0) << " wins!"
            << std::endl;
  return 0;
}