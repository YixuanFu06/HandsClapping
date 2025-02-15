#pragma once

#include "referee.h"

namespace Game {

class BattleField {
 private:
  std::vector<Player> players_;
  Referee referee_ = Referee(players_);
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
      uint32_t mode = 0);  // mode = 0 for normal, 1 for detailed, 2 for only
                           // names, 3 for referee mode

  std::string GetBattleFieldMessage(
      uint32_t mode = 0);  // mode = 0 for normal, 1 for detailed, 2 for only
                           // names, 3 for referee mode when printing

  void BattleFieldUpdate(
      uint32_t mode = 0);  // mode = 0 for normal, 1 for detailed, 2 for only
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