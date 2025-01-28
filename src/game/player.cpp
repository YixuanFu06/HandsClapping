#include "player.h"

#include "action.h"
#include "define_actions.h"

namespace Game {

Player::Player(float health,
               float energy,
               std::string name,
               PlayerPosition position,
               ActionName action_name,
               Action *action)
    : health_(health),
      energy_(energy),
      name_(name),
      position_(position),
      action_name_(action_name),
      action_(action) {
}

Player::Player(float health,
               float energy,
               std::string name,
               PlayerPosition position)
    : health_(health),
      energy_(energy),
      name_(name),
      position_(position),
      action_name_(NONE) {
  SetAction();
}

void Player::SetAction() {
  for (uint32_t i = 0; i < actions.size(); i++) {
    if (static_cast<ActionName>(actions[i].GetId()) == action_name_) {
      action_ = &actions[i];
      return;
    }
  }
  std::cout << "Error: action " << action_name_ << " not found, set as NONE."
            << std::endl;
  action_name_ = NONE;
  action_ = nullptr;
}

void Player::PrintPlayer(uint32_t type) {
  std::cout << "Player " << name_ << ".  ";
  std::cout << "Action: " << action_->GetFormalName() << ".  ";
  for (uint32_t i = 0; i < targets_.size(); i++) {
    std::cout << "Target: " << targets_[i].first << " " << targets_[i].second
              << " times.  ";
  }
  switch (type) {
    case 1:
      std::cout << "Health: " << health_ << ".  ";
      std::cout << "Energy: " << energy_ << ".  ";
      break;
    case 3:
      std::cout << std::endl;
      action_->PrintAction();
  }
  std::cout << std::endl;
}

std::string Player::GetPlayerMessage(uint32_t type) {
  std::string message = "Player " + name_ + ".  ";
  message += "Action: " + action_->GetFormalName() + ".  ";
  for (uint32_t i = 0; i < targets_.size(); i++) {
    message += "Target: " + targets_[i].first + " " +
               std::to_string(targets_[i].second) + " times.  ";
  }
  switch (type) {
    case 1:
      message += "Health: " + std::to_string(health_) + ".  ";
      message += "Energy: " + std::to_string(energy_) + ".  ";
      break;
    case 3:
      message += "\n";
      message += action_->GetActionMessage();
  }
  message += "\n";
  return message;
}

uint32_t Player::IsAimedAt(const Player *target) {
  uint32_t times = 0;
  for (uint32_t i = 0; i < targets_.size(); i++) {
    if (targets_[i].first == target->GetName() || targets_[i].first == "#ALL") {
      times += targets_[i].second;
    }
  }
  return times;
}

}  // namespace Game