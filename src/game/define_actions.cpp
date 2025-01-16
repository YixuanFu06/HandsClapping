#include "define_actions.h"

#include <cmath>

#include "action.h"
#include "player.h"

namespace Game {

void InitActions() {
  DefineAction(0, 0, NONE);
  DefineAction(-1, PRODUCE);
  DefineAction(1, 1, ATTACK, PISTOL);
  DefineAction(2, 1, {3, 4, 5}, ATTACK, CHOP_HORIZONTAL);
  DefineAction(2, 1, {1, 4, 7}, ATTACK, CHOP_VERTICAL);
  DefineAction(2, 1, {0, 4, 8}, ATTACK, CHOP_INCLINE1);
  DefineAction(2, 1, {2, 4, 6}, ATTACK, CHOP_INCLINE2);
  DefineAction(2, 1, {0, 1, 2}, ATTACK, CHOP_UPPER);
  DefineAction(2, 1, {6, 7, 8}, ATTACK, CHOP_LOWER);
  DefineAction(2, 1, {0, 3, 6}, ATTACK, CHOP_LEFT);
  DefineAction(2, 1, {2, 5, 8}, ATTACK, CHOP_RIGHT);
  DefineAction(2, 1, {0, 1, 2, 4}, 1, {0, 1, 2, 4}, ATTACK, VISION);
  DefineAction(2, 1, {4, 6, 7, 8}, 1, {4, 6, 7, 8}, ATTACK, HAMMER);
  DefineAction(4, {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
               ATTACK, HAWKEYE);
  DefineAction(4, {0, 1, 0, 1, 2, 1, 0, 1, 0}, ATTACK, CHOP_CROSS);
  DefineAction(4, 1, {0, 1, 2, 3, 4, 5}, ATTACK, ICEAGE);
  DefineAction(4, 1, {3, 4, 5, 6, 7, 8}, ATTACK, EARTHQUAKE);
  DefineAction(5, std::pow(10, 5), {0, 1, 2, 3, 4, 5, 6, 7, 8}, std::pow(10, 5),
               {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, EDITH);
  DefineAction(10, std::pow(10, 10), {0, 1, 2, 3, 4, 5, 6, 7, 8},
               std::pow(10, 10), {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, DOOMSDAY);
  DefineAction(0, -1, SHIELD);
  DefineAction(1, -2, REMOTE_SHIELD);
  DefineAction(1, -2, REBOUNDER);
  DefineAction(UpLeft, DODGE_UPLEFT);
  DefineAction(UpMid, DODGE_UPMID);
  DefineAction(UpRight, DODGE_UPRIGHT);
  DefineAction(GroundLeft, DODGE_GROUNDLEFT);
  DefineAction(GroundMid, DODGE_GROUNDMID);
  DefineAction(GroundRight, DODGE_GROUNDRIGHT);
  DefineAction(DownLeft, DODGE_DOWNLEFT);
  DefineAction(DownMid, DODGE_DOWNMID);
  DefineAction(DownRight, DODGE_DOWNRIGHT);
  DefineAction(0, SUICIDE);
  DefineAction(2, ARTIFACT_SOUL);
  DefineAction(2, ARTIFACT_TIME);
  DefineAction(2, ARTIFACT_SPACE);
  DefineAction(2, ARTIFACT_MIND);
  DefineAction(4, ARTIFACT_REALITY);
  DefineAction(3, ARTIFACT_STRENGTH);
  DefineAction(3, DUPLICATOR);
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

void DefineAction(float energy, float effect, uint32_t id) {
  actions.push_back(Action(energy, effect, id));
}

void DefineAction(PlayerPosition dodge_position, uint32_t id) {
  actions.push_back(Action(dodge_position, id));
}

void DefineAction(float energy, uint32_t id) {
  actions.push_back(Action(energy, id));
}

}  // namespace Game