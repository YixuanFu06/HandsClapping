#pragma once

#include <bits/stdint-uintn.h>

#include <cstdint>
#include <vector>
#include <cmath>
#include <iostream>

namespace Game {

enum ActionType { ATTACK, DEFEND, DODGE, EQUIP };

uint32_t DIMENSION = 2;
uint32_t POSITION_NUM = std::pow(3, DIMENSION);
uint32_t CENTER = POSITION_NUM / 2;

enum PlayerPosition {
  UpLeft = 0,
  UpMid = 1,
  UpRight = 2,
  GroundLeft = 3,
  GroundMid = 4,
  GroundRight = 5,
  DownLeft = 6,
  DownMid = 7,
  DownRight = 8
};

class Action {
 private:
  float energy_;
  std::vector<float> damage_;
  std::vector<float> effect_;
  PlayerPosition dodge_position_;

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

  Action(float energy, float effect, uint32_t id);

  Action(PlayerPosition dodge_position, uint32_t id);

  Action(float energy, uint32_t id);

  inline float GetEnergy() { return energy_; }
  inline void SetEnergy(float energy) { energy_ = energy; }

  inline float GetDamage(int position) { return damage_[position]; }

  inline float GetEffect(int position) { return effect_[position]; }

  inline PlayerPosition GetDodgePosition() { return dodge_position_; }

  inline ActionType GetType() { return type_; }

  inline uint32_t GetId() { return id_; }

  void PrintAction();
};

}  // namespace Game