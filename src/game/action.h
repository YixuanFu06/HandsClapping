#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace Game {

extern uint32_t DIMENSION;
extern uint32_t POSITION_NUM;
extern uint32_t CENTER;

enum ActionType { ATTACK, DEFEND, DODGE, EQUIP };

enum TargetType { ALL, SINGLE, SELF };

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
  std::vector<std::string> nicknames_;

  ActionType type_;
  TargetType target_type_;
  uint32_t id_;

 public:
  Action(float energy,
         std::vector<float> damage,
         std::vector<float> effect,
         ActionType type,
         TargetType target_type,
         uint32_t id,
         std::vector<std::string> nicknames);
  Action(float energy,
         std::vector<float> damage,
         ActionType type,
         TargetType target_type,
         uint32_t id,
         std::vector<std::string> nicknames);

  Action(float energy,
         float damage,
         float effect,
         ActionType type,
         TargetType target_type,
         uint32_t id,
         std::vector<std::string> nicknames);
  Action(float energy,
         float damage,
         ActionType type,
         TargetType target_type,
         uint32_t id,
         std::vector<std::string> nicknames);

  Action(float energy,
         float damage,
         std::vector<uint32_t> damage_range,
         float effect,
         std::vector<uint32_t> effect_range,
         ActionType type,
         TargetType target_type,
         uint32_t id,
         std::vector<std::string> nicknames);
  Action(float energy,
         float damage,
         std::vector<uint32_t> range,
         ActionType type,
         TargetType target_type,
         uint32_t id,
         std::vector<std::string> nicknames);

  Action(float energy,
         float effect,
         uint32_t id,
         std::vector<std::string> nicknames);

  Action(PlayerPosition dodge_position,
         uint32_t id,
         std::vector<std::string> nicknames);

  Action(float energy, uint32_t id, std::vector<std::string> nicknames);

  inline float GetEnergy() {
    return energy_;
  }
  inline void SetEnergy(float energy) {
    energy_ = energy;
  }

  inline float GetDamage(int position) {
    return damage_[position];
  }

  inline float GetEffect(int position) {
    return effect_[position];
  }

  inline PlayerPosition GetDodgePosition() {
    return dodge_position_;
  }

  inline std::string GetFormalName() {
    return nicknames_[0];
  }

  inline std::string GetNickName(int index) {
    if (index < nicknames_.size()) {
      return nicknames_[index];
    }
    return nicknames_[0];
  }

  inline std::vector<std::string> GetNicknames() {
    return nicknames_;
  }

  inline ActionType GetType() {
    return type_;
  }

  inline TargetType GetTargetType() {
    return target_type_;
  }

  inline uint32_t GetId() {
    return id_;
  }

  void PrintAction();

  std::string GetActionMessage();
};

}  // namespace Game