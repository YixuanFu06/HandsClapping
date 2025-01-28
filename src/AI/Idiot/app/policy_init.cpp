#include <string>

#include "../include/policy.h"

int main() {
  std::filesystem::path root_path = AI::Idiot::FindRootPath();
  std::cout << "Enter the path to the policy file: (e.g. "
               "./data/AI/Idiot/policy_name.txt)"
            << std::endl;
  std::string data_path_str;
  std::cin >> data_path_str;
  std::filesystem::path data_path = root_path / data_path_str;

  Game::InitActions();
  std::cout << "Enter the name of the policy: (e.g. idiot-alpha)" << std::endl;
  std::string name;
  std::cin >> name;
  AI::Idiot::Policy new_policy = AI::Idiot::Policy(name, 0);
  new_policy.Store(data_path.string());
}