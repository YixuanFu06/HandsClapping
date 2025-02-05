#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "game/battle_field.h"

namespace py = pybind11;

PYBIND11_MODULE(handsclapping, m) {
  m.def("InitActions", &Game::InitActions, "Initialize the actions");
  py::class_<Game::BattleField>(m, "BattleField")
      .def(py::init<>(), "Default constructor")
      .def(py::init<std::vector<std::string>>(),
           "Constructor with player names", py::arg("player_names"))
      .def("GetMemberNum", &Game::BattleField::GetMemberNum,
           "Get the number of players")
      .def("GetTurn", &Game::BattleField::GetTurn, "Get the turn")
      .def("GetPlayerName", &Game::BattleField::GetPlayerName,
           "Get the name of the player")
      .def("GetPlayerHealth", &Game::BattleField::GetPlayerHealth,
           "Get the health of the player")
      .def("GetPlayerEnergy", &Game::BattleField::GetPlayerEnergy,
           "Get the energy of the player")
      .def("PrintBattleField", &Game::BattleField::PrintBattleField,
           "Print the battle field")
      .def("GetBattleFieldMessage", &Game::BattleField::GetBattleFieldMessage,
           "Print the battle field message")
      .def("BattleFieldUpdate",
           py::overload_cast<uint32_t>(&Game::BattleField::BattleFieldUpdate),
           "Update the battle field")
      .def("BattleFieldUpdate",
           py::overload_cast<std::vector<std::string>, uint32_t>(
               &Game::BattleField::BattleFieldUpdate),
           "Update the battle field with input action names")
      .def("ActionUpdate",
           py::overload_cast<uint32_t>(&Game::BattleField::ActionUpdate),
           "Update the action")
      .def("ActionUpdate",
           py::overload_cast<std::vector<std::string>, uint32_t>(
               &Game::BattleField::ActionUpdate),
           "Update the action with input action names")
      .def("PositionUpdate", &Game::BattleField::PositionUpdate,
           "Update the position")
      .def("EnergyUpdate", &Game::BattleField::EnergyUpdate,
           "Update the energy")
      .def("HealthUpdate", &Game::BattleField::HealthUpdate,
           "Update the health")
      .def("MemberNumUpdate", &Game::BattleField::MemberNumUpdate,
           "Update the number of members")
      .def("TurnUpdate", &Game::BattleField::TurnUpdate, "Update the turn");
}