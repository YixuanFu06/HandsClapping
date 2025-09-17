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
      .def("AddPlayer",
           py::overload_cast<std::string, float, float>(
               &Game::BattleField::AddPlayer),
           "Add a player with custom health and energy")
      .def("RemovePlayer",
           py::overload_cast<std::string>(&Game::BattleField::RemovePlayer),
           "Remove a player by name")
      .def("GetMemberNum", &Game::BattleField::GetMemberNum,
           "Get the number of players")
      .def("GetTurn", &Game::BattleField::GetTurn, "Get the turn")
      .def("GetPlayerName", &Game::BattleField::GetPlayerName,
           "Get the name of the player by ID")
      .def("GetPlayerId", &Game::BattleField::GetPlayerId,
           "Get the ID of the player by name")
      .def("GetPlayerHealth", &Game::BattleField::GetPlayerHealth,
           "Get the health of the player")
      .def("GetPlayerEnergy", &Game::BattleField::GetPlayerEnergy,
           "Get the energy of the player")
      .def("GetActionFormalName", &Game::BattleField::GetActionFormalName,
           "Get the formal name of the action by ActionName (enum ID)")
      .def("GetActionEnergy", &Game::BattleField::GetActionEnergy,
           "Get the energy cost of the action by ActionName (enum ID)")
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
           py::overload_cast<>(&Game::BattleField::ActionUpdate),
           "Update the action")
      .def("ActionUpdate",
           py::overload_cast<std::vector<std::string>>(
               &Game::BattleField::ActionUpdate),
           "Update the action with input action names")
      .def("ActionUpdate",
           py::overload_cast<std::vector<uint32_t>>(
               &Game::BattleField::ActionUpdate),
           "Update the action with input action ids")
      .def("PositionUpdate", &Game::BattleField::PositionUpdate,
           "Update the position")
      .def("EnergyUpdate", &Game::BattleField::EnergyUpdate,
           "Update the energy")
      .def("HealthUpdate", &Game::BattleField::HealthUpdate,
           "Update the health")
      .def("MemberNumUpdate", &Game::BattleField::MemberNumUpdate,
           "Update the number of members")
      .def("TurnUpdate", &Game::BattleField::TurnUpdate, "Update the turn")
      .def("RemoveDead", &Game::BattleField::RemoveDead, "Remove dead players",
           py::arg("mode") = 0);
}