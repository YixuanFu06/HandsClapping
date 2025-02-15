#include "../include/policy.h"

void Initialization();
void SimilarityCheck();
void ProbabilityDistribution();
void ArgumentsEdit();
void Copy();
void Delete();

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::Idiot::FindRootPath()
            << std::endl
            << std::endl;

  std::cout << "Select your tool: init (initialization), sim (similarity "
               "check), p (probability distribution), arg (arguments edit), c "
               "(copy), del (delete): ";
  std::string command;
  std::cin >> command;

  if (command == "init") {
    Initialization();
  } else if (command == "sim") {
    SimilarityCheck();
  } else if (command == "p") {
    ProbabilityDistribution();
  } else if (command == "arg") {
    ArgumentsEdit();
  } else if (command == "c") {
    Copy();
  } else if (command == "del") {
    Delete();
  } else {
    std::cout << "Invalid command. Try again." << std::endl;
  }

  std::cout << "Press any key to continue..." << std::endl;
  std::cin.get();
}

void Initialization() {
  std::cout << "Enter the name of the policy: (e.g. idiot-alpha)" << std::endl;
  std::string name;
  std::cin >> name;
  if (std::filesystem::exists(AI::Idiot::GetPolicyPath(name))) {
    std::cout << "Policy " << name << " at " << AI::Idiot::GetPolicyPath(name)
              << " already exists, do you want to overwrite it? (y/n) ";
    std::string YesOrNo;
    std::cin >> YesOrNo;
    if (YesOrNo != "y") {
      std::cout << "Initialization fails. Try again with another name."
                << std::endl;
      return;
    } else {
      std::cout << "Policy " << name << " will be overwritten." << std::endl;
    }
  }

  AI::Idiot::Policy new_policy(name, 0);
  new_policy.Store();
  std::cout << "Policy " << name << " is stored to "
            << AI::Idiot::GetPolicyPath(name).string() << std::endl;
}

void SimilarityCheck() {
  std::cout << "Enter the names of the policies you want to compare (separated "
               "by space or backspace): ";
  std::string name1;
  std::string name2;
  std::cin >> name1 >> name2;

  AI::Idiot::Policy policy1(AI::Idiot::GetPolicyPath(name1));
  AI::Idiot::Policy policy2(AI::Idiot::GetPolicyPath(name2));

  std::cout << "Similarity between " << policy1.GetName() << " and "
            << policy2.GetName() << ": " << policy1.Similarity(policy2)
            << std::endl;
}

void ProbabilityDistribution() {
  std::cout << "Enter the name of the policy you want to query: ";
  std::string name;
  std::cin >> name;

  AI::Idiot::Policy policy(AI::Idiot::GetPolicyPath(name));

  float enemy_health;
  float enemy_energy;
  float health;
  float energy;
  std::cout
      << "Enter the arguments of the state you want to query in the order of "
         "<enemy_health> <self_health> <enemy_energy> <self_energy>:\n";
  std::cin >> enemy_health >> health >> enemy_energy >> energy;
  if (enemy_health <= 0 || enemy_health > AI::Idiot::MAX_HEALTH ||
      health <= 0 || health > AI::Idiot::MAX_HEALTH || enemy_energy < 0 ||
      enemy_energy > AI::Idiot::MAX_ENERGY || energy < 0 ||
      energy > AI::Idiot::MAX_ENERGY) {
    std::cout << "Invalid argument <enemy_health>. Expected in (0,"
              << AI::Idiot::MAX_HEALTH << ")." << std::endl;
    return;
  }
  if (health <= 0 || health > AI::Idiot::MAX_HEALTH) {
    std::cout << "Invalid argument <self_health>. Expected in (0,"
              << AI::Idiot::MAX_HEALTH << ")." << std::endl;
    return;
  }
  if (enemy_energy < 0 || enemy_energy > AI::Idiot::MAX_ENERGY) {
    std::cout << "Invalid argument <enemy_energy>. Expected in (0,"
              << AI::Idiot::MAX_ENERGY << ")." << std::endl;
    return;
  }
  if (energy < 0 || energy > AI::Idiot::MAX_ENERGY) {
    std::cout << "Invalid argument <self_energy>. Expected in (0,"
              << AI::Idiot::MAX_ENERGY << ")." << std::endl;
    return;
  }
  policy.PrintDistribution(enemy_health, health, enemy_energy, energy);
}

void ArgumentsEdit() {
  std::cout << "Enter the name of the policy you want to edit: ";
  std::string name;
  std::cin >> name;
  AI::Idiot::Policy policy(AI::Idiot::GetPolicyPath(name));

  std::cout << "Policy " << policy.GetName() << "[" << policy.GetId() << "]:\n";
  std::cout << "Update precision: " << policy.GetUpdatePrecision() << std::endl;
  std::cout << "Declining coefficient: " << policy.GetDecliningCoefficient()
            << std::endl;
  std::cout << "Conservative coefficient: "
            << policy.GetConservativeCoefficient() << std::endl;
  std::cout << "Greedy coefficient: " << policy.GetGreedyCoefficient()
            << std::endl;
  std::cout << "Aggressive coefficient: " << policy.GetAggressiveCoefficient()
            << std::endl;
  std::cout << "Which argument do you want to edit (name, update, decline, "
               "conservative, greedy, aggressive): ";
  std::string argument;
  std::cin >> argument;

  if (argument == "name") {
    std::cout << "Enter the new name: ";
    std::string new_name;
    std::cin >> new_name;

    if (std::filesystem::exists(AI::Idiot::GetPolicyPath(new_name))) {
      std::cout << "Policy " << new_name << " at "
                << AI::Idiot::GetPolicyPath(new_name)
                << " already exists, do you want to overwrite it? (y/n) ";
      std::string YesOrNo;
      std::cin >> YesOrNo;
      if (YesOrNo != "y") {
        std::cout << "Editing fails. Try again with another name." << std::endl;
        return;
      } else {
        std::cout << "Policy " << new_name << " will be overwritten."
                  << std::endl;
      }
    }

    policy.SetName(new_name);
    std::filesystem::remove(AI::Idiot::GetPolicyPath(name));
  } else if (argument == "update") {
    std::cout << "Enter the new update precision: ";
    float new_update_precision;
    std::cin >> new_update_precision;
    policy.SetUpdatePrecision(new_update_precision);
  } else if (argument == "decline") {
    std::cout << "Enter the new declining coefficient: ";
    float new_declining_coefficient;
    std::cin >> new_declining_coefficient;
    policy.SetDecliningCoefficient(new_declining_coefficient);
  } else if (argument == "conservative") {
    std::cout << "Enter thr new conservative coefficient: ";
    float new_conservative_coefficient;
    std::cin >> new_conservative_coefficient;
    policy.SetConservativeCoefficient(new_conservative_coefficient);
  } else if (argument == "greedy") {
    std::cout << "Enter the new greedy coefficient: ";
    float new_greedy_coefficient;
    std::cin >> new_greedy_coefficient;
    policy.SetGreedyCoefficient(new_greedy_coefficient);
  } else if (argument == "aggressive") {
    std::cout << "Enter the new aggressive coefficient: ";
    float new_aggressive_coefficient;
    std::cin >> new_aggressive_coefficient;
    policy.SetAggressiveCoefficient(new_aggressive_coefficient);
  } else {
    std::cout << "Invalid argument. Try again." << std::endl;
    return;
  }

  policy.Store();
  std::cout << "Policy " << name << "'s edit is stored to "
            << AI::Idiot::GetPolicyPath(name).string() << std::endl;
}

void Copy() {
  std::cout << "Enter the name of the policy you want to copy and the name of "
               "new policy (separated by space or backspace): ";
  std::string copied_name;
  std::string new_name;
  std::cin >> copied_name >> new_name;

  if (std::filesystem::exists(AI::Idiot::GetPolicyPath(new_name))) {
    std::cout << "Policy " << new_name << " at "
              << AI::Idiot::GetPolicyPath(new_name)
              << " already exists, do you want to overwrite it? (y/n) ";
    std::string YesOrNo;
    std::cin >> YesOrNo;
    if (YesOrNo != "y") {
      std::cout << "Editing fails. Try again with another name." << std::endl;
      return;
    } else {
      std::cout << "Policy " << new_name << " will be overwritten."
                << std::endl;
    }
  }

  AI::Idiot::Policy policy(AI::Idiot::GetPolicyPath(copied_name));

  policy.SetName(new_name);
  policy.Store();

  std::cout << "New policy " << new_name << " is stored to "
            << AI::Idiot::GetPolicyPath(new_name).string() << std::endl;
}

void Delete() {
  std::cout << "Enter the name of the policy you want to delete: ";
  std::string name;
  std::cin >> name;
  std::filesystem::remove(AI::Idiot::GetPolicyPath(name));
  std::cout << "File " << AI::Idiot::GetPolicyPath(name).string()
            << " is deleted." << std::endl;
}