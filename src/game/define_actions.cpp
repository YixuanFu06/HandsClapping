#include "define_actions.h"

#include "action.h"

namespace Game {

void InitActions() {
  DefineAction(-1, 0, PRODUCE);
  DefineAction(1, 1, ActionType::ATTACK, PISTOL);
  DefineAction(4, {0, 1, 0, 1, 2, 1, 0, 1, 0}, ActionType::ATTACK, CHOP_CROSS);
  DefineAction(4, 1, {0, 1, 2, 3, 4, 5}, ActionType::ATTACK, ICEAGE);
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  std::vector<float> effect,
                  ActionType type,
                  uint32_t id) {
  actions.push_back(Action(energy, damage, effect, type, id));
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  ActionType type,
                  uint32_t id) {
  actions.push_back(Action(energy, damage, type, id));
}

void DefineAction(float energy,
                  float damage,
                  float effect,
                  ActionType type,
                  uint32_t id) {
  actions.push_back(Action(energy, damage, effect, type, id));
}

void DefineAction(float energy, float damage, ActionType type, uint32_t id) {
  actions.push_back(Action(energy, damage, type, id));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> damage_range,
                  float effect,
                  std::vector<uint32_t> effect_range,
                  ActionType type,
                  uint32_t id) {
  actions.push_back(
      Action(energy, damage, damage_range, effect, effect_range, type, id));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> range,
                  ActionType type,
                  uint32_t id) {
  actions.push_back(Action(energy, damage, range, type, id));
}

void DefineAction(float dodge_position_, uint32_t id_) {
  actions.push_back(Action(dodge_position_, id_));
}

void DefineAction(float energy, float damage, uint32_t id) {
  actions.push_back(Action(energy, damage, id));
}

}  // namespace Game