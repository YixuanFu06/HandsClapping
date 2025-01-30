#include "../include/policy.h"

const std::string POLICY1 = "Idiot-alpha";
const std::string POLICY2 = "init";

int main() {
  Game::InitActions();
  AI::Idiot::Policy policy1 = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY1).string());
  AI::Idiot::Policy policy2 = AI::Idiot::Policy(AI::Idiot::GetPolicyPath(POLICY2).string());

  float similarity = policy1.Similarity(policy2);
  std::cout << "Similarity between " << policy1.GetName() << " and "
            << policy2.GetName() << ": " << similarity << std::endl;

  // TODO: query for the action probability distribution in a specific state
}