#include "action.h"

#include <iostream>

namespace Game {

Action::Action(float energy,
               std::vector<float> damage,
               std::vector<float> effect,
               ActionType type,
               uint32_t id) {
  if (damage.size() != POSITION_NUM || effect.size() != POSITION_NUM) {
    std::cout
        << "Error: damage vector size is not equal to POSITION_NUM in action "
        << id << std::endl;
    exit(1);
  }
  energy_ = energy;
  damage_ = damage;
  effect_ = effect;
  type_ = type;
  id_ = id;
}

Action::Action(float energy,
               std::vector<float> damage,
               ActionType type,
               uint32_t id) {
  if (damage.size() != POSITION_NUM) {
    std::cout
        << "Error: damage vector size is not equal to POSITION_NUM in action "
        << id << std::endl;
    exit(1);
  }
  energy_ = energy;
  damage_ = damage;
  effect_ = damage;
  type_ = type;
  id_ = id;
}

Action::Action(float energy,
               float damage,
               float effect,
               ActionType type,
               uint32_t id) {
  energy_ = energy;
  damage_.resize(POSITION_NUM, 0.0f);
  damage_[CENTER] = damage;
  effect_.resize(POSITION_NUM, 0.0f);
  effect_[CENTER] = effect;
  type_ = type;
  id_ = id;
}

Action::Action(float energy, float damage, ActionType type, uint32_t id) {
  energy_ = energy;
  damage_.resize(POSITION_NUM, 0.0f);
  damage_[CENTER] = damage;
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    if (damage_[pos] != 0) {
      effect_[pos] = energy_;
    } else {
      effect_[pos] = 0;
    }
  }
  type_ = type;
  id_ = id;
}

Action::Action(float energy,
               float damage,
               std::vector<uint32_t> damage_range,
               float effect,
               std::vector<uint32_t> effect_range,
               ActionType type,
               uint32_t id) {
  energy_ = energy;
  damage_.resize(POSITION_NUM, 0.0f);
  effect_.resize(POSITION_NUM, 0.0f);
  for (uint32_t i = 0; i < damage_range.size(); i++) {
    damage_[damage_range[i]] = damage;
    effect_[effect_range[i]] = effect;
  }
  type_ = type;
  id_ = id;
}

Action::Action(float energy,
               float damage,
               std::vector<uint32_t> range,
               ActionType type,
               uint32_t id) {
  energy_ = energy;
  damage_.resize(POSITION_NUM, 0.0f);
  for (uint32_t i = 0; i < range.size(); i++) {
    damage_[range[i]] = damage;
  }
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    if (damage_[pos] != 0) {
      effect_[pos] = energy_;
    } else {
      effect_[pos] = 0;
    }
  }
  type_ = type;
  id_ = id;
}

Action::Action(float energy, float effect, uint32_t id) {
  energy_ = energy;
  effect_.resize(POSITION_NUM, 0.0f);
  effect_[CENTER] = effect;
  type_ = ActionType::DEFEND;
  id = id_;
}

Action::Action(PlayerPosition dodge_position, uint32_t id) {
  dodge_position_ = dodge_position;
  type_ = ActionType::DODGE;
  id_ = id;
}

Action::Action(float energy, uint32_t id) {
  energy_ = energy;
  type_ = ActionType::EQUIP;
  id_ = id;
}

inline float Action::GetEnergy() {
  return energy_;
}

inline void Action::SetEnergy(float energy) {
  energy_ = energy;
}

inline float Action::GetDamage(int position) {
  return damage_[position];
}

inline float Action::GetEffect(int position) {
  return effect_[position];
}

}  // namespace Game