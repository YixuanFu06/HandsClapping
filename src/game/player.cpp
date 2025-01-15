#include "player.h"

namespace Game {
inline float Player::GetHealth() {
  return health_;
}

inline void Player::SetHealth(float health) {
  health_ = health;
}

inline float Player::GetEnegy() {
  return energy_;
}

inline void Player::SetEnergy(float energy) {
  energy_ = energy;
}

inline PlayerPosition Player::GetPosition() {
  return position_;
}
}  // namespace Game