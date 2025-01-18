#include "battle_field.h"

#include "action.h"
#include "define_actions.h"
#include "player.h"

namespace Game {

BattleField::BattleField(std::vector<std::string> player_names) : turn_(0) {
  for (uint32_t i = 0; i < player_names.size(); i++) {
    AddPlayer(player_names[i], 1.0f, 0.0f, static_cast<PlayerPosition>(CENTER));
  }
  member_num_ = players_.size();
}

BattleField::BattleField() : turn_(0), member_num_(0) {
}

void BattleField::AddPlayer(Player player) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == player.GetName()) {
      std::cout << "Error: player " << player.GetName()
                << " is already in the game" << std::endl;
      return;
    }
  }
  players_.push_back(player);
  member_num_ = players_.size();
}

void BattleField::AddPlayer(std::string name,
                            float health,
                            float energy,
                            PlayerPosition position,
                            ActionName action_name,
                            Action *action) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == name) {
      std::cout << "Error: player " << name << " is already in the game"
                << std::endl;
      return;
    }
  }
  players_.push_back(
      Player(health, energy, name, position, action_name, action));
  member_num_ = players_.size();
}

void BattleField::AddPlayer(std::string name,
                            float health,
                            float energy,
                            PlayerPosition position) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == name) {
      std::cout << "Error: player " << name << " is already in the game"
                << std::endl;
      return;
    }
  }
  players_.push_back(Player(health, energy, name, position));
  member_num_ = players_.size();
}

Player BattleField::GetPlayer(std::string name) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == name) {
      return players_[i];
    }
  }
  return players_[0];
}

uint32_t BattleField::GetPlayerId(std::string name) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == name) {
      return i;
    }
  }
  return 0;
}

std::string BattleField::RemovePlayer(uint32_t player_id) {
  std::string name = players_[player_id].GetName();
  players_.erase(players_.begin() + player_id);
  return name;
}

void BattleField::RemovePlayer(std::string name) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == name) {
      players_.erase(players_.begin() + i);
      break;
    }
  }
}

void BattleField::PrintBattleField(uint32_t type) {
  switch (type) {
    case 1:
      std::cout << "The " << turn_ << " turn. " << member_num_
                << " players: \n";
      for (uint32_t i = 0; i < players_.size(); i++) {
        players_[i].PrintPlayer(1);
      }
      std::cout << std::endl;
      break;
    case 2:
      std::cout << std::endl
                << "The " << turn_ << " turn. " << member_num_ << " players: ";
      for (uint32_t i = 0; i < players_.size(); i++) {
        std::cout << players_[i].GetName() << " ";
      }
      std::cout << std::endl;
      break;
    case 3:
      for (uint32_t i = 0; i < players_.size(); i++) {
        players_[i].PrintPlayer(3);
      }
      std::cout << std::endl;
      break;
    case 0:
    default:
      std::cout << "The " << turn_ << " turn. " << member_num_
                << " players: \n";
      for (uint32_t i = 0; i < players_.size(); i++) {
        players_[i].PrintPlayer(0);
      }
      std::cout << std::endl;
  }
}

void BattleField::BattleFieldUpdate(uint32_t type) {
  switch (type) {
    case 1: {
      turn_++;
      PrintBattleField(1);
      ActionUpdate();
      PrintBattleField(1);
      PositionUpdate();
      EnergyUpdate();
      HealthUpdate();
      member_num_ = players_.size();
    } break;
    case 3: {
      std::vector<float> healths;
      std::vector<float> energies;
      for (uint32_t i = 0; i < players_.size(); i++) {
        healths.push_back(players_[i].GetHealth());
        energies.push_back(players_[i].GetEnergy());
      }
      PrintBattleField(2);
      ActionUpdate();
      PrintBattleField(3);
      PositionUpdate();
      EnergyUpdate();
      HealthUpdate();
      for (uint32_t i = 0; i < players_.size(); i++) {
        std::cout << players_[i].GetName() << ": health: " << std::showpos
                  << (players_[i].GetHealth() - healths[i])
                  << ".  energy: " << std::showpos
                  << (players_[i].GetEnergy() - energies[i]) << ".\n";
        std::cout << std::noshowpos;
      }
    } break;
    case 0:
    case 2:
    default:
      turn_++;
      PrintBattleField(2);
      ActionUpdate();
      PrintBattleField(type);
      PositionUpdate();
      EnergyUpdate();
      HealthUpdate();
      member_num_ = players_.size();
  }
}

void BattleField::ActionUpdate() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    std::cout << players_[i].GetName() << "'s action name: " << std::endl;
    std::string player_action_name;
    std::cin >> player_action_name;
    ActionName action_name = NONE;

    bool IsFound = false;
    for (Action &it : actions) {
      for (std::string &nickname : it.GetNicknames()) {
        if (nickname == player_action_name) {
          action_name = static_cast<ActionName>(it.GetId());
          IsFound = true;
          break;
        }
      }
    }

    if (!IsFound) {
      std::cout << "Error: action " << player_action_name
                << " not found. Set as NONE." << std::endl;
    }
    players_[i].SetActionName(action_name);
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].SetAction();
  }
}

void BattleField::PositionUpdate() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].SetPosition();
  }
}

void BattleField::EnergyUpdate() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].SetEnergy(players_[i].GetEnergy() -
                          players_[i].GetAction()->GetEnergy());
    if (players_[i].GetEnergy() < 0) {
      players_[i].GoDie(EXAUHSTED);
    }
  }
  RemoveDead();
}

void BattleField::HealthUpdate() {
  // single person actions update
  for (uint32_t i = 0; i < players_.size(); i++) {
    switch (players_[i].GetAction()->GetId()) {
      case DUPLICATOR:
        players_[i].SetEnergy(players_[i].GetEnergy() + 5);
        break;
      case ARTIFACT_SOUL:
        players_[i].SetHealth(players_[i].GetHealth() + 1);
        break;
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    referee_.ActionLogAdd(&players_[i], i);
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    referee_.JudgeBattle(&players_[i]);
  }

  referee_.ActionLogClear();

  referee_.DamageCommit();
  referee_.DamageLogClear();

  RemoveDead();
}

void Referee::JudgeBattle(Player *player) {
  switch (player->GetAction()->GetType()) {
    case DEFEND: {
      float total_damage = 0;
      float total_effect = 0;
      for (ActionLog &it : action_log_) {
        if (it.owner_ == player ||
            it.owner_->GetAction()->GetType() != ATTACK) {
          continue;
        }
        total_damage +=
            it.owner_->GetAction()->GetDamage(player->GetPosition());
        total_effect +=
            it.owner_->GetAction()->GetEffect(player->GetPosition());
      }
      // special cases: REBOUNDER
      if (player->GetAction()->GetId() == REBOUNDER && total_effect <= 1) {
        for (ActionLog &it : action_log_) {
          if (it.owner_->GetAction()->GetType() == ATTACK) {
            DamageLogAdd(
                it.owner_,
                it.owner_->GetAction()->GetDamage(it.owner_->GetPosition()),
                it.owner_->GetAction()->GetEffect(it.owner_->GetPosition()));
          }
        }
        break;
      }
      DamageLogAdd(
          player, total_damage,
          total_effect + player->GetAction()->GetEffect(player->GetPosition()));
    } break;
    case ATTACK: {
      float total_damage = 0;
      float total_effect = 0;
      for (ActionLog &it : action_log_) {
        if (it.owner_ == player ||
            it.owner_->GetAction()->GetType() != ATTACK) {
          continue;
        }
        total_damage +=
            it.owner_->GetAction()->GetDamage(player->GetPosition());
        // special cases: VISION > HAMMER
        if (it.owner_->GetAction()->GetId() == VISION &&
            player->GetAction()->GetId() == HAMMER) {
          total_effect +=
              it.owner_->GetAction()->GetEffect(player->GetPosition());
          continue;
        }
        total_effect +=
            std::max(it.owner_->GetAction()->GetEffect(player->GetPosition()) -
                         player->GetAction()->GetEffect(player->GetPosition()),
                     0.0f);
      }
      DamageLogAdd(player, total_damage, total_effect);
    } break;
    case EQUIP:
    case DODGE: {
      // special cases: SUICIDE
      if (player->GetAction()->GetId() == SUICIDE) {
        bool IsAttacked = false;
        for (ActionLog &it : action_log_) {
          if (it.owner_->GetAction()->GetType() == ATTACK &&
              it.owner_->GetAction()->GetId() != DOOMSDAY) {
            DamageLogAdd(
                it.owner_,
                it.owner_->GetAction()->GetDamage(it.owner_->GetPosition()),
                it.owner_->GetAction()->GetEffect(it.owner_->GetPosition()));
            IsAttacked = true;
          } else if (it.owner_->GetAction()->GetId() == DOOMSDAY) {
            DamageLogAdd(
                player,
                it.owner_->GetAction()->GetDamage(player->GetPosition()),
                it.owner_->GetAction()->GetDamage(player->GetPosition()));
          } else if (it.owner_->GetAction()->GetId() == REBOUNDER) {
            DamageLogAdd(it.owner_, 1, 1);
            IsAttacked = true;
          }
        }
        if (!IsAttacked) {
          DamageLogAdd(player, 1, 1);
        }
        break;
      }

      float total_damage = 0;
      float total_effect = 0;
      for (ActionLog &it : action_log_) {
        if (it.owner_ == player ||
            it.owner_->GetAction()->GetType() != ATTACK) {
          continue;
        }
        total_damage +=
            it.owner_->GetAction()->GetDamage(player->GetPosition());
        total_effect +=
            it.owner_->GetAction()->GetEffect(player->GetPosition());
      }
      DamageLogAdd(player, total_damage, total_effect);
    } break;
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

void BattleField::RemoveDead() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].IsDead()) {
      std::cout << players_[i].GetName() << " is dead because of ";
      switch (players_[i].GetDeathType()) {
        case EXAUHSTED:
          std::cout << "getting exauhsted." << std::endl;
          break;
        case KILLED:
          std::cout << "being killed." << std::endl;
          break;
        case SUICIDED:
          std::cout << "commit suicide." << std::endl;
          break;
        case ATTACK_REBOUNDED:
          std::cout << "the attack is rebounded." << std::endl;
          break;
        case BACKFIRED:
          std::cout << "been backfired" << std::endl;
          break;
      }
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].IsDead()) {
      std::string name = RemovePlayer(i--);
      std::cout << name << " is removed from the battle field." << std::endl;
    }
  }
}

}  // namespace Game