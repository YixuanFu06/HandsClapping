#include "../game/battle_field.h"

int main() {
  Game::InitActions();
  uint32_t player_num;
  std::cout << "Please input the number of players: ";
  std::cin >> player_num;
  std::vector<std::string> player_names;
  for (uint32_t i = 0; i < player_num; i++) {
    std::string player_name;
    std::cout << "Please input the name of player " << i + 1 << ": ";
    std::cin >> player_name;
    player_names.push_back(player_name);
  }
  Game::BattleField battle_field(player_names);
  while (battle_field.GetMemberNum() > 1) {
    battle_field.BattleFieldUpdate(1);
  }
  std::cout << "Game Over! ";
  if (battle_field.GetMemberNum() == 0) {
    std::cout << "No one wins!" << std::endl;
  } else {
    std::cout << battle_field.GetPlayerName(0) << " wins!" << std::endl;
  }

  std::cout << "Press any key to continue..." << std::endl;
  system("pause");
  return 0;
}