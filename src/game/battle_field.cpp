#include "battle_field.h"

#include <bits/stdint-uintn.h>

#include <algorithm>

#include "action.h"
#include "define_actions.h"
#include "player.h"

namespace Game {

void BattleField::DecodeInputString(std::string &player_action_name,
                                    std::string &current_action_name,
                                    std::string &target_name,
                                    uint32_t &repeated_times) {
  std::string sub_string;
  size_t pos = player_action_name.find(',');
  if (pos != std::string::npos) {
    sub_string = player_action_name.substr(0, pos);
    player_action_name.erase(0, pos + 1);
  } else {
    sub_string = player_action_name;
    player_action_name.clear();
  }

  std::string repeated_times_string = "";
  while (sub_string.size() > 0 && std::isdigit(sub_string[0])) {
    repeated_times_string += sub_string[0];
    sub_string.erase(0, 1);
  }
  if (repeated_times_string.size() > 0) {
    repeated_times = std::stoi(repeated_times_string);
  } else {
    repeated_times = 1;
  }

  pos = sub_string.find("->");
  if (pos != std::string::npos) {
    current_action_name = sub_string.substr(0, pos);
    sub_string.erase(0, pos + 2);
    if (sub_string[0] != '#') {
      bool IsFound = false;
      for (uint32_t i = 0; i < players_.size(); i++) {
        if (players_[i].GetName() == sub_string) {
          target_name = sub_string;
          IsFound = true;
          break;
        }
      }
      if (!IsFound) {
        std::cout << "Error: target name " << sub_string
                  << " is not found. Ignored here." << std::endl;
        target_name = "#NONE";
      }
    } else {
      sub_string.erase(0, 1);
      if (std::isdigit(sub_string[0])) {
        uint32_t target_id = std::stoi(sub_string) - 1;
        if (target_id < players_.size()) {
          target_name = players_[target_id].GetName();
        } else {
          std::cout << "Error: target id " << sub_string
                    << " is out of range. Ignored here." << std::endl;
          target_name = "#NONE";
        }
      } else {
        std::cout << "Error: target name " << sub_string
                  << " is not a number. Ignored here." << std::endl;
        target_name = "#NONE";
      }
    }
  } else {
    current_action_name = sub_string;
    sub_string.clear();
    target_name = "#ALL";
  }
}

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
      std::cout << "The " << turn_ << "th turn. " << member_num_
                << " players: \n";
      for (uint32_t i = 0; i < players_.size(); i++) {
        players_[i].PrintPlayer(1);
      }
      std::cout << std::endl;
      break;
    case 2:
      std::cout << std::endl
                << "The " << turn_ << "th turn. " << member_num_
                << " players: ";
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
      std::cout << "The " << turn_ << "th turn. " << member_num_
                << " players: \n";
      for (uint32_t i = 0; i < players_.size(); i++) {
        players_[i].PrintPlayer(0);
      }
      std::cout << std::endl;
  }
}

std::string BattleField::GetBattleFieldMessage(uint32_t type) {
  std::string output;
  switch (type) {
    case 1:
      output += "The " + std::to_string(turn_) + "th turn. " +
                std::to_string(member_num_) + " players: \n";
      for (uint32_t i = 0; i < players_.size(); i++) {
        output += players_[i].GetPlayerMessage(1);
      }
      output += "\n";
      break;
    case 2:
      output += "\nThe " + std::to_string(turn_) + "th turn. " +
                std::to_string(member_num_) + " players: ";
      for (uint32_t i = 0; i < players_.size(); i++) {
        output += players_[i].GetName() + " ";
      }
      output += "\n";
      break;
    case 3:
      for (uint32_t i = 0; i < players_.size(); i++) {
        output += players_[i].GetPlayerMessage(3);
      }
      output += "\n";
      break;
    case 0:
    default:
      output += "The " + std::to_string(turn_) + "th turn. " +
                std::to_string(member_num_) + " players: \n";
      for (uint32_t i = 0; i < players_.size(); i++) {
        output += players_[i].GetPlayerMessage(0);
      }
      output += "\n";
  }
  return output;
}

void BattleField::BattleFieldUpdate(uint32_t type) {
  switch (type) {
    case 1: {
      TurnUpdate();
      PrintBattleField(1);
      ActionUpdate();
      PrintBattleField(1);
      PositionUpdate();
      EnergyUpdate();
      HealthUpdate();
      MemberNumUpdate();
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
      TurnUpdate();
      PrintBattleField(2);
      ActionUpdate();
      PrintBattleField(type);
      PositionUpdate();
      EnergyUpdate();
      HealthUpdate();
      MemberNumUpdate();
  }
}

void BattleField::BattleFieldUpdate(std::vector<std::string> player_actions) {
  ActionUpdate(player_actions);
  PrintBattleField(0);
  PositionUpdate();
  EnergyUpdate();
  HealthUpdate();
  MemberNumUpdate();
}

void BattleField::ActionUpdate() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].ClearTargets();
    std::cout << players_[i].GetName() << "'s action name: ";
    std::string player_action_name;
    std::cin >> player_action_name;
    ActionName action = NONE;

    bool IsFound = false;
    while (player_action_name.size() > 0) {
      std::string current_action_name;
      std::string target_id;
      uint32_t repeated_times;
      DecodeInputString(player_action_name, current_action_name, target_id,
                        repeated_times);

      ActionName current_action;
      bool current_IsFound = false;
      for (Action &it : actions) {
        for (std::string &nickname : it.GetNicknames()) {
          if (nickname == current_action_name) {
            current_action = static_cast<ActionName>(it.GetId());
            current_IsFound = true;
            break;
          }
        }
      }
      if (!current_IsFound) {
        std::cout << "Error: action " << current_action_name
                  << " not found. Ignored here." << std::endl;
      } else if (action != NONE && action != current_action) {
        std::cout << "Error: action " << current_action_name
                  << " is different from previous actions. The whole input is "
                     "ignored."
                  << std::endl;
        break;
      } else {
        action = current_action;
        players_[i].SetActionName(action);
        players_[i].AddTarget(target_id, repeated_times);
        IsFound = true;
      }
    }

    if (!IsFound) {
      std::cout << "No valid action found. Set as NONE." << std::endl;
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].SetAction();
  }
  RemoveDead();
}

void BattleField::ActionUpdate(std::vector<std::string> player_actions) {
  if (player_actions.size() > players_.size()) {
    std::cout << "Error: too many actions. Only the first " << players_.size()
              << " actions are used." << std::endl;
  } else if (player_actions.size() < players_.size()) {
    std::cout << "Error: too few actions. The last "
              << players_.size() - player_actions.size()
              << " players' actions are set as NONE." << std::endl;
    for (uint32_t i = player_actions.size(); i < players_.size(); i++) {
      player_actions.push_back("NONE");
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].ClearTargets();
    ActionName action = NONE;
    std::string player_action_name = player_actions[i];

    bool IsFound = false;
    while (player_action_name.size() > 0) {
      std::string current_action_name;
      std::string target_id;
      uint32_t repeated_times;
      DecodeInputString(player_action_name, current_action_name, target_id,
                        repeated_times);

      ActionName current_action;
      bool current_IsFound = false;
      for (Action &it : actions) {
        for (std::string &nickname : it.GetNicknames()) {
          if (nickname == current_action_name) {
            current_action = static_cast<ActionName>(it.GetId());
            current_IsFound = true;
            break;
          }
        }
      }
      if (!current_IsFound) {
        std::cout << "Error: action " << current_action_name
                  << " not found. Ignored here." << std::endl;
      } else if (action != NONE && action != current_action) {
        std::cout << "Error: action " << current_action_name
                  << " is different from previous actions. The whole input is "
                     "ignored."
                  << std::endl;
        break;
      } else {
        action = current_action;
        players_[i].SetActionName(action);
        players_[i].AddTarget(target_id, repeated_times);
        IsFound = true;
      }
    }

    if (!IsFound) {
      std::cout << "No valid action found. Set as NONE." << std::endl;
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].SetAction();
  }
  RemoveDead();
}

void BattleField::PositionUpdate() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].SetPosition();
  }
}

void BattleField::EnergyUpdate() {
  for (uint32_t i = 0; i < players_.size(); i++) {
    for (uint32_t j = 0; j < players_[i].GetTargets().size(); j++) {
      if (players_[i].GetTargets()[j].first == "#NONE") {
        continue;
      } else if (players_[i].GetTargets()[j].first == "#ALL") {
        switch (players_[i].GetAction()->GetTargetType()) {
          case SINGLE:
            players_[i].SetEnergy(players_[i].GetEnergy() -
                                  players_[i].GetAction()->GetEnergy() *
                                      (GetMemberNum() - 1) *
                                      players_[i].GetTargets()[j].second);
            break;
          case ALL:
          case SELF:
            players_[i].SetEnergy(players_[i].GetEnergy() -
                                  players_[i].GetAction()->GetEnergy() *
                                      players_[i].GetTargets()[j].second);
            break;
        }
      } else {
        players_[i].SetEnergy(players_[i].GetEnergy() -
                              players_[i].GetAction()->GetEnergy() *
                                  players_[i].GetTargets()[j].second);
      }
    }

    if (players_[i].GetEnergy() < 0) {
      players_[i].SetHealth(0);
      players_[i].GoDie(EXHAUSTED);
    }
    if (players_[i].GetAction()->GetFormalName() == "TIMEOUT") {
      players_[i].SetHealth(0);
      players_[i].GoDie(TIMEOUTED);
    }
  }
  RemoveDead();
}

void BattleField::HealthUpdate() {
  // single person actions update
  for (uint32_t i = 0; i < players_.size(); i++) {
    switch (players_[i].GetAction()->GetId()) {
      case DUPLICATOR:
        for (uint32_t j = 0; j < players_[i].GetTargets().size(); j++) {
          players_[i].SetEnergy(players_[i].GetEnergy() +
                                5 * players_[i].GetTargets()[j].second);
        }
        break;
      case ARTIFACT_SOUL:
        for (uint32_t j = 0; j < players_[i].GetTargets().size(); j++) {
          players_[i].SetHealth(players_[i].GetHealth() +
                                1 * players_[i].GetTargets()[j].second);
        }
        break;
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    for (uint32_t j = 0; j < players_[i].GetTargets().size(); j++) {
      if (players_[i].GetTargets()[j].first == "#NONE") {
        continue;
      } else if (players_[i].GetTargets()[j].first == "#ALL") {
        for (uint32_t k = 0; k < players_[i].GetTargets()[j].second; k++) {
          for (Player &it : players_) {
            if (it.GetName() != players_[i].GetName()) {
              referee_.ActionLogAdd(&players_[i], players_[i].GetAction(),
                                    it.GetName());
            }
          }
        }
      } else {
        for (uint32_t k = 0; k < players_[i].GetTargets()[j].second; k++) {
          referee_.ActionLogAdd(&players_[i], players_[i].GetAction(),
                                players_[i].GetTargets()[j].first);
        }
      }
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    referee_.JudgeBattle(players_, &players_[i]);
  }

  referee_.ActionLogClear();

  referee_.DamageCommit();
  referee_.DamageLogClear();

  RemoveDead();
}

void Referee::JudgeBattle(std::vector<Player> &player_list, Player *player) {
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
      // special cases: REBOUNDER
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
      DamageLogAdd(
          player, total_damage,
          total_effect + player->GetAction()->GetEffect(player->GetPosition()));
    } break;
    case ATTACK: {
      float total_damage = 0;
      float total_effect = 0;
      std::vector<std::pair<const Player *, std::pair<float, float>>> defenders;
      for (const Player &it : player_list) {
        if (it.GetName() == player->GetName()) {
          continue;
        }
        defenders.push_back(std::make_pair(
            &it, std::make_pair(
                     -(player->IsAimedAt(&it) *
                       player->GetAction()->GetEffect(player->GetPosition())),
                     0)));
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
        /*
        // special cases: VISION > HAMMER
        if (it.action_->GetId() == VISION &&
            player->GetAction()->GetId() == HAMMER) {
          total_effect +=
              it.action_->GetEffect(player->GetPosition());
        } else {
          total_effect +=
              std::max(it.action_->GetEffect(player->GetPosition()) -
                          player->GetAction()->GetEffect(player->GetPosition()),
                      0.0f);
        }
        */
      }

      for (auto &defender : defenders) {
        // special cases: VISION > HAMMER
        if (defender.second.first == 0 &&
            player->GetAction()->GetId() == HAMMER) {
          for (Player &it : player_list) {
            if (it.GetName() == defender.first->GetName() &&
                it.GetAction()->GetId() == VISION) {
              DamageLogAdd(player, player->IsAimedAt(&it),
                           player->IsAimedAt(&it));
            }
          }
        }

        total_effect += std::min(std::max(defender.second.first, 0.0f),
                                 defender.second.second);
        total_damage += defender.second.second;
      }
      DamageLogAdd(player, total_damage, total_effect);
    } break;
    case EQUIP:
    case DODGE: {
      // special cases: SUICIDE
      if (player->GetAction()->GetId() == SUICIDE) {
        bool IsAttacked = false;
        for (ActionLog &it : action_log_) {
          if (it.action_->GetType() == ATTACK &&
              it.target_ == player->GetName() &&
              it.action_->GetEffect(player->GetPosition()) > 0 &&
              it.action_->GetId() != BLACKHOLE &&
              it.action_->GetId() != DOOMSDAY) {
            DamageLogAdd(it.owner_,
                         it.action_->GetDamage(it.owner_->GetPosition()),
                         it.action_->GetEffect(it.owner_->GetPosition()));
            IsAttacked = true;
          } else if (it.action_->GetId() == BLACKHOLE ||
                     it.action_->GetId() == DOOMSDAY) {
            DamageLogAdd(player, it.action_->GetDamage(player->GetPosition()),
                         it.action_->GetDamage(player->GetPosition()));
          } else if (it.action_->GetId() == REBOUNDER) {
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
        if (it.owner_ == player || it.action_->GetType() != ATTACK ||
            it.target_ != player->GetName()) {
          continue;
        }
        total_damage += it.action_->GetDamage(player->GetPosition());
        total_effect += it.action_->GetEffect(player->GetPosition());
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
        case TIMEOUTED:
          std::cout << "being timeout." << std::endl;
          break;
        case EXHAUSTED:
          std::cout << "getting exhausted." << std::endl;
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