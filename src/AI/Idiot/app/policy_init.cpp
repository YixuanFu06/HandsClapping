#include <string>

#include "../include/policy.h"

int main() {
  Game::InitActions();
  std::cout << "Enter the name of the policy: (e.g. idiot-alpha)" << std::endl;
  std::string name;
  std::cin >> name;
  AI::Idiot::Policy new_policy = AI::Idiot::Policy(name, 0);
  new_policy.Store();
  std::cout << "Policy " << name << " is stored to " << AI::Idiot::GetPolicyPath(name).string() << std::endl;
}