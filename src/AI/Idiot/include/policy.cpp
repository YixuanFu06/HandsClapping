#include "policy.h"

#include <fstream>

namespace AI {

namespace Idiot {

std::filesystem::path FindRootPath() {
  std::filesystem::path current_path = std::filesystem::current_path();
  std::filesystem::path root_path;

  std::string current_path_str = current_path.string();
  std::size_t found = current_path_str.find("HandsClapping");

  if (found != std::string::npos) {
    root_path = current_path_str.substr(
        0, found + std::string("HandsClapping").length());
  } else {
    std::cerr << "Error: HandsClapping directory not found in " << current_path
              << ". Please run the program under the project directory."
              << std::endl;
    exit(1);
  }

  return root_path;
}

std::filesystem::path GetPolicyPath(const std::string &name) {
  std::filesystem::path root_path = FindRootPath();
  std::filesystem::path policy_path =
      root_path / "data/AI/Idiot" / ("policy_" + name + ".txt");
  return policy_path;
}

Reward::Reward()
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      id_(0),
      declining_coefficient_(default_declining_coefficient),
      conservative_coefficient_(default_conservative_coefficient),
      greedy_coefficient_(default_greedy_coefficient),
      aggressive_coefficient_(default_aggressive_coefficient) {
}

Reward::Reward(const Reward &r)
    : Tensor<float, STATE_DIM * 2 + 1>(r),
      id_(r.id_),
      declining_coefficient_(r.declining_coefficient_),
      conservative_coefficient_(r.conservative_coefficient_),
      greedy_coefficient_(r.greedy_coefficient_),
      aggressive_coefficient_(r.aggressive_coefficient_) {
}

Reward::Reward(const Policy &p)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}) {
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            (*this)[i][j][k][l][n] = 0.0;
          }
        }
      }
    }
  }
  id_ = p.id_;
  declining_coefficient_ = p.declining_coefficient_;
  conservative_coefficient_ = p.conservative_coefficient_;
  greedy_coefficient_ = p.greedy_coefficient_;
  aggressive_coefficient_ = p.aggressive_coefficient_;
}

Reward &Reward::operator=(const Reward &r) {
  if (this != &r) {
    id_ = r.id_;
    declining_coefficient_ = r.declining_coefficient_;
    conservative_coefficient_ = r.conservative_coefficient_;
    greedy_coefficient_ = r.greedy_coefficient_;
    aggressive_coefficient_ = r.aggressive_coefficient_;
    for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
      (*this)[i] = r[i];
    }
  }
  return *this;
}

Reward Reward::operator+(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
  Reward result;
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    result[i] = (*this)[i] + r[i];
  }
  result.id_ = this->id_;
  result.declining_coefficient_ = this->declining_coefficient_;
  result.conservative_coefficient_ = this->conservative_coefficient_;
  result.greedy_coefficient_ = this->greedy_coefficient_;
  result.aggressive_coefficient_ = this->aggressive_coefficient_;
  return result;
}

Reward Reward::operator-(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
  Reward result;
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    result[i] = (*this)[i] - r[i];
  }
  result.id_ = this->id_;
  result.declining_coefficient_ = this->declining_coefficient_;
  result.conservative_coefficient_ = this->conservative_coefficient_;
  result.greedy_coefficient_ = this->greedy_coefficient_;
  result.aggressive_coefficient_ = this->aggressive_coefficient_;
  return result;
}

Reward Reward::operator*(const float f) {
  Reward result;
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    result[i] = (*this)[i] * f;
  }
  result.id_ = this->id_;
  result.declining_coefficient_ = this->declining_coefficient_;
  result.conservative_coefficient_ = this->conservative_coefficient_;
  result.greedy_coefficient_ = this->greedy_coefficient_;
  result.aggressive_coefficient_ = this->aggressive_coefficient_;
  return result;
}

Reward &Reward::operator+=(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    (*this)[i] += r[i];
  }
  return *this;
}

Reward &Reward::operator-=(const Reward &r) {
  this->id_ = std::max(this->id_, r.id_);
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    (*this)[i] -= r[i];
  }
  return *this;
}

Reward &Reward::operator*=(const float f) {
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    (*this)[i] *= f;
  }
  return *this;
}

void Reward::StrategyUpdate(float enemy_health,
                            float health,
                            float enemy_energy,
                            float energy,
                            Game::Action *enemy_action,
                            Game::Action *action) {
  *this *= declining_coefficient_;
  (*this)[static_cast<uint32_t>(enemy_health)][static_cast<uint32_t>(health)]
         [static_cast<uint32_t>(enemy_energy)][static_cast<uint32_t>(energy)]
         [action->GetId()] += conservative_coefficient_;
  (*this)[static_cast<uint32_t>(enemy_health)][static_cast<uint32_t>(health)]
         [static_cast<uint32_t>(enemy_energy)][static_cast<uint32_t>(energy)]
         [enemy_action->GetId()] -= conservative_coefficient_;
}

void Reward::ActionUpdate_Health(float enemy_health,
                                 float health,
                                 float enemy_energy,
                                 float energy,
                                 Game::Action *action,
                                 float change_of_health) {
  if (action->GetType() != Game::ATTACK) {
    std::cout
        << "Error: the argument of ActionUpdate_Health is not an ATTACK type."
        << std::endl;
    exit(1);
  }
  (*this)[static_cast<uint32_t>(enemy_health)][static_cast<uint32_t>(health)]
         [static_cast<uint32_t>(enemy_energy)][static_cast<uint32_t>(energy)]
         [action->GetId()] += change_of_health + aggressive_coefficient_;
}

void Reward::ActionUpdate_Energy(float enemy_health,
                                 float health,
                                 float enemy_energy,
                                 float energy,
                                 Game::Action *action,
                                 float change_of_energy) {
  (*this)[static_cast<uint32_t>(enemy_health)][static_cast<uint32_t>(health)]
         [static_cast<uint32_t>(enemy_energy)][static_cast<uint32_t>(energy)]
         [action->GetId()] +=
      (change_of_energy - 0.5 - action->GetEnergy()) * greedy_coefficient_;
}

void Reward::Store(const std::string &path) {
  std::filesystem::path fs_path(path);
  std::filesystem::create_directory(fs_path.parent_path());
  std::ofstream fout(fs_path);
  if (!fout.is_open()) {
    std::cerr << "Error: Failed to open file " << path << std::endl;
    exit(1);
  }
  fout << id_ << std::endl;
  fout << declining_coefficient_ << " " << conservative_coefficient_ << " "
       << greedy_coefficient_ << " " << aggressive_coefficient_ << std::endl;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            fout << (*this)[i][j][k][l][n] << " ";
          }
          fout << std::endl;
        }
        fout << std::endl;
      }
      fout << std::endl;
    }
    fout << std::endl;
  }
  fout.close();
}

Policy::Policy()
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}) {
}

Policy::Policy(const Policy &p)
    : Tensor<float, STATE_DIM * 2 + 1>(p),
      name_(p.name_),
      id_(p.id_),
      update_precision_(p.update_precision_),
      declining_coefficient_(p.declining_coefficient_),
      conservative_coefficient_(p.conservative_coefficient_),
      greedy_coefficient_(p.greedy_coefficient_),
      aggressive_coefficient_(p.aggressive_coefficient_) {
}

Policy::Policy(const std::string &name, uint32_t id)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      id_(id),
      name_(name),
      update_precision_(default_update_precision),
      declining_coefficient_(default_declining_coefficient),
      conservative_coefficient_(default_conservative_coefficient),
      greedy_coefficient_(default_greedy_coefficient),
      aggressive_coefficient_(default_aggressive_coefficient) {
  for (uint32_t i = 0; i <= MAX_ENERGY; i++) {
    uint32_t possible_action_num = 0;
    for (Game::Action &action : Game::actions) {
      if (action.GetEnergy() <= i && action.GetId() != Game::NONE &&
          action.GetId() != Game::TIMEOUT) {
        possible_action_num++;
      }
    }
    for (uint32_t j = 0; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_HEALTH; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if (Game::actions[n].GetEnergy() <= i &&
                Game::actions[n].GetId() != Game::NONE &&
                Game::actions[n].GetId() != Game::TIMEOUT) {
              (*this)[j][k][l][i][n] = 1.0 / possible_action_num;
            } else {
              (*this)[j][k][l][i][n] = 0.0;
            }
          }
        }
      }
    }
  }
}

Policy::Policy(const std::string &path)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}) {
  std::ifstream fin(path);
  if (!fin.is_open()) {
    std::cerr << "Error: Failed to open file " << path << std::endl;
    exit(1);
  }
  fin >> name_ >> id_;
  fin >> update_precision_ >> declining_coefficient_ >>
      conservative_coefficient_ >> greedy_coefficient_ >>
      aggressive_coefficient_;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            fin >> (*this)[i][j][k][l][n];
          }
        }
      }
    }
  }
  fin.close();
}

Policy::Policy(const std::filesystem::path &path) : Policy(path.string()) {
}

Policy &Policy::operator=(const Policy &p) {
  if (this != &p) {
    name_ = p.name_;
    id_ = p.id_;
    update_precision_ = p.update_precision_;
    declining_coefficient_ = p.declining_coefficient_;
    conservative_coefficient_ = p.conservative_coefficient_;
    greedy_coefficient_ = p.greedy_coefficient_;
    aggressive_coefficient_ = p.aggressive_coefficient_;
    for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
      (*this)[i] = p[i];
    }
  }
  return *this;
}

Policy Policy::operator+(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  Policy result;
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    result[i] = (*this)[i] + r[i];
  }
  result.id_ = this->id_;
  result.name_ = this->name_;
  result.update_precision_ = this->update_precision_;
  result.declining_coefficient_ = this->declining_coefficient_;
  result.conservative_coefficient_ = this->conservative_coefficient_;
  result.greedy_coefficient_ = this->greedy_coefficient_;
  result.aggressive_coefficient_ = this->aggressive_coefficient_;
  return result;
}

Policy Policy::operator*(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  Policy result;
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    result[i] = (*this)[i] * r[i];
  }
  result.id_ = this->id_;
  result.name_ = this->name_;
  result.update_precision_ = this->update_precision_;
  result.declining_coefficient_ = this->declining_coefficient_;
  result.conservative_coefficient_ = this->conservative_coefficient_;
  result.greedy_coefficient_ = this->greedy_coefficient_;
  result.aggressive_coefficient_ = this->aggressive_coefficient_;
  return result;
}

Policy &Policy::operator+=(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    (*this)[i] += r[i];
  }
  return *this;
}

Policy &Policy::operator*=(const Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    (*this)[i] *= r[i];
  }
  return *this;
}

void Policy::Store() {
  std::filesystem::path path = GetPolicyPath(name_);
  std::filesystem::create_directories(path.parent_path());
  std::ofstream fout(path);
  if (!fout.is_open()) {
    std::cerr << "Error: Failed to open file " << path << std::endl;
    exit(1);
  }
  fout << name_ << " " << id_ << std::endl;
  fout << update_precision_ << " " << declining_coefficient_ << " "
       << conservative_coefficient_ << " " << greedy_coefficient_ << " "
       << aggressive_coefficient_ << std::endl;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            fout << (*this)[i][j][k][l][n] << " ";
          }
          fout << std::endl;
        }
        fout << std::endl;
      }
      fout << std::endl;
    }
    fout << std::endl;
  }
  fout.close();
}

void Policy::Store(const std::string &name) {
  name_ = name;
  Store();
}

void Policy::Normalize() {
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          float sum = 0.0;
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            sum += (*this)[i][j][k][l][n];
          }
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if (sum == 0.0) {
              (*this)[i][j][k][l][n] = 0.0;
            } else {
              (*this)[i][j][k][l][n] = (*this)[i][j][k][l][n] / sum;
            }
          }
        }
      }
    }
  }
}

void Policy::RewardFeedback(Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  bool IsInvalid = true;
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if (r[i][j][k][l][n] != 0) {
              (*this)[i][j][k][l][n] =
                  (*this)[i][j][k][l][n] *
                  (1 + r[i][j][k][l][n] / update_precision_);
              IsInvalid = false;
            }
          }
        }
      }
    }
  }
  if (IsInvalid) {
    std::cerr << "Error: invalid reward tensor." << std::endl;
    exit(1);
  }
}

float Policy::Similarity(const Policy &p) {
  float tvd = 0.0;
  float total_entry = 0.0;
  float declining_coefficient =
      std::min(declining_coefficient_, p.declining_coefficient_);
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          float ratio = pow(declining_coefficient, i + j + k + l - 2);
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if (Game::actions[n].GetEnergy() <= l) {
              tvd += std::abs((*this)[i][j][k][l][n] - p[i][j][k][l][n]) / 2 *
                     ratio;
            }
          }
          total_entry += ratio;
        }
      }
    }
  }
  return 1 - tvd / total_entry;
}

void Policy::PrintDistribution(float enemy_health,
                               float health,
                               float enemy_energy,
                               float energy) {
  bool IsPrinted[ACTION_NUM];
  for (uint32_t i = 0; i < ACTION_NUM; i++) {
    IsPrinted[i] = false;
  }

  bool PrintIsOver;
  do {
    PrintIsOver = true;
    uint32_t current_max_action;
    for (uint32_t i = 0; i < ACTION_NUM; i++) {
      if ((*this)[enemy_health][health][enemy_energy][energy][i] != 0 &&
          !IsPrinted[i]) {
        current_max_action = i;
        PrintIsOver = false;
        break;
      }
    }
    if (PrintIsOver) {
      break;
    }

    for (uint32_t i = current_max_action + 1; i < ACTION_NUM; i++) {
      if (!IsPrinted[i] &&
          (*this)[enemy_health][health][enemy_energy][energy][i] >
              (*this)[enemy_health][health][enemy_energy][energy]
                     [current_max_action]) {
        current_max_action = i;
      }
    }

    IsPrinted[current_max_action] = true;
    std::cout << Game::actions[current_max_action].GetFormalName() << ": "
              << (*this)[enemy_health][health][enemy_energy][energy]
                        [current_max_action] *
                     100
              << "%\n";
  } while (!PrintIsOver);
}

void Policy::Update(Reward &r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  RewardFeedback(r);
  this->Normalize();
  id_++;
}

void Policy::Update(Reward &&r) {
  if (this->id_ != r.id_) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  RewardFeedback(r);
  this->Normalize();
  id_++;
}

Game::Action *Policy::GetAction(float enemy_health,
                                float health,
                                float enemy_energy,
                                float energy) {
  std::vector<float> probabilities;
  for (Game::Action &action : Game::actions) {
    if (action.GetId() != Game::NONE && action.GetId() != Game::TIMEOUT) {
      probabilities.push_back(
          (*this)[enemy_health][health][enemy_energy][energy][action.GetId()]);
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());

  int index = dist(gen);
  return &Game::actions[index];
}

void Policy::SetUpdatePrecision(float update_precision) {
  if (update_precision > 0) {
    update_precision_ = update_precision;
    std::cout << "Reset update precision for " << name_ << " to "
              << update_precision << std::endl;
  } else {
    std::cout << "Invalid update precision for " << name_
              << " , (0, +inf), set as default value "
              << default_update_precision << std::endl;
  }
}

void Policy::SetDecliningCoefficient(float declining_coefficient) {
  if (declining_coefficient >= 0 && declining_coefficient <= 1) {
    declining_coefficient_ = declining_coefficient;
    std::cout << "Reset declining coefficient for " << name_ << " to "
              << declining_coefficient << std::endl;
  } else {
    std::cout << "Invalid declining coefficient for " << name_
              << " , [0, 1], set as default value "
              << default_declining_coefficient << std::endl;
  }
}

void Policy::SetConservativeCoefficient(float conservative_coefficient) {
  if (conservative_coefficient >= 0) {
    conservative_coefficient_ = conservative_coefficient;
    std::cout << "Reset conservative coefficient for " << name_ << " to "
              << conservative_coefficient << std::endl;
  } else {
    std::cout << "Invalid conservative coefficient for " << name_
              << " , [0, +inf), set as default value "
              << default_conservative_coefficient << std::endl;
  }
}

void Policy::SetGreedyCoefficient(float greedy_coefficient) {
  if (greedy_coefficient >= 0) {
    greedy_coefficient_ = greedy_coefficient;
    std::cout << "Reset greedy coefficient for " << name_ << " to "
              << greedy_coefficient << std::endl;
  } else {
    std::cout << "Invalid greedy coefficient for " << name_
              << " , [0, +inf), set as default value "
              << default_greedy_coefficient << std::endl;
  }
}

void Policy::SetAggressiveCoefficient(float aggressive_coefficient) {
  if (aggressive_coefficient >= 0) {
    aggressive_coefficient_ = aggressive_coefficient;
    std::cout << "Reset aggressive coefficient for " << name_ << " to "
              << aggressive_coefficient << std::endl;
  } else {
    std::cout << "Invalid aggressive coefficient for " << name_
              << " , [0, +inf), set as default value "
              << default_aggressive_coefficient << std::endl;
  }
}

}  // namespace Idiot

}  // namespace AI