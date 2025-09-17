#include "battle_field.h"

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
    AddPlayer(player_names[i], 1.0f, 0.0f);
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

void BattleField::AddPlayer(std::string name, float health, float energy) {
  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].GetName() == name) {
      std::cout << "Error: player " << name << " is already in the game"
                << std::endl;
      return;
    }
  }
  players_.push_back(Player(health, energy, name,
                            static_cast<Game::PlayerPosition>(Game::CENTER)));
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
  std::cout << "Error: player " << name
            << " is not found. Return the first player." << std::endl;
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

void BattleField::PrintBattleField(uint32_t mode) {
  switch (mode) {
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

std::string BattleField::GetBattleFieldMessage(uint32_t mode) {
  std::string output;
  switch (mode) {
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

void BattleField::BattleFieldUpdate(uint32_t mode) {
  switch (mode) {
    case 1: {
      TurnUpdate();
      PrintBattleField(1);
      ActionUpdate();
      RemoveDead(mode);
      PrintBattleField(1);
      PositionUpdate();
      EnergyUpdate();
      RemoveDead(mode);
      HealthUpdate();
      RemoveDead(mode);
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
      RemoveDead(mode);
      PrintBattleField(3);
      PositionUpdate();
      EnergyUpdate();
      RemoveDead(mode);
      HealthUpdate();
      RemoveDead(mode);
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
      RemoveDead(mode);
      PrintBattleField(mode);
      PositionUpdate();
      EnergyUpdate();
      RemoveDead(mode);
      HealthUpdate();
      RemoveDead(mode);
      MemberNumUpdate();
  }
}

void BattleField::BattleFieldUpdate(std::vector<std::string> player_actions,
                                    uint32_t mode) {
  TurnUpdate();
  ActionUpdate(player_actions);
  RemoveDead(mode);
  if (mode == 0) {
    PrintBattleField(0);
  }
  PositionUpdate();
  EnergyUpdate();
  RemoveDead(mode);
  HealthUpdate();
  RemoveDead(mode);
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
      std::string target_name;
      uint32_t repeated_times;
      DecodeInputString(player_action_name, current_action_name, target_name,
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
        players_[i].AddTarget(target_name, repeated_times);
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
}

void BattleField::ActionUpdate(
    std::vector<uint32_t>
        player_actions) {  // this function is designed for HDP-pro specifically
  if (player_actions.size() > players_.size()) {
    std::cout << "Error: too many actions. Only the first " << players_.size()
              << " actions are used." << std::endl;
  } else if (player_actions.size() < players_.size()) {
    std::cout << "Error: too few actions. The last "
              << players_.size() - player_actions.size()
              << " players' actions are set as NONE." << std::endl;
    for (uint32_t i = player_actions.size(); i < players_.size(); i++) {
      player_actions.push_back(NONE);
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    players_[i].ClearTargets();
    ActionName action = static_cast<ActionName>(player_actions[i]);
    players_[i].SetActionName(action);
    players_[i].AddTarget("#ALL", 1);
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
    for (uint32_t j = 0; j < players_[i].GetTargetNum(); j++) {
      if (players_[i].GetTarget(j).first == "#ALL") {
        switch (players_[i].GetAction()->GetTargetType()) {
          case SINGLE:
            players_[i].SetEnergy(players_[i].GetEnergy() -
                                  players_[i].GetAction()->GetEnergy() *
                                      (GetMemberNum() - 1) *
                                      players_[i].GetTarget(j).second);
            break;
          case ALL:
          case SELF:
            players_[i].SetEnergy(players_[i].GetEnergy() -
                                  players_[i].GetAction()->GetEnergy() *
                                      players_[i].GetTarget(j).second);
            break;
        }
      } else {
        players_[i].SetEnergy(players_[i].GetEnergy() -
                              players_[i].GetAction()->GetEnergy() *
                                  players_[i].GetTarget(j).second);
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
}

void BattleField::HealthUpdate() {
  /**************** special cases: single-person actions: DUPLICATOR,
   * ARTIFACT_SOUL ********************/
  for (uint32_t i = 0; i < players_.size(); i++) {
    switch (players_[i].GetAction()->GetId()) {
      case DUPLICATOR:
        for (uint32_t j = 0; j < players_[i].GetTargetNum(); j++) {
          players_[i].SetEnergy(players_[i].GetEnergy() +
                                5 * players_[i].GetTarget(j).second);
        }
        break;
      case ARTIFACT_SOUL:
        for (uint32_t j = 0; j < players_[i].GetTargetNum(); j++) {
          players_[i].SetHealth(players_[i].GetHealth() +
                                1 * players_[i].GetTargets()[j].second);
        }
        break;
    }
  }
  /******************************************************************************************************/

  for (uint32_t i = 0; i < players_.size(); i++) {
    for (uint32_t j = 0; j < players_[i].GetTargetNum(); j++) {
      // if (players_[i].GetTarget(j).first == "#NONE") {
      //   continue;
      // } else
      if (players_[i].GetTarget(j).first == "#ALL") {
        for (uint32_t k = 0; k < players_[i].GetTarget(j).second; k++) {
          for (Player &it : players_) {
            if (it.GetName() != players_[i].GetName()) {
              referee_.ActionLogAdd(&players_[i], players_[i].GetAction(),
                                    it.GetName());
            }
          }
        }
      } else {
        for (uint32_t k = 0; k < players_[i].GetTarget(j).second; k++) {
          referee_.ActionLogAdd(&players_[i], players_[i].GetAction(),
                                players_[i].GetTarget(j).first);
        }
      }
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    referee_.JudgeBattle(&players_[i]);
  }

  referee_.ActionLogClear();

  referee_.DamageCommit();
  referee_.DamageLogClear();
}

void BattleField::RemoveDead(uint32_t mode) {
  if (mode == 0) {
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
            std::cout << "been backfired." << std::endl;
            break;
        }
      }
    }
  }

  for (uint32_t i = 0; i < players_.size(); i++) {
    if (players_[i].IsDead()) {
      std::string name = RemovePlayer(i--);
      if (mode == 0) {
        std::cout << name << " is removed from the battle field." << std::endl;
      }
    }
  }
}

}  // namespace Game