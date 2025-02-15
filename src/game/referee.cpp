#include "referee.h"

namespace Game {

void Referee::JudgeBattle(Player *player) {
  // special cases in single-person actions are dealt in
  // BattleField::HealthUpdate(uint32_t mode); special cases in multi-person
  // actions are dealt in this function

  switch (player->GetAction()->GetType()) {
    case DEFEND: {
      float total_damage = 0;
      float total_effect = 0;
      for (ActionLog &it : action_log_) {
        if (it.owner_ == player || it.action_->GetType() != ATTACK ||
            it.target_ != player->GetName()) {
          continue;
        }
        total_damage += it.action_->GetDamage(player->GetPosition());
        total_effect += it.action_->GetEffect(player->GetPosition());
      }

      /************************************ Special case: REBOUNDER
       ******************************************/
      if (player->GetAction()->GetId() == REBOUNDER && total_effect <= 1) {
        for (ActionLog &it : action_log_) {
          if (it.action_->GetType() == ATTACK &&
              it.target_ == player->GetName() &&
              it.action_->GetEffect(player->GetPosition()) > 0) {
            DamageLogAdd(it.owner_,
                         it.action_->GetDamage(it.owner_->GetPosition()),
                         it.action_->GetEffect(it.owner_->GetPosition()));
          }
        }
        break;
      }
      /******************************************************************************************************/

      DamageLogAdd(
          player, total_damage,
          total_effect + player->GetAction()->GetEffect(player->GetPosition()));
      break;
    }
    case ATTACK: {
      float total_damage = 0;
      float total_effect = 0;
      std::vector<std::pair<const Player *, std::pair<float, float>>> defenders;
      for (const Player &it : players_) {
        if (it.GetName() == player->GetName()) {
          continue;
        }
        defenders.push_back(std::make_pair(
            &it, std::make_pair(
                     -(player->IsAimedAt(&it) *
                       player->GetAction()->GetEffect(player->GetPosition())),
                     0.0f)));
      }

      for (ActionLog &it : action_log_) {
        if (it.action_->GetType() != ATTACK ||
            it.target_ != player->GetName()) {
          continue;
        }
        bool IsDefended = false;
        for (auto &defender : defenders) {
          if (defender.first->GetName() == it.owner_->GetName()) {
            defender.second.first +=
                it.action_->GetEffect(player->GetPosition());
            defender.second.second +=
                it.action_->GetDamage(player->GetPosition());
            IsDefended = true;
          }
        }
        if (!IsDefended) {
          defenders.push_back(std::make_pair(
              it.owner_,
              std::make_pair(it.action_->GetEffect(player->GetPosition()),
                             it.action_->GetDamage(player->GetPosition()))));
        }
      }

      for (auto &defender : defenders) {
        /********************************** Special case: HAMMER < VISION
         *************************************/
        if (defender.second.first == 0 &&
            player->GetAction()->GetId() == HAMMER) {
          for (Player &it : players_) {
            if (it.GetName() == defender.first->GetName() &&
                it.GetAction()->GetId() == VISION) {
              DamageLogAdd(player, player->IsAimedAt(&it),
                           player->IsAimedAt(&it));
            }
          }
        }
        /******************************************************************************************************/

        total_effect += std::min(std::max(defender.second.first, 0.0f),
                                 defender.second.second);
        total_damage += defender.second.second;
      }
      DamageLogAdd(player, total_damage, total_effect);
      break;
    }
    case EQUIP:
    case DODGE: {
      /************************************* Special case: SUICIDE
       *****************************************/
      if (player->GetAction()->GetId() == SUICIDE) {
        bool IsAttacked = false;
        for (ActionLog &it : action_log_) {
          if (it.action_->GetType() == ATTACK &&
              it.target_ == player->GetName() &&
              it.action_->GetEffect(player->GetPosition()) > 0 &&
              it.action_->GetId() != BLACKHOLE &&
              it.action_->GetId() != DOOMSDAY) {
            DamageLogAdd(it.owner_,
                         it.action_->GetDamage(it.owner_->GetPosition()) *
                             player->GetTarget(0).second,
                         it.action_->GetEffect(it.owner_->GetPosition()) *
                             player->GetTarget(0).second);
            IsAttacked = true;
          } else if (it.action_->GetId() == BLACKHOLE ||
                     it.action_->GetId() == DOOMSDAY) {
            DamageLogAdd(player, it.action_->GetDamage(player->GetPosition()),
                         it.action_->GetDamage(player->GetPosition()));
          } else if (it.action_->GetId() == REBOUNDER) {
            DamageLogAdd(it.owner_, player->GetTarget(0).second,
                         player->GetTarget(0).second);
            IsAttacked = true;
          }
        }
        if (!IsAttacked) {
          DamageLogAdd(player, player->GetTarget(0).second,
                       player->GetTarget(0).second);
        }
        break;
      }
      /******************************************************************************************************/

      float total_damage = 0;
      float total_effect = 0;
      for (ActionLog &it : action_log_) {
        if (it.owner_ == player || it.action_->GetType() != ATTACK ||
            it.target_ != player->GetName()) {
          continue;
        }
        total_damage += it.action_->GetDamage(player->GetPosition());
        total_effect += it.action_->GetEffect(player->GetPosition());
      }
      DamageLogAdd(player, total_damage, total_effect);
      break;
    }
  }
}

void Referee::DamageCommit() {
  for (DamageLog &it : damage_log_) {
    float damage = std::max(std::min(it.damage_, it.effect_), 0.0f);
    it.object_->SetHealth(it.object_->GetHealth() - damage);
    if (it.object_->GetHealth() <= 0) {
      if (it.object_->GetAction()->GetId() == SUICIDE) {
        it.object_->GoDie(SUICIDED);
      } else {
        it.object_->GoDie(KILLED);
      }
    }
  }
}

}  // namespace Game