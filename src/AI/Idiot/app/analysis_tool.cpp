#include "../include/policy.h"

const std::string DATA_PATH1 = "./data/AI/Idiot/policy_Idiot-alpha.txt";
const std::string DATA_PATH2 = "./data/AI/Idiot/init.txt";

int main() {
  std::filesystem::path root_path = AI::Idiot::FindRootPath();
  std::filesystem::path data_path1 = root_path / DATA_PATH1;
  std::filesystem::path data_path2 = root_path / DATA_PATH2;
  std::filesystem::create_directories(data_path1.parent_path());
  std::filesystem::create_directories(data_path2.parent_path());

  Game::InitActions();
  AI::Idiot::Policy policy1 = AI::Idiot::Policy(data_path1.string());
  AI::Idiot::Policy policy2 = AI::Idiot::Policy(data_path2.string());

  float similarity = policy1.Similarity(policy2);
  std::cout << "Similarity between " << policy1.GetName() << " and "
            << policy2.GetName() << ": " << similarity << std::endl;

  // TODO: query for the action probability distriibution in a specific state
}