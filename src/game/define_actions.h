#pragma once

#include "action.h"

namespace Game {

enum ActionName { PRODUCE, PISTOL, CHOP_CROSS, ICEAGE };

std::vector<Action> actions;

void InitActions();

void DefineAction(float energy,
                  std::vector<float> damage,
                  std::vector<float> effect,
                  ActionType type,
                  uint32_t id);
void DefineAction(float energy,
                  std::vector<float> damage,
                  ActionType type,
                  uint32_t id);

void DefineAction(float energy,
                  float damage,
                  float effect,
                  ActionType type,
                  uint32_t id);
void DefineAction(float energy, float damage, ActionType type, uint32_t id);

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> damage_range,
                  float effect,
                  std::vector<uint32_t> effect_range,
                  ActionType type,
                  uint32_t id);
void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> range,
                  ActionType type,
                  uint32_t id);

void DefineAction(float dodge_position_, uint32_t id_);

void DefineAction(float energy, float damage, uint32_t id);

}  // namespace Game