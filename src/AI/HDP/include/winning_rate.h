#pragma once

#include "../../tensor.h"
#include "state.h"

namespace AI {

namespace HDP {

const uint32_t STATE_DIM = 2;
const uint32_t ACTION_NUM = 36;
const uint32_t MAX_ENERGY = 15;
const uint32_t MAX_HEALTH = 5;

class Model;

class StateWinningRate : public Tensor<float, STATE_DIM * 2> {
 private:
  std::string name_;
  uint32_t id_;
  Model *master_model_;

 public:
  StateWinningRate();
  StateWinningRate(const StateWinningRate &swr);
  StateWinningRate(Model *master_model_);
  StateWinningRate &operator=(const StateWinningRate &swr);
  ~StateWinningRate() {
  }

  float GetStateWinningRate(float enemy_health,
                            float health,
                            float enemy_energy,
                            float energy);
  inline float GetStateWinningRate(const State &state) {
    return GetStateWinningRate(state.enemy_health, state.health,
                               state.enemy_energy, state.energy);
  }
  void Store();
  void Normalize();
  void Update();
  float Similarity(const StateWinningRate &swr);

  inline void SetName(std::string name) {
    name_ = name;
  }
  inline std::string GetName() const {
    return name_;
  }
  inline void SetId(uint32_t id) {
    id_ = id;
  }
  inline uint32_t GetId() const {
    return id_;
  }
};

class ActionWinningRate : public Tensor<float, STATE_DIM * 2 + 1> {
 private:
  std::string name_;
  uint32_t id_;
  Model *master_model_;

 public:
  ActionWinningRate();
  ActionWinningRate(const ActionWinningRate &awr);
  ActionWinningRate(Model *master_model_);
  ActionWinningRate &operator=(const ActionWinningRate &awr);
  ~ActionWinningRate() {
  }

  float GetActionWinningRate(float enemy_health,
                             float health,
                             float enemy_energy,
                             float energy,
                             uint32_t action_id);
  inline float GetActionWinningRate(const State &state, uint32_t action_id) {
    return GetActionWinningRate(state.enemy_health, state.health,
                                state.enemy_energy, state.energy, action_id);
  }
  void Update();
  void Store();
  float Similarity(const ActionWinningRate &awr);

  inline void SetName(std::string name) {
    name_ = name;
  }
  inline std::string GetName() const {
    return name_;
  }
  inline void SetId(uint32_t id) {
    id_ = id;
  }
  inline uint32_t GetId() const {
    return id_;
  }
};

}  // namespace HDP

}  // namespace AI