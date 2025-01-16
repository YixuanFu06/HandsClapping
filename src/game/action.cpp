#include "action.h"

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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
  type_ = type;
  id_ = id;
}

Action::Action(float energy, float effect, uint32_t id) {
  energy_ = energy;
  effect_.resize(POSITION_NUM, 0.0f);
  effect_[CENTER] = effect;
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
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
  dodge_position_ = static_cast<PlayerPosition>(CENTER);
  type_ = ActionType::EQUIP;
  id_ = id;
}

void Action::PrintAction() {
  std::cout << "Action " << id_ << ":\n";
  std::cout << "Energy: " << energy_ << std::endl;
  std::cout << "Damage: ";
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    std::cout << "position: " << static_cast<PlayerPosition>(pos) << " " << damage_[pos] << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Effect: ";
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    std::cout << "position: " << static_cast<PlayerPosition>(pos) << " " << effect_[pos] << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Dodge Position: " << dodge_position_ << std::endl;
  std::cout << "Type: " << type_ << std::endl;
}

}  // namespace Game