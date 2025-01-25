#include "policy.h"

#include <fstream>
#include <random>

#include "define_actions.h"

namespace AI {

namespace Idiot {

Reward::Reward()
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      id_(0) {
}

Reward::Reward(uint32_t id)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}) {
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_ENERGY; j++) {
      for (uint32_t k = 0; k <= MAX_HEALTH; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            (*this)[i][j][k][l][n] = 0.0;
          }
        }
      }
    }
  }
  id_ = id;
}

Reward &Reward::operator=(const Reward &r) {
  if (this != &r) {
    id_ = r.id_;
    for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
      (*this)[i] = r[i];
    }
  }
  return *this;
}

Reward Reward::operator+(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
  Reward result;
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    result[i] = (*this)[i] + r[i];
  }
  result.id_ = this->id_;
  return result;
}

Reward Reward::operator-(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
  Reward result;
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    result[i] = (*this)[i] - r[i];
  }
  result.id_ = this->id_;
  return result;
}

Reward Reward::operator*(const float f) {
  Reward result;
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    result[i] = (*this)[i] * f;
  }
  result.id_ = this->id_;
  return result;
}

Reward &Reward::operator+=(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
  for (uint32_t i = 0; i < MAX_HEALTH + 1; i++) {
    (*this)[i] += r[i];
  }
  return *this;
}

Reward &Reward::operator-=(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Reward and Reward do not match" << std::endl;
    exit(1);
  }
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

void Reward::Update(float enemy_health,
                    float enemy_energy,
                    float health,
                    float energy,
                    Game::Action *action) {
  *this *= GAMMA;
  (*this)[static_cast<uint32_t>(enemy_health)]
         [static_cast<uint32_t>(enemy_energy)][static_cast<uint32_t>(health)]
         [static_cast<uint32_t>(energy)][action->GetId()] += 1.0;
}

void Reward::Store(const std::string &path) {
  std::ofstream fout(path);
  if (!fout.is_open()) {
    std::cerr << "Error: Failed to open file " << path << std::endl;
    exit(1);
  }
  fout << id_ << std::endl;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_ENERGY; j++) {
      for (uint32_t k = 1; k <= MAX_HEALTH; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            fout << (*this)[i][j][k][l][n] << " ";
          }
          fout << std::endl;
        }
      }
    }
  }
  fout.close();
}

Policy::Policy()
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}) {
}

Policy::Policy(uint32_t id)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      id_(id),
      name_("init") {
  for (uint32_t i = 0; i <= MAX_ENERGY; i++) {
    uint32_t possible_action_num = 0;
    for (Game::Action &action : Game::actions) {
      if (action.GetEnergy() <= i && action.GetId() != Game::NONE &&
          action.GetId() != Game::TIMEOUT) {
        possible_action_num++;
      }
    }
    for (uint32_t j = 0; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_HEALTH; l++) {
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
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        MAX_HEALTH + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}) {
  std::ifstream fin(path);
  if (!fin.is_open()) {
    std::cerr << "Error: Failed to open file " << path << std::endl;
    exit(1);
  }
  fin >> name_ >> id_;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_ENERGY; j++) {
      for (uint32_t k = 1; k <= MAX_HEALTH; k++) {
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

Policy &Policy::operator=(const Policy &p) {
  if (this != &p) {
    name_ = p.name_;
    id_ = p.id_;
    for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
      (*this)[i] = p[i];
    }
  }
  return *this;
}

Policy Policy::operator+(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  Policy result;
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    result[i] = (*this)[i] + r[i];
  }
  result.id_ = this->id_;
  result.name_ = this->name_;
  return result;
}

Policy Policy::operator*(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  Policy result;
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    result[i] = (*this)[i] * r[i];
  }
  result.id_ = this->id_;
  result.name_ = this->name_;
  return result;
}

Policy &Policy::operator+=(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    (*this)[i] += r[i];
  }
  return *this;
}

Policy &Policy::operator*=(const Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    (*this)[i] *= r[i];
  }
  return *this;
}

void Policy::Store(const std::string &path) {
  std::ofstream fout(path);
  if (!fout.is_open()) {
    std::cerr << "Error: Failed to open file " << path << std::endl;
    exit(1);
  }
  fout << name_ << " " << id_ << std::endl;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_ENERGY; j++) {
      for (uint32_t k = 1; k <= MAX_HEALTH; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            fout << (*this)[i][j][k][l][n] << " ";
          }
          fout << std::endl;
        }
      }
    }
  }
  fout.close();
}

void Policy::Store(const std::string &path, const std::string &name) {
  name_ = name;
  Store(path);
}

void Policy::Normalize() {
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_ENERGY; j++) {
      for (uint32_t k = 0; k <= MAX_HEALTH; k++) {
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
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  bool IsInvalid = true;
  for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 0; j <= MAX_ENERGY; j++) {
      for (uint32_t k = 0; k <= MAX_HEALTH; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if (r[i][j][k][l][n] != 0) {
              (*this)[i][j][k][l][n] =
                  (*this)[i][j][k][l][n] *
                  (1 + r[i][j][k][l][n] / UPDATE_PRECISION);
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

void Policy::Update(Reward &r) {
  if (this->id_ != r.GetId()) {
    std::cerr << "Error: Policy and Reward do not match" << std::endl;
    exit(1);
  }
  RewardFeedback(r);
  this->Normalize();
  id_++;
}

Game::Action *Policy::GetAction(uint32_t enemy_health,
                                uint32_t enemy_energy,
                                uint32_t health,
                                uint32_t energy) {
  std::vector<float> probabilities;
  for (Game::Action action : Game::actions) {
    probabilities.push_back(
        (*this)[enemy_health][enemy_energy][health][energy][action.GetId()]);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());

  int index = dist(gen);
  return &Game::actions[index];
}

}  // namespace Idiot

}  // namespace AI