#include "winning_rate.h"

#include <atomic>
#include <fstream>
#include <thread>

#include "model.h"

namespace AI {

namespace HDP {

StateWinningRate::StateWinningRate()
    : Tensor<float, STATE_DIM * 2>(
          {MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1, MAX_ENERGY + 1}),
      id_(0),
      master_model_(nullptr) {
}

StateWinningRate::StateWinningRate(const StateWinningRate &swr)
    : Tensor<float, STATE_DIM * 2>(swr),
      name_(swr.name_),
      id_(swr.id_),
      master_model_(swr.master_model_) {
}

StateWinningRate::StateWinningRate(Model *master_model)
    : Tensor<float, STATE_DIM * 2>(
          {MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1, MAX_ENERGY + 1}),
      name_(master_model->GetName()),
      id_(master_model->GetId()),
      master_model_(master_model) {
  std::filesystem::path path = GetModelPath(name_) / "state_winning_rate.txt";

  if (std::filesystem::exists(path)) {
    std::ifstream fin(path);
    fin >> name_ >> id_;
    for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
      for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
        for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
          for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
            fin >> (*this)[i][j][k][l];
          }
        }
      }
    }
  } else {
    std::cout << "Winning rate file missing in model " << path
              << ". Are you going to generate one "
                 "with default settings? (y/n) "
              << std::endl;
    std::string YesOrNo;
    std::cin >> YesOrNo;
    if (YesOrNo == "y") {
      for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
        for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
          for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
            for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
              float eff_enemy_energy = i * 2 + k;
              float eff_energy = j * 2 + l;
              float enemy_energy_ratio =
                  eff_enemy_energy / (eff_enemy_energy + eff_energy);
              float energy_ratio = eff_energy / (eff_enemy_energy + eff_energy);
              (*this)[i][j][k][l] = 1 / (1 + Attention(enemy_energy_ratio) /
                                                 Attention(energy_ratio));
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

StateWinningRate &StateWinningRate::operator=(const StateWinningRate &swr) {
  if (this != &swr) {
    name_ = swr.name_;
    id_ = swr.id_;
    master_model_ = swr.master_model_;
    for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
      (*this)[i] = swr[i];
    }
  }
  return *this;
}

float StateWinningRate::GetStateWinningRate(float enemy_health,
                                            float health,
                                            float enemy_energy,
                                            float energy) {
  if (enemy_energy < 0 || energy < 0) {
    std::cerr << "Error: state_winning_rate out of range" << std::endl;
    exit(1);
  }
  if (enemy_health > 0 && health <= 0) {
    return 0;
  }
  if (enemy_health <= 0 && health > 0) {
    return 1;
  }
  if (enemy_health <= 0 && health <= 0) {
    return 0.5;
  }
  if (enemy_health > MAX_HEALTH) {
    enemy_health = MAX_HEALTH;
  }
  if (health > MAX_HEALTH) {
    health = MAX_HEALTH;
  }
  if (enemy_energy > MAX_ENERGY) {
    enemy_energy = MAX_ENERGY;
  }
  if (energy > MAX_ENERGY) {
    energy = MAX_ENERGY;
  }
  return (*this)[enemy_health][health][enemy_energy][energy];
}

void StateWinningRate::Store() {
  std::filesystem::path path = GetModelPath(name_).parent_path() /
                               (name_ + "-tmp") / "state_winning_rate.txt";
  std::filesystem::create_directories(path.parent_path());

  std::ofstream fout(path);
  if (!fout) {
    std::cerr << "Error: Cannot open file " << path << std::endl;
    exit(1);
  }
  fout << name_ << " " << id_ << std::endl;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          fout << (*this)[i][j][k][l] << " ";
        }
        fout << std::endl;
      }
      fout << std::endl;
    }
    fout << std::endl;
  }
  fout.close();
}

void StateWinningRate::Normalize() {
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = i; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = k; l <= MAX_ENERGY; l++) {
          if ((*this)[i][j][k][l] >= 1.00001 ||
              (*this)[i][j][k][l] <= -0.00001) {
            std::cerr << "Error: Invalid state winning rate "
                      << (*this)[i][j][k][l] << " should be in [0,1]"
                      << std::endl;
            exit(1);
          }
          if ((*this)[i][j][k][l] < 0.00001 && (*this)[i][j][k][l] > -0.00001) {
            (*this)[i][j][k][l] = 0;
            (*this)[j][i][l][k] = 1;
            continue;
          }
          if ((*this)[i][j][k][l] > 0.99999 && (*this)[i][j][k][l] < 1.00001) {
            (*this)[i][j][k][l] = 1;
            (*this)[j][i][l][k] = 0;
            continue;
          }

          (*this)[i][j][k][l] =
              ((*this)[i][j][k][l] + 1 - (*this)[j][i][l][k]) / 2;
          (*this)[j][i][l][k] = 1 - (*this)[i][j][k][l];
        }
      }
    }
  }
}

void StateWinningRate::Update() {
  id_++;

  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          (*this)[i][j][k][l] = 0;
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            (*this)[i][j][k][l] +=
                master_model_->GetActionProbability(i, j, k, l, n) *
                master_model_->GetActionWinningRate(i, j, k, l, n);
          }
        }
      }
    }
  }

  Normalize();
}

float StateWinningRate::Similarity(const StateWinningRate &swr) {
  float tvd = 0.0;
  float total_entry = 0.0;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          tvd += std::abs((*this)[i][j][k][l] - swr[i][j][k][l]) / 2;
          total_entry += 1;
        }
      }
    }
  }
  return 1 - tvd / total_entry;
}

ActionWinningRate::ActionWinningRate()
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      id_(0),
      master_model_(nullptr) {
}

ActionWinningRate::ActionWinningRate(const ActionWinningRate &awr)
    : Tensor<float, STATE_DIM * 2 + 1>(awr),
      name_(awr.name_),
      id_(awr.id_),
      master_model_(awr.master_model_) {
}

ActionWinningRate::ActionWinningRate(Model *master_model)
    : Tensor<float, STATE_DIM * 2 + 1>({MAX_HEALTH + 1, MAX_HEALTH + 1,
                                        MAX_ENERGY + 1, MAX_ENERGY + 1,
                                        ACTION_NUM}),
      id_(master_model->GetId()),
      name_(master_model->GetName()),
      master_model_(master_model) {
  std::filesystem::path path = GetModelPath(name_) / "action_winning_rate.txt";

  if (std::filesystem::exists(path)) {
    std::ifstream fin(path);
    fin >> name_ >> id_;
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
  }
}

ActionWinningRate &ActionWinningRate::operator=(const ActionWinningRate &awr) {
  if (this != &awr) {
    name_ = awr.name_;
    id_ = awr.id_;
    master_model_ = awr.master_model_;
    for (uint32_t i = 0; i <= MAX_HEALTH; i++) {
      (*this)[i] = awr[i];
    }
  }
  return *this;
}

float ActionWinningRate::GetActionWinningRate(float enemy_health,
                                              float health,
                                              float enemy_energy,
                                              float energy,
                                              uint32_t action_id) {
  if (enemy_health <= 0 || enemy_health > MAX_HEALTH || health <= 0 ||
      health > MAX_HEALTH || enemy_energy < 0 || enemy_energy > MAX_ENERGY ||
      energy < 0 || energy > MAX_ENERGY || action_id >= ACTION_NUM) {
    std::cerr << "Error: action_winning_rate out of range" << std::endl;
    exit(1);
  }
  return (*this)[enemy_health][health][enemy_energy][energy][action_id];
}

void ActionWinningRate::Update() {
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
            (*this)[i][j][k][l][n] = 0;
            for (uint32_t m = 0; m < ACTION_NUM; m++) {
              if (Game::actions[m].GetEnergy() > k) {
                continue;
              }
              State new_state =
                  StateTransform(state, Game::actions[m], Game::actions[n]);
              (*this)[i][j][k][l][n] +=
                  master_model_->GetActionProbability(ConjugateState(state),
                                                      m) *
                  (master_model_->DeclineFunction(
                      master_model_->GetStateWinningRate(new_state)));
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
}

void ActionWinningRate::Store() {
  std::filesystem::path path = GetModelPath(name_).parent_path() /
                               (name_ + "-tmp") / "action_winning_rate.txt";
  std::filesystem::create_directories(path.parent_path());

  std::ofstream fout(path);
  if (!fout) {
    std::cerr << "Error: Cannot open file " << path << std::endl;
    exit(1);
  }
  fout << name_ << " " << id_ << std::endl;
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
}

float ActionWinningRate::Similarity(const ActionWinningRate &awr) {
  float tvd = 0.0;
  float total_entry = 0.0;
  for (uint32_t i = 1; i <= MAX_HEALTH; i++) {
    for (uint32_t j = 1; j <= MAX_HEALTH; j++) {
      for (uint32_t k = 0; k <= MAX_ENERGY; k++) {
        for (uint32_t l = 0; l <= MAX_ENERGY; l++) {
          for (uint32_t n = 0; n < ACTION_NUM; n++) {
            tvd += std::abs((*this)[i][j][k][l][n] - awr[i][j][k][l][n]) / 2;
            total_entry += 1;
          }
        }
      }
    }
  }
  return 1 - tvd / total_entry;
}

}  // namespace HDP

}  // namespace AI