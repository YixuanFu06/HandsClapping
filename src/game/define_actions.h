#pragma once

#include "action.h"

namespace Game {

enum ActionName {
  NONE,
  PRODUCE,
  PISTOL,
  PISTOL_UPPER,
  PISTOL_LOWER,
  PISTOL_LEFT,
  PISTOL_RIGHT,
  CHOP_HORIZONTAL,
  CHOP_VERTICAL,
  CHOP_INCLINE1,
  CHOP_INCLINE2,
  CHOP_UPPER,
  CHOP_LOWER,
  CHOP_LEFT,
  CHOP_RIGHT,
  VISION,
  HAMMER,
  HAWKEYE,
  CHOP_CROSS,
  CHOP_X,
  ICEAGE,
  EARTHQUAKE,
  EDITH,
  BLACKHOLE,
  DOOMSDAY,
  SHIELD,
  REMOTE_SHIELD,
  REBOUNDER,
  DODGE_UPLEFT,
  DODGE_UPMID,
  DODGE_UPRIGHT,
  DODGE_GROUNDLEFT,
  DODGE_GROUNDRIGHT,
  DODGE_DOWNLEFT,
  DODGE_DOWNMID,
  DODGE_DOWNRIGHT,
  SUICIDE,
  ARTIFACT_SOUL,
  ARTIFACT_TIME,
  ARTIFACT_SPACE,
  ARTIFACT_MIND,
  ARTIFACT_REALITY,
  ARTIFACT_STRENGTH,
  DUPLICATOR,
  TIMEOUT
};

extern std::vector<Action> actions;

void InitActions();

void DefineAction(float energy,
                  std::vector<float> damage,
                  std::vector<float> effect,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames);
void DefineAction(float energy,
                  std::vector<float> damage,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames);

void DefineAction(float energy,
                  float damage,
                  float effect,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames);
void DefineAction(float energy,
                  float damage,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames);

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> damage_range,
                  float effect,
                  std::vector<uint32_t> effect_range,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames);
void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> range,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames);

void DefineAction(float energy,
                  float effect,
                  uint32_t id,
                  std::vector<std::string> nicknames);

void DefineAction(PlayerPosition dodge_position,
                  uint32_t id,
                  std::vector<std::string> nicknames);

void DefineAction(float energy,
                  uint32_t id,
                  std::vector<std::string> nicknames);

}  // namespace Game