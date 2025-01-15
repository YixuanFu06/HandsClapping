#pragma once

#include <bits/stdint-uintn.h>

#include <cstdint>
#include <vector>

#include "battle_field.h"

namespace Game {

enum class ActionType { ATTACK, DEFEND, DODGE, EQUIP };

class Action {
 private:
  float energy_;
  std::vector<float> damage_;
  std::vector<float> effect_;
  uint32_t dodge_position_;

  ActionType type_;
  uint32_t id_;

 public:
  Action(float energy,
         std::vector<float> damage,
         std::vector<float> effect,
         ActionType type,
         uint32_t id);
  Action(float energy, std::vector<float> damage, ActionType type, uint32_t id);

  Action(float energy,
         float damage,
         float effect,
         ActionType type,
         uint32_t id);
  Action(float energy, float damage, ActionType type, uint32_t id);

  Action(float energy,
         float damage,
         std::vector<uint32_t> damage_range,
         float effect,
         std::vector<uint32_t> effect_range,
         ActionType type,
         uint32_t id);
  Action(float energy,
         float damage,
         std::vector<uint32_t> range,
         ActionType type,
         uint32_t id);

  Action(float dodge_position_, uint32_t id_);

  Action(float energy, float damage, uint32_t id);

  inline float GetEnergy();
  inline void SetEnergy(float energy);

  inline float GetDamage(int position);

  inline float GetEffect(int position);
};

}  // namespace Game