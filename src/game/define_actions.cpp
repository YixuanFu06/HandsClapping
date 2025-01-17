#include "define_actions.h"

#include <cmath>
#include <algorithm>

#include "action.h"
#include "player.h"

namespace Game {

std::vector<Action> actions;

std::vector<std::string> AllNicknames;

std::vector<std::string> NickNameProcess(std::vector<std::string> nicknames) {
  std::string lower_nickname = nicknames[0];
  std::transform(lower_nickname.begin(), lower_nickname.end(), lower_nickname.begin(), ::tolower);
  nicknames.insert(nicknames.begin() + 1, lower_nickname);
  std::vector<std::string> new_nicknames;
  for (std::string &nickname : nicknames) {
    if (std::find(AllNicknames.begin(), AllNicknames.end(), nickname) != AllNicknames.end()) {
      std::cout << "Nickname " << nickname << " of action " << nicknames[0] << " is been used. Ignored here.\n";
      continue;
    }
    AllNicknames.push_back(nickname);
    new_nicknames.push_back(nickname);
  }
  return new_nicknames;
}

void InitActions() {
  DefineAction(0, 0, NONE, {"NONE"});
  DefineAction(-1, PRODUCE, {"PRODUCE", "qi", "1", "气"});
  DefineAction(1, 1, ATTACK, PISTOL, {"PISTOL", "dia", "gun"});
/*
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
  TODO: CHOP_X  
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
*/
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  std::vector<float> effect,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, effect, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  float effect,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, effect, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy, float damage, ActionType type, uint32_t id, std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> damage_range,
                  float effect,
                  std::vector<uint32_t> effect_range,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(
      Action(energy, damage, damage_range, effect, effect_range, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> range,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, range, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy, float effect, uint32_t id, std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, effect, id, NickNameProcess(nicknames)));
}

void DefineAction(PlayerPosition dodge_position, uint32_t id, std::vector<std::string> nicknames) {
  actions.push_back(Action(dodge_position, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy, uint32_t id, std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, id, NickNameProcess(nicknames)));
}

}  // namespace Game