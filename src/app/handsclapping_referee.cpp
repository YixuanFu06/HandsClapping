#include <limits>

#include "../game/battle_field.h"

int main() {
  const float HEALTH = 1000;
  const float ENERGY = 1000;
  Game::InitActions();
  Game::BattleField battle_field;
  while (true) {
    battle_field.AddPlayer("me", HEALTH, ENERGY);
    battle_field.AddPlayer("opponent", HEALTH, ENERGY);
    battle_field.BattleFieldUpdate(3);
    battle_field.RemovePlayer("me");
    battle_field.RemovePlayer("opponent");
  }
}