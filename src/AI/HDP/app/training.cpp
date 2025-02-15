#include "../include/model.h"
#include "define_actions.h"

int main() {
  Game::InitActions();
  std::cout << "HandsClapping directory found at: " << AI::HDP::FindRootPath()
            << std::endl
            << std::endl;
            
  std::cout << "Enter the name of the AI model you want to train: ";
  std::string model_name;
  std::cin >> model_name;
  AI::HDP::Model model(model_name);

  model.Update();
  model.Store();
  std::cout << "New files are stored at "
            << AI::HDP::GetModelPath(model_name).parent_path() / (model_name + "-tmp")
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

  std::cout << "Press any key to continue..." << std::endl;
  std::cin.get();
  return 0;
}