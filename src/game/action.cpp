#include "action.h"

namespace Game {

uint32_t DIMENSION = 2;
uint32_t POSITION_NUM = std::pow(3, DIMENSION);
uint32_t CENTER = POSITION_NUM / 2;

Action::Action(float energy,
               std::vector<float> damage,
               std::vector<float> effect,
               ActionType type,
               TargetType target_type,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      damage_(damage),
      effect_(effect),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(type),
      target_type_(target_type),
      id_(id),
      nicknames_(nicknames) {
  if (damage.size() != POSITION_NUM || effect.size() != POSITION_NUM) {
    std::cout
        << "Error: damage vector size is not equal to POSITION_NUM in action "
        << id << std::endl;
    exit(1);
  }
}

Action::Action(float energy,
               std::vector<float> damage,
               ActionType type,
               TargetType target_type,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      damage_(damage),
      effect_(damage),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(type),
      target_type_(target_type),
      id_(id),
      nicknames_(nicknames) {
  if (damage.size() != POSITION_NUM) {
    std::cout
        << "Error: damage vector size is not equal to POSITION_NUM in action "
        << id << std::endl;
    exit(1);
  }
}

Action::Action(float energy,
               float damage,
               float effect,
               ActionType type,
               TargetType target_type,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(type),
      target_type_(target_type),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  damage_[CENTER] = damage;
  effect_.resize(POSITION_NUM, 0.0f);
  effect_[CENTER] = effect;
}

Action::Action(float energy,
               float damage,
               ActionType type,
               TargetType target_type,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(type),
      target_type_(target_type),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  damage_[CENTER] = damage;
  effect_.resize(POSITION_NUM, 0.0f);
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    if (damage_[pos] != 0) {
      effect_[pos] = energy_;
    } else {
      effect_[pos] = 0;
    }
  }
}

Action::Action(float energy,
               float damage,
               std::vector<uint32_t> damage_range,
               float effect,
               std::vector<uint32_t> effect_range,
               ActionType type,
               TargetType target_type,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(type),
      target_type_(target_type),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  effect_.resize(POSITION_NUM, 0.0f);
  for (uint32_t i = 0; i < damage_range.size(); i++) {
    damage_[damage_range[i]] = damage;
    effect_[effect_range[i]] = effect;
  }
}

Action::Action(float energy,
               float damage,
               std::vector<uint32_t> range,
               ActionType type,
               TargetType target_type,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(type),
      target_type_(target_type),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  for (uint32_t i = 0; i < range.size(); i++) {
    damage_[range[i]] = damage;
  }
  effect_.resize(POSITION_NUM, 0.0f);
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    if (damage_[pos] != 0) {
      effect_[pos] = energy_;
    } else {
      effect_[pos] = 0;
    }
  }
}

Action::Action(float energy,
               float effect,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(energy),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(DEFEND),
      target_type_(SELF),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  effect_.resize(POSITION_NUM, 0.0f);
  effect_[CENTER] = effect;
}

Action::Action(PlayerPosition dodge_position,
               uint32_t id,
               std::vector<std::string> nicknames)
    : energy_(0),
      dodge_position_(dodge_position),
      type_(DODGE),
      target_type_(SELF),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  effect_.resize(POSITION_NUM, 0.0f);
}

Action::Action(float energy, uint32_t id, std::vector<std::string> nicknames)
    : energy_(energy),
      dodge_position_(static_cast<PlayerPosition>(CENTER)),
      type_(EQUIP),
      target_type_(SELF),
      id_(id),
      nicknames_(nicknames) {
  damage_.resize(POSITION_NUM, 0.0f);
  effect_.resize(POSITION_NUM, 0.0f);
}

void Action::PrintAction() {
  std::cout << "Action " << id_ << ": ";
  for (int i = 0; i < nicknames_.size(); i++) {
    std::cout << nicknames_[i] << "  ";
  }
  std::cout << std::endl;
  std::cout << "Energy: " << energy_ << std::endl;
  std::cout << "Damage:\n";
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    std::cout << "position: " << static_cast<PlayerPosition>(pos) << " "
              << damage_[pos] << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Effect:\n";
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    std::cout << "position: " << static_cast<PlayerPosition>(pos) << " "
              << effect_[pos] << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Dodge Position: " << dodge_position_ << std::endl;
  std::cout << "Type: " << type_ << std::endl;
}

std::string Action::GetActionMessage() {
  std::string message = "Action " + std::to_string(id_) + ": ";
  for (int i = 0; i < nicknames_.size(); i++) {
    message += nicknames_[i] + "  ";
  }
  message += "\n";
  message += "Energy: " + std::to_string(energy_) + "\n";
  message += "Damage:\n";
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    message += "position: " + std::to_string(pos) + " " +
               std::to_string(damage_[pos]) + "\n";
  }
  message += "\n";
  message += "Effect:\n";
  for (int pos = 0; pos < POSITION_NUM; pos++) {
    message += "position: " + std::to_string(pos) + " " +
               std::to_string(effect_[pos]) + "\n";
  }
  message += "\n";
  message += "Dodge Position: " + std::to_string(dodge_position_) + "\n";
  message += "Type: " + std::to_string(type_) + "\n";
  return message;
}

}  // namespace Game