#include "state.h"

namespace AI {

namespace HDP {

State StateTransform(const State &state,
                     Game::Action &enemy_action,
                     Game::Action &action) {
  State new_state = state;
  if (state.energy < action.GetEnergy()) {
    new_state.health = 0;
    return new_state;
  }
  if (state.enemy_energy < enemy_action.GetEnergy()) {
    new_state.enemy_health = 0;
    return new_state;
  }

  const float HEALTH = 1000;
  const float ENERGY = 1000;
  Game::BattleField battle_field;
  battle_field.AddPlayer("self", HEALTH, ENERGY,
                         static_cast<Game::PlayerPosition>(Game::CENTER));
  battle_field.AddPlayer("enemy", HEALTH, ENERGY,
                         static_cast<Game::PlayerPosition>(Game::CENTER));
  battle_field.ActionUpdate(
      {action.GetFormalName(), enemy_action.GetFormalName()}, 1);
  battle_field.PositionUpdate();
  battle_field.EnergyUpdate();
  battle_field.HealthUpdate();

  new_state.energy += battle_field.GetPlayerEnergy(0) - ENERGY;
  new_state.health += battle_field.GetPlayerHealth(0) - HEALTH;
  new_state.enemy_energy += battle_field.GetPlayerEnergy(1) - ENERGY;
  new_state.enemy_health += battle_field.GetPlayerHealth(1) - HEALTH;
  return new_state;
}

State ConjugateState(const State &state) {
  State new_state = state;
  std::swap(new_state.health, new_state.enemy_health);
  std::swap(new_state.energy, new_state.enemy_energy);
  return new_state;
}

float Attention(float p) {
  if (p <= -0.00001 || p >= 1.00001) {
    std::cerr << "Error: Invalid attention parameter " << p
              << " should be in [0,1]" << std::endl;
    exit(1);
  }
  // if (p < 0 && p > -0.00001) {
  //   return 0;
  // } else if (p >= 1 && p < 1.00001) {
  //   return std::numeric_limits<float>::infinity();
  // }
  // return 1 / (1 - p) - 1;
  return std::exp(2 * p) - 1;
}

}  // namespace HDP

}  // namespace AI
