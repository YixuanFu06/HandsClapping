#pragma once

#include "policy.h"

namespace AI {

namespace HDP {

std::filesystem::path FindRootPath();
std::filesystem::path GetModelPath(const std::string &name);
void PrintProgressBar(int current, int total);

class Model {
 private:
  std::string name_;
  uint32_t id_;
  Policy policy_;
  StateWinningRate state_winning_rate_;
  ActionWinningRate action_winning_rate_;

 public:
  Model() = default;
  Model(const Model &m);
  Model(const std::string &name);
  Model &operator=(const Model &m);
  ~Model() {
  }

  void Store();
  void Rewrite();
  void Abort();

  void Update();

  float Similarity(const Model &m);

  inline float GetActionProbability(float enemy_health,
                                    float health,
                                    float enemy_energy,
                                    float energy,
                                    uint32_t action_id) {
    return policy_.GetActionProbability(enemy_health, health, enemy_energy,
                                        energy, action_id);
  }

  inline float GetActionProbability(const State &state, uint32_t action_id) {
    return GetActionProbability(state.enemy_health, state.health,
                                state.enemy_energy, state.energy, action_id);
  }
  inline bool GetOriginalCoverStability(float enemy_health,
                                        float health,
                                        float enemy_energy,
                                        float energy,
                                        uint32_t action_id) {
    return policy_.GetOriginalCoverStability(enemy_health, health, enemy_energy,
                                             energy, action_id);
  }
  inline bool GetOriginalCoverStability(const State &state,
                                        uint32_t action_id) {
    return GetOriginalCoverStability(state.enemy_health, state.health,
                                     state.enemy_energy, state.energy,
                                     action_id);
  }
  inline void PrintDistribution(float enemy_health,
                                float health,
                                float enemy_energy,
                                float energy) {
    policy_.PrintDistribution(enemy_health, health, enemy_energy, energy);
  }
  inline void PrintDistribution(const State &state) {
    PrintDistribution(state.enemy_health, state.health, state.enemy_energy,
                      state.energy);
  }
  inline Game::Action *GetAction(float enemy_health,
                                 float health,
                                 float enemy_energy,
                                 float energy) {
    return policy_.GetAction(enemy_health, health, enemy_energy, energy);
  }
  inline Game::Action *GetAction(const State &state) {
    return GetAction(state.enemy_health, state.health, state.enemy_energy,
                     state.energy);
  }

  inline std::string GetName() const {
    return name_;
  }
  inline void SetName(std::string name) {
    name_ = name;
    policy_.SetName(name);
    state_winning_rate_.SetName(name);
    action_winning_rate_.SetName(name);
  }
  inline uint32_t GetId() const {
    return id_;
  }

  inline void SetDecliningCoefficient(float declining_coefficient) {
    policy_.SetDecliningCoefficient(declining_coefficient);
  }
  inline float GetDecliningCoefficient() {
    return policy_.GetDecliningCoefficient();
  }
  inline float DeclineFunction(float x) {
    return 4 * (1 - GetDecliningCoefficient()) * std::pow(x - 0.5, 3) +
           GetDecliningCoefficient() * (x - 0.5) + 0.5;
  }

  inline float GetStateWinningRate(float enemy_health,
                                   float health,
                                   float enemy_energy,
                                   float energy) {
    return state_winning_rate_.GetStateWinningRate(enemy_health, health,
                                                   enemy_energy, energy);
  }
  inline float GetStateWinningRate(const State &state) {
    return GetStateWinningRate(state.enemy_health, state.health,
                               state.enemy_energy, state.energy);
  }

  inline float GetActionWinningRate(float enemy_health,
                                    float health,
                                    float enemy_energy,
                                    float energy,
                                    uint32_t action_id) {
    return action_winning_rate_.GetActionWinningRate(
        enemy_health, health, enemy_energy, energy, action_id);
  }
  inline float GetActionWinningRate(const State &state, uint32_t action_id) {
    return GetActionWinningRate(state.enemy_health, state.health,
                                state.enemy_energy, state.energy, action_id);
  }
};

}  // namespace HDP

}  // namespace AI