#include "../game/battle_field.h"

int main() {
  Game::InitActions();
  Game::BattleField battle_field({"fyx", "zzr", "gzj"});
  while (battle_field.GetMemberNum() > 1) {
    battle_field.BattleFieldUpdate(1);
  }
  std::cout << "Game Over! ";
  if (battle_field.GetMemberNum() == 0) {
    std::cout << "No one wins!" << std::endl;
  } else {
    std::cout << battle_field.GetPlayerName(0) << " wins!" << std::endl;
  }
  return 0;
}