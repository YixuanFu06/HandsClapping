#pragma once

#include <cmath>
#include <cstdint>

#include "battle_field.h"

namespace Game {

uint32_t DIMENSION = 2;
uint32_t POSITION_NUM = std::pow(3, DIMENSION);
uint32_t CENTER = POSITION_NUM / 2;

enum PlayerPosition {
  UpLeft = 0,
  UpMid = 1,
  UpRight = 2,
  GroundLeft = 3,
  GroundMid = 4,
  GroundRight = 5,
  DownLeft = 6,
  DownMid = 7,
  DownRight = 8
};

class Player {
 private:
  float health_;
  float energy_;
  PlayerPosition position_;

 public:
  inline float GetHealth();
  inline void SetHealth(float health);

  inline float GetEnegy();
  inline void SetEnergy(float energy);

  inline PlayerPosition GetPosition();
};

}  // namespace Game