#include "policy.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

#include "../../../game/define_actions.h"
#include "state.h"
#include "model.h"
#include "winning_rate.h"

namespace AI {

namespace HDP {

Policy::Policy()
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      original_cover_stability_({MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1,
                                 MAX_ENERGY + 1, ACTION_NUM}),
      new_cover_stability_({MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1,
                            MAX_ENERGY + 1, ACTION_NUM}) {
}

Policy::Policy(const Policy &p)
    : Tensor<float, STATE_DIM * 2 + 1>(p),
      name_(p.name_),
      id_(p.id_),
      master_model_(p.master_model_),
      original_cover_stability_(p.original_cover_stability_),
      new_cover_stability_(p.new_cover_stability_),
      declining_coefficient_(p.declining_coefficient_) {
}

Policy::Policy(Model *master_model)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      master_model_(master_model),
      name_(master_model->GetName()),
      original_cover_stability_({MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1,
                                 MAX_ENERGY + 1, ACTION_NUM}),
      new_cover_stability_({MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1,
                            MAX_ENERGY + 1, ACTION_NUM}) {
  std::filesystem::path path = GetModelPath(name_) / "policy.txt";

  if (std::filesystem::exists(path)) {
    std::ifstream fin(path);
    if (!fin.is_open()) {
      std::cerr << "Error: Cannot open file " << path << std::endl;
      exit(1);
    }
    fin >> name_ >> id_;
    fin >> declining_coefficient_;
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

    bool has_non_empty_line = false;
    std::string line;
    std::streampos last_pos;
    std::streampos current_pos = fin.tellg();
    while (std::getline(fin, line)) {
      if (!line.empty() &&
          line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
        has_non_empty_line = true;
        last_pos = current_pos;
        current_pos = fin.tellg();
        break;
      }
    }
    if (has_non_empty_line) {
      fin.seekg(last_pos);  // go back to the last non-empty line
      for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
        for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
          for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
            for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
              for (uint32_t n = 0; n < ACTION_NUM; n++) {
                fin >> original_cover_stability_[i][j][k][l][n];
              }
            }
          }
        }
      }
    } else {
      std::cout
          << "Covering stability file missing in model " << path
          << ". Are you going to generate one with default settings? (y/n) "
          << std::endl;
      std::string YesOrNo;
      std::cin >> YesOrNo;
      if (YesOrNo == "y") {
        for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
          for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
            for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
              for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
                for (uint32_t n = 0; n < ACTION_NUM; n++) {
                  original_cover_stability_[i][j][k][l][n] = 0;
                }
              }
            }
          }
        }
      } else {
        std::cout << "Loading model aborted." << std::endl;
        exit(1);
      }
    }

    fin.close();
  } else {
    std::cout << "Model " << path
              << " does not exist or policy file missing. Are you going to "
                 "generate one with default "
                 "settings? (y/n) "
              << std::endl;
    std::string YesOrNo;
    std::cin >> YesOrNo;
    if (YesOrNo == "y") {
      std::filesystem::remove(path.parent_path());
      id_ = 0;
      declining_coefficient_ = default_declining_coefficient;
      for (uint32_t i = 0; i <= MAX_ENERGY; i++) {
        uint32_t possible_action_num = 0;
        for (uint32_t n = 0; n < ACTION_NUM; n++) {
          if (Game::actions[n].GetEnergy() <= i) {
            possible_action_num++;
          }
        }
        for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
          for (uint32_t k = 1; k <= MAX_HEALTH; k++) {
            for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
              for (uint32_t n = 0; n < ACTION_NUM; n++) {
                if (Game::actions[n].GetEnergy() <= i) {
                  (*this)[j][k][l][i][n] = 1.0 / possible_action_num;
                } else {
                  (*this)[j][k][l][i][n] = 0.0;
                }
                original_cover_stability_[j][k][l][i][n] = 0;
              }
            }
          }
        }
      }
    } else {
      std::cout << "Loading model aborted." << std::endl;
      exit(1);
    }
  }
}

Policy &Policy::operator=(const Policy &p) {
  if (this != &p) {
    name_ = p.name_;
    id_ = p.id_;
    master_model_ = p.master_model_;
    original_cover_stability_ = p.original_cover_stability_;
    new_cover_stability_ = p.new_cover_stability_;
    declining_coefficient_ = p.declining_coefficient_;
    for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
      (*this)[i] = p[i];
    }
  }
  return *this;
}

void Policy::Normalize() {
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          float sum = 0.0;
          uint32_t infty_num = 0;
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if ((*this)[i][j][k][l][n] ==
                std::numeric_limits<float>::infinity()) {
              infty_num++;
            } else {
              sum += (*this)[i][j][k][l][n];
            }
          }
          if (infty_num == 0) {
            if (sum == 0) {
              uint32_t feasible_action_num = 0;
              for (uint32_t n = 0; n < ACTION_NUM; n++) {
                if (Game::actions[n].GetEnergy() <= l) {
                  feasible_action_num++;
                }
              }
              for (uint32_t n = 0; n < ACTION_NUM; n++) {
                if (Game::actions[n].GetEnergy() <= l) {
                  (*this)[i][j][k][l][n] = 1.0 / feasible_action_num;
                } else {
                  (*this)[i][j][k][l][n] = 0;
                }
              }
            } else {
              for (uint32_t n = 0; n < ACTION_NUM; n++) {
                if ((*this)[i][j][k][l][n] != 0) {
                  (*this)[i][j][k][l][n] /= sum;
                }
              }
            }
          } else {
            for (uint32_t n = 0; n < ACTION_NUM; n++) {
              if ((*this)[i][j][k][l][n] !=
                  std::numeric_limits<float>::infinity()) {
                (*this)[i][j][k][l][n] = 0;
              } else {
                (*this)[i][j][k][l][n] = 1.0 / infty_num;
              }
            }
          }
        }
      }
    }
  }
}

bool Policy::IsNormalized() {
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          float sum = 0.0;
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            sum += (*this)[i][j][k][l][n];
          }
          if (std::abs(sum - 1) > 0.00001) {
            std::cerr << "Error: Policy is not normalized at (" << i << ", "
                      << j << ", " << k << ", " << l << "). The value is "
                      << sum << std::endl;
            return false;
          }
        }
      }
    }
  }
  return true;
}

void Policy::Update() {
  id_++;

  const uint32_t num_threads = std::thread::hardware_concurrency() - 1;
  std::vector<std::thread> threads;
  std::atomic<uint32_t> completed_threads(0);

  auto thread_task = [&](uint32_t start, uint32_t end) {
    for (uint32_t idx = start; idx <= end; ++idx) {
      uint32_t k = idx / (MAX_ENERGY + 1);
      uint32_t l = idx % (MAX_ENERGY + 1);
      for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
        for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
          State state = {static_cast<float>(i), static_cast<float>(j),
                         static_cast<float>(k), static_cast<float>(l)};

          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            (*this)[i][j][k][l][n] =
                Attention(master_model_->GetActionWinningRate(state, n));
            new_cover_stability_[i][j][k][l][n] = 0;
            for (uint32_t n_prime = 0; n_prime < ACTION_NUM; n_prime++) {
              bool IsCovered = true;
              bool IsStableCovered = true;
              bool IsEqual = true;
              for (uint32_t m = 0; m < ACTION_NUM; m++) {
                if (Game::actions[m].GetEnergy() > k) {
                  continue;
                }
                State new_state =
                    StateTransform(state, Game::actions[m], Game::actions[n]);
                State new_state_prime = StateTransform(state, Game::actions[m],
                                                       Game::actions[n_prime]);
                float winning_rate =
                    DeclineFunction(
                        master_model_->GetStateWinningRate(new_state));
                float winning_rate_prime =
                    DeclineFunction(
                        master_model_->GetStateWinningRate(new_state_prime));
                if (winning_rate > winning_rate_prime) {
                  IsStableCovered = false;
                  if (GetOriginalCoverStability(ConjugateState(state), m) ==
                      0) {
                    IsCovered = false;
                    break;
                  }
                }
                if (winning_rate < winning_rate_prime) {
                  IsEqual = false;
                }
              }

              if (IsStableCovered && !IsEqual) {
                new_cover_stability_[i][j][k][l][n] = 1;
              }
              if (IsCovered && !IsEqual) {
                (*this)[i][j][k][l][n] = 0;
                break;
              }
            }
          }
        }
      }
    }
    completed_threads++;
  };

  uint32_t total_tasks = (MAX_ENERGY + 1) * (MAX_ENERGY + 1);
  uint32_t range = total_tasks / num_threads;
  for (uint32_t t = 0; t < num_threads; ++t) {
    uint32_t start = t * range;
    uint32_t end =
        (t == num_threads - 1) ? total_tasks - 1 : (t + 1) * range - 1;
    threads.emplace_back(thread_task, start, end);
  }

  while (completed_threads < num_threads) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    PrintProgressBar(completed_threads, num_threads);
  }

  for (auto &thread : threads) {
    thread.join();
  }

  PrintProgressBar(num_threads, num_threads);
  std::cout << std::endl;

  Normalize();
}

float Policy::Similarity(const Policy &p) {
  float tvd = 0.0;
  float total_entry = 0.0;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            if (Game::actions[n].GetEnergy() <= l) {
              tvd += std::abs((*this)[i][j][k][l][n] - p[i][j][k][l][n]) / 2;
            }
          }
          total_entry += 1;
        }
      }
    }
  }
  return 1 - tvd / total_entry;
}

void Policy::Store() {
  std::filesystem::path path =
      GetModelPath(name_).parent_path() / (name_ + "-tmp") / "policy.txt";
  std::filesystem::create_directories(path.parent_path());

  std::ofstream fout(path);
  fout << name_ << " " << id_ << std::endl;
  fout << declining_coefficient_ << std::endl;
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

  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            fout << new_cover_stability_[i][j][k][l][n] << " ";
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

float Policy::GetActionProbability(float enemy_health,
                                   float health,
                                   float enemy_energy,
                                   float energy,
                                   uint32_t action_id) {
  if (enemy_health <= 0 || enemy_health > MAX_HEALTH || health <= 0 ||
      health > MAX_HEALTH || enemy_energy < 0 || enemy_energy > MAX_ENERGY ||
      energy < 0 || energy > MAX_ENERGY || action_id >= ACTION_NUM) {
    std::cerr << "Error: policy out of range" << std::endl;
    exit(1);
  }
  return (*this)[enemy_health][health][enemy_energy][energy][action_id];
}

bool Policy::GetOriginalCoverStability(float enemy_health,
                                       float health,
                                       float enemy_energy,
                                       float energy,
                                       uint32_t action_id) {
  if (enemy_health <= 0 || enemy_health > MAX_HEALTH || health <= 0 ||
      health > MAX_HEALTH || enemy_energy < 0 || enemy_energy > MAX_ENERGY ||
      energy < 0 || energy > MAX_ENERGY || action_id >= ACTION_NUM) {
    std::cerr << "Error: policy out of range" << std::endl;
    exit(1);
  }
  return original_cover_stability_[enemy_health][health][enemy_energy][energy]
                                  [action_id];
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
              << 100 * (*this)[enemy_health][health][enemy_energy][energy]
                              [current_max_action]
              << std::endl;
  } while (!PrintIsOver);
}

Game::Action *Policy::GetAction(float enemy_health,
                                float health,
                                float enemy_energy,
                                float energy) {
  std::vector<float> probabilities;
  for (uint32_t n = 0; n < ACTION_NUM; n++) {
    probabilities.push_back(
        (*this)[enemy_health][health][enemy_energy][energy][n]);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());

  int index = dist(gen);
  return &Game::actions[index];
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

}  // namespace HDP

}  // namespace AI