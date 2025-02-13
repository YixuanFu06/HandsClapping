#include "../include/model.h"
#include "define_actions.h"

const std::string MODEL = "HDP-alpha";

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::HDP::FindRootPath()
            << std::endl
            << std::endl;
  AI::HDP::Model model(MODEL);
  model.Update();
  model.Store();
  std::cout << "New files are stored at "
            << AI::HDP::GetModelPath(MODEL).parent_path() / (MODEL + "-tmp")
            << ". Do you want to cover the original files? (y/n) " << std::endl;
  std::string YesOrNo;
  std::cin >> YesOrNo;
  if (YesOrNo == "y") {
    model.Rewrite();
    std::cout << "New model stored." << std::endl;
  } else {
    model.Abort();
    std::cout << "New model aborted." << std::endl;
  }
  return 0;
}