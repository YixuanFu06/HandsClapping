#include <limits>

#include "../game/battle_field.h"

int main() {
  const float HEALTH = 1000;
  const float ENERGY = 1000;
  Game::InitActions();
  while (true) {
    Game::BattleField battle_field;
    battle_field.AddPlayer("player1", HEALTH, ENERGY,
                           static_cast<Game::PlayerPosition>(Game::CENTER));
    battle_field.AddPlayer("player2", HEALTH, ENERGY,
                           static_cast<Game::PlayerPosition>(Game::CENTER));
    battle_field.BattleFieldUpdate(3);
  }

  std::cout << "Press any key to continue..." << std::endl;
  system("pause");
  return 0;
}