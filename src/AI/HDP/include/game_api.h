#pragma once

#include "../../../game/battle_field.h"

namespace AI {

namespace HDP {

const float INFINITY_FLOAT = 1e9;

struct State {
  float enemy_health;
  float health;
  float enemy_energy;
  float energy;
};

State StateTransform(const State &state,
                     Game::Action &enemy_action,
                     Game::Action &action);

State ConjugateState(const State &state);

int StateReward(const State &state);

float Attention(float p);

}  // namespace HDP

}  // namespace AI