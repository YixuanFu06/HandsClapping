#pragma once

#include <filesystem>
#include <functional>

#include "../../../game/define_actions.h"
#include "tensor.h"

namespace AI {

namespace Idiot {

const uint32_t STATE_DIM = 2;
const uint32_t ACTION_NUM = 38;
const uint32_t MAX_ENERGY = 15;
const uint32_t MAX_HEALTH = 5;
const float default_update_precision = 100.0;     // greater than 0
const float default_declining_coefficient = 0.7;  // in [0,1]
const float default_conservative_coefficient =
    2.0;                                       // greater than or equals to 0
const float default_greedy_coefficient = 0.5;  // greater than or equals to 0
const float default_aggressive_coefficient =
    0.5;  // greater than or equals to 0

std::filesystem::path FindRootPath();
std::filesystem::path GetPolicyPath(const std::string &name);

class Policy;

class Reward : public Tensor<float, STATE_DIM * 2 + 1> {
  friend class Policy;

 private:
  uint32_t id_;
  float declining_coefficient_;     // in [0,1]
  float conservative_coefficient_;  // greater than or equals to 0
  float greedy_coefficient_;        // greater than or equals to 0
  float aggressive_coefficient_;    // greater than or equals to 0

 public:
  Reward();
  Reward(const Policy &p);
  Reward(const Reward &r) : Tensor<float, STATE_DIM * 2 + 1>(r), id_(r.id_) {
  }
  Reward &operator=(const Reward &r);
  Reward operator+(const Reward &r);
  Reward operator-(const Reward &r);
  Reward operator*(const float f);
  Reward &operator+=(const Reward &r);
  Reward &operator-=(const Reward &r);
  Reward &operator*=(const float f);
  ~Reward() {
  }

  void StrategyUpdate(float enemy_health,
                      float enemy_energy,
                      float health,
                      float energy,
                      Game::Action *enemy_action,
                      Game::Action *action);
  void ActionUpdate_Health(float enemy_health,
                           float enemy_energy,
                           float health,
                           float energy,
                           Game::Action *action,
                           float chang_of_health);
  void ActionUpdate_Energy(float enemy_health,
                           float enemy_energy,
                           float health,
                           float energy,
                           Game::Action *action,
                           float change_of_energy);
  void Store(const std::string &path);

  inline uint32_t GetId() const {
    return id_;
  }
};

class Policy : public Tensor<float, STATE_DIM * 2 + 1> {
  friend class Reward;

 private:
  std::string name_;
  uint32_t id_;
  float update_precision_;          // greater than 0
  float declining_coefficient_;     // in [0,1]
  float conservative_coefficient_;  // greater than or equals to 0
  float greedy_coefficient_;        // greater than or equals to 0
  float aggressive_coefficient_;    // greater than or equals to 0

 public:
  Policy();
  Policy(const std::string name, uint32_t id);
  Policy(const Policy &p)
      : Tensor<float, STATE_DIM * 2 + 1>(p), name_(p.name_), id_(p.id_) {
  }
  Policy(const std::string &path);
  Policy(const std::filesystem::path &path);
  Policy &operator=(const Policy &p);
  Policy operator+(const Reward &r);
  Policy operator*(const Reward &r);
  Policy &operator+=(const Reward &r);
  Policy &operator*=(const Reward &r);
  ~Policy() {
  }

  void Store();
  void Store(const std::string &name);
  void Normalize();
  void Update(Reward &r);
  void Update(Reward &&r);
  void RewardFeedback(Reward &r);
  float Similarity(
      const Policy &p);  // get the tv-distance between two policies
  void PrintDistribution(float enemy_health, float health, float enemy_energy, float energy);
  Game::Action *GetAction(uint32_t health,
                          uint32_t energy,
                          uint32_t enemy_health,
                          uint32_t enemy_energy);

  inline std::string GetName() const {
    return name_;
  }
  inline void SetName(std::string name) {
    name_ = name;
  }
  inline uint32_t GetId() const {
    return id_;
  }

  void SetUpdatePrecision(float update_precision);
  inline float GetUpdatePrecision() {
    return update_precision_;
  }
  void SetDecliningCoefficient(float declining_coefficient);
  inline float GetDecliningCoefficient() {
    return declining_coefficient_;
  }
  void SetConservativeCoefficient(float conservative_coefficient);
  inline float GetConservativeCoefficient() {
    return conservative_coefficient_;
  }
  void SetGreedyCoefficient(float greedy_coefficient);
  float GetGreedyCoefficient() {
    return greedy_coefficient_;
  }
  void SetAggressiveCoefficient(float aggressive_coefficient);
  float GetAggressiveCoefficient() {
    return aggressive_coefficient_;
  }
};

}  // namespace Idiot

}  // namespace AI