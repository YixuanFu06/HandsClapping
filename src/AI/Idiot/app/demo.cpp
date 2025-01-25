#include <filesystem>

#include "../../../game/battle_field.h"
#include "../include/policy.h"

const std::string DATA_PATH = "./data/AI/Idiot/policy.txt";

int main() {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path data_path = current_path / DATA_PATH;
    std::filesystem::create_directories(data_path.parent_path());
    Game::InitActions();
    AI::Idiot::Policy policy = AI::Idiot::Policy(data_path.string());

    Game::BattleField battle_field({"player1", "player2"});
    while (battle_field.GetMemberNum() > 1) {
        float health1 = battle_field.GetPlayerHealth(0);
        float energy1 = battle_field.GetPlayerEnergy(0);
        float health2 = battle_field.GetPlayerHealth(1);
        float energy2 = battle_field.GetPlayerEnergy(1);
        Game::Action *action1 = policy.GetAction(health2, energy2, health1, energy1);
        Game::Action *action2 = policy.GetAction(health1, energy1, health2, energy2);
        battle_field.BattleFieldUpdate({action1->GetFormalName(), action2->GetFormalName()});
    }
    if (battle_field.GetMemberNum() < 1) {
        std::cout << "Game over! No winner!" << std::endl;
    } else {
        std::cout << "Game over! " << battle_field.GetPlayerName(0) << " wins!" << std::endl;
    }

    return 0;
}