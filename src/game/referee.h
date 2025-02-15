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
  std::vector<Player> &players_;

 public:
  Referee(std::vector<Player> &players) : players_(players) {
  }

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
  void JudgeBattle(Player *player);
  void DamageCommit();
};

}  // namespace Game