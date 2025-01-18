#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "game/battle_field.h"

PYBIND11_MODULE(handsclapping, m) {
  m.def("InitActions", &Game::InitActions, "Initialize the actions");
  pybind11::class_<Game::BattleField>(m, "BattleField")
      .def(pybind11::init<>(), "Default constructor")
      .def(pybind11::init<std::vector<std::string>>(),
           "Constructor with player names", pybind11::arg("player_names"))
      .def("GetMemberNum", &Game::BattleField::GetMemberNum,
           "Get the number of players")
      .def("GetPlayerName", &Game::BattleField::GetPlayerName,
           "Get the name of the player")
      .def("PrintBattleField", &Game::BattleField::PrintBattleField,
           "Print the battle field")
      .def("ActionUpdate", &Game::BattleField::ActionUpdate,
           "Update the action")
      .def("PositionUpdate", &Game::BattleField::PositionUpdate,
           "Update the position")
      .def("EnergyUpdate", &Game::BattleField::EnergyUpdate,
           "Update the energy")
      .def("HealthUpdate", &Game::BattleField::HealthUpdate,
           "Update the health")
      .def("MemberNumUpdate", &Game::BattleField::MemberNumUpdate,
           "Update the number of members");
}