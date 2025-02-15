#pragma once

#include <cstdint>
#include <filesystem>

#include "../../tensor.h"
#include "winning_rate.h"

namespace AI {

namespace HDP {

const float default_declining_coefficient = 0.9;  // in [0,1]

class Model;

class Policy : public Tensor<float, STATE_DIM * 2 + 1> {
 private:
  std::string name_;
  uint32_t id_;
  Model *master_model_;
  Tensor<int, STATE_DIM * 2 + 1> original_cover_stability_;
  Tensor<int, STATE_DIM * 2 + 1> new_cover_stability_;
  float declining_coefficient_;  // in [0,1]

 public:
  Policy();
  Policy(const Policy &p);
  Policy(Model *master_model);
  Policy &operator=(const Policy &p);
  ~Policy() {
  }

  inline float DeclineFunction(float x) {
    return 4 * (1 - declining_coefficient_) * std::pow(x - 0.5, 3) +
           declining_coefficient_ * (x - 0.5) + 0.5;
  }
  void Normalize();
  bool IsNormalized();
  void Update();
  float Similarity(
      const Policy &p);  // get the tv-distance between two policies
  void Store();
  float GetActionProbability(float enemy_health,
                             float health,
                             float enemy_energy,
                             float energy,
                             uint32_t action_id);
  inline float GetActionProbability(const State &state, uint32_t action_id) {
    return GetActionProbability(state.enemy_health, state.health,
                                state.enemy_energy, state.energy, action_id);
  }
  bool GetOriginalCoverStability(float enemy_health,
                                 float health,
                                 float enemy_energy,
                                 float energy,
                                 uint32_t action_id);
  inline bool GetOriginalCoverStability(const State &state,
                                        uint32_t action_id) {
    return GetOriginalCoverStability(state.enemy_health, state.health,
                                     state.enemy_energy, state.energy,
                                     action_id);
  }
  void PrintDistribution(float enemy_health,
                         float health,
                         float enemy_energy,
                         float energy);
  inline void PrintDistribution(const State &state) {
    PrintDistribution(state.enemy_health, state.health, state.enemy_energy,
                      state.energy);
  }
  Game::Action *GetAction(float enemy_health,
                          float health,
                          float enemy_energy,
                          float energy);
  inline Game::Action *GetAction(const State &state) {
    return GetAction(state.enemy_health, state.health, state.enemy_energy,
                     state.energy);
  }

  inline std::string GetName() const {
    return name_;
  }
  inline void SetName(std::string name) {
    name_ = name;
  }
  inline uint32_t GetId() const {
    return id_;
  }

  void SetDecliningCoefficient(float declining_coefficient);
  inline float GetDecliningCoefficient() {
    return declining_coefficient_;
  }
};

}  // namespace HDP

}  // namespace AI