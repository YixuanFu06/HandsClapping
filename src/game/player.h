#pragma once

#include <cstdint>

#include "define_actions.h"

namespace Game {

enum DeathType {
  TIMEOUTED,
  EXHAUSTED,
  KILLED,
  SUICIDED,
  ATTACK_REBOUNDED,
  BACKFIRED
};

class Player {
 private:
  float health_;
  float energy_;
  std::string name_;
  bool ShouldDie_{false};
  DeathType death_type_;

  PlayerPosition position_;
  ActionName action_name_;
  Action *action_;
  std::vector<std::pair<std::string, uint32_t>> targets_;

 public:
  Player(float health,
         float energy,
         std::string name,
         PlayerPosition position,
         ActionName action_name,
         Action *action);
  Player(float health, float energy, std::string name, PlayerPosition position);

  inline float GetHealth() {
    return health_;
  }
  inline void SetHealth(float health) {
    health_ = health;
  }

  inline float GetEnergy() {
    return energy_;
  }
  inline void SetEnergy(float energy) {
    energy_ = energy;
  }

  inline std::string GetName() const {
    return name_;
  }
  inline void SetName(std::string name) {
    name_ = name;
  }

  inline void GoDie(DeathType death_type) {
    ShouldDie_ = true;
    death_type_ = death_type;
  }
  inline void GoDie() {
    ShouldDie_ = true;
  }
  inline bool IsDead() {
    return ShouldDie_;
  }
  inline DeathType GetDeathType() {
    return death_type_;
  }
  inline void SetDeathType(DeathType death_type) {
    death_type_ = death_type;
  }

  inline PlayerPosition GetPosition() {
    return position_;
  }
  inline void SetPosition(PlayerPosition position) {
    position_ = position;
  }
  inline void SetPosition() {
    position_ = static_cast<PlayerPosition>(action_->GetDodgePosition());
  }

  inline ActionName GetActionName() {
    return action_name_;
  }
  inline void SetActionName(ActionName action_name) {
    action_name_ = action_name;
  }

  inline Action *GetAction() {
    return action_;
  }
  inline void SetAction(Action *action) {
    action_ = action;
  }
  void SetAction();

  inline void AddTarget(std::string target_id, uint32_t repeated_times) {
    targets_.push_back(std::make_pair(target_id, repeated_times));
  }

  inline std::vector<std::pair<std::string, uint32_t>> GetTargets() {
    return targets_;
  }

  inline std::pair<std::string, uint32_t> GetTarget(uint32_t target_id) {
    return targets_[target_id];
  }

  inline uint32_t GetTargetNum() {
    return targets_.size();
  }

  inline void ClearTargets() {
    targets_.clear();
  }

  void PrintPlayer(uint32_t type = 0);  // type = 0 for normal, 1 for detailed

  std::string GetPlayerMessage(
      uint32_t type = 0);  // type = 0 for normal, 1 for detailed

  uint32_t IsAimedAt(const Player *target);
};

}  // namespace Game