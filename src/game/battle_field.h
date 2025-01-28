#pragma once

#include "player.h"

namespace Game {

struct ActionLog {
  Player *owner_;
  Action *action_;
  std::string target_;
  uint32_t id_;
  ActionLog(Player *owner, Action *action, std::string target)
      : owner_(owner), action_(action), target_(target) {
  }
};

struct DamageLog {
  Player *object_;
  float damage_;
  float effect_;
  DamageLog(Player *object, float damage, float effect)
      : object_(object), damage_(damage), effect_(effect) {
  }
};

class Referee {
 private:
  std::vector<ActionLog> action_log_;
  std::vector<DamageLog> damage_log_;

 public:
  inline void ActionLogAdd(Player *player, Action *action, std::string target) {
    action_log_.push_back(ActionLog(player, action, target));
  }
  inline void DamageLogAdd(Player *object, float damage, float effect) {
    damage_log_.push_back(DamageLog(object, damage, effect));
  }
  inline void ActionLogClear() {
    action_log_.clear();
  }
  inline void DamageLogClear() {
    damage_log_.clear();
  }
  void JudgeBattle(std::vector<Player> &player_list, Player *player);
  void DamageCommit();
};

class BattleField {
 private:
  std::vector<Player> players_;
  Referee referee_;
  uint32_t turn_;
  uint32_t member_num_;

 public:
  BattleField();
  BattleField(std::vector<std::string> player_names);

  void AddPlayer(Player player);
  void AddPlayer(std::string name,
                 float health,
                 float energy,
                 PlayerPosition position,
                 ActionName action_name,
                 Action *action);
  void AddPlayer(std::string name,
                 float health,
                 float energy,
                 PlayerPosition position);

  inline Player GetPlayer(uint32_t player_id) {
    return players_[player_id];
  }
  Player GetPlayer(std::string name);
  inline std::string GetPlayerName(uint32_t player_id) {
    return players_[player_id].GetName();
  }
  uint32_t GetPlayerId(std::string name);

  std::string RemovePlayer(uint32_t player_id);
  void RemovePlayer(std::string name);

  inline uint32_t GetTurn() {
    return turn_;
  }
  inline void SetTurn(uint32_t turn) {
    turn_ = turn;
  }
  inline void TurnUpdate() {
    turn_++;
  }

  inline uint32_t GetMemberNum() {
    return member_num_;
  }

  void PrintBattleField(
      uint32_t type = 0);  // type = 0 for normal, 1 for detailed, 2 for only
                           // names, 3 for referee mode

  std::string GetBattleFieldMessage(
      uint32_t type = 0);  // type = 0 for normal, 1 for detailed, 2 for only
                           // names, 3 for referee mode when printing

  void BattleFieldUpdate(
      uint32_t type = 0);  // type = 0 for normal, 1 for detailed, 2 for only
                           // names, 3 for referee mode when printing

  inline float GetPlayerHealth(uint32_t player_id) {
    return players_[player_id].GetHealth();
  }

  inline void SetPlayerHealth(uint32_t player_id, float health) {
    players_[player_id].SetHealth(health);
  }

  inline float GetPlayerEnergy(uint32_t player_id) {
    return players_[player_id].GetEnergy();
  }

  inline void SetPlayerEnergy(uint32_t player_id, float energy) {
    players_[player_id].SetEnergy(energy);
  }

  void BattleFieldUpdate(
      std::vector<std::string> player_actions,
      uint32_t mode = 0);  // 0 for default and 1 for slient mode

  void ActionUpdate(uint32_t mode = 0);  // 0 for default and 1 for slient mode

  void ActionUpdate(std::vector<std::string> player_actions,
                    uint32_t mode = 0);  // 0 for default and 1 for slient mode

  void PositionUpdate();

  void EnergyUpdate(uint32_t mode = 0);  // 0 for default and 1 for slient mode

  void HealthUpdate(uint32_t mode = 0);  // 0 for default and 1 for slient mode

  inline void MemberNumUpdate() {
    member_num_ = players_.size();
  }

  void RemoveDead(uint32_t mode = 0);  // 0 for default and 1 for slient mode

  void DecodeInputString(std::string &player_action_name,
                         std::string &current_action_name,
                         std::string &target_name,
                         uint32_t &repeated_times);
};

}  // namespace Game