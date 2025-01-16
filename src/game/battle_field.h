#pragma once

#include <queue>

#include "player.h"

namespace Game {

struct ActionLog {
  Player *owner_;
  uint32_t id_;
  ActionLog(Player *owner, uint32_t id) : owner_(owner), id_(id) {}
};

struct DamageLog {
  Player *object_;
  float damage_;
  float effect_;
  DamageLog(Player *object, float damage, float effect) : object_(object), damage_(damage), effect_(effect) {}
};

class Referee {
 private:
  std::vector<ActionLog> action_log_;
  std::vector<DamageLog> damage_log_;

 public:
  inline void ActionLogAdd(Player *player, uint32_t id) { action_log_.push_back(ActionLog(player, id)); }
  inline void DamageLogAdd(Player *object, float damage, float effect) { damage_log_.push_back(DamageLog(object, damage, effect)); }
  void JudgeBattle(Player *player);
  void DamageCommit();
};

class BattleField {
 private:
  std::vector<Player> players_;
  Referee referee_;
  uint32_t turn_;

 public:
  BattleField(std::vector<std::string> player_names);

  void AddPlayer(Player player);
  void AddPlayer(std::string name, float health, float energy, PlayerPosition position, ActionName action_name, Action *action);
  void AddPlayer(std::string name, float health, float energy, PlayerPosition position);

  inline Player GetPlayer(uint32_t player_id) { return players_[player_id]; }
  Player GetPlayer(std::string name);
  inline std::string GetPlayerName(uint32_t player_id) { return players_[player_id].GetName(); }
  uint32_t GetPlayerId(std::string name);

  std::string RemovePlayer(uint32_t player_id);
  void RemovePlayer(std::string name);

  inline uint32_t GetTurn() { return turn_; }
  inline void SetTurn(uint32_t turn) { turn_ = turn; }

  void PrintBattleField(uint32_t type = 0); // type = 0 for normal, 1 for detailed, 2 for only names

  void ActionUpdate();

  void PositionUpdate();

  void EnergyUpdate();

  void HealthUpdate();

  void RemoveDead();
};

}