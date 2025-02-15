#include "model.h"

namespace AI {

namespace HDP {

void PrintProgressBar(int current, int total) {
  int bar_width = 70;
  float progress = static_cast<float>(current) / total;

  std::cout << "[";
  int pos = bar_width * progress;
  for (int i = 0; i < bar_width; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";
  std::cout.flush();
}

std::filesystem::path FindRootPath() {
  std::filesystem::path current_path = std::filesystem::current_path();
  std::filesystem::path root_path;

  while (current_path.has_parent_path()) {
    if (std::filesystem::exists(current_path / "HandsClapping") &&
        std::filesystem::is_directory(current_path / "HandsClapping")) {
      root_path = current_path / "HandsClapping";
      break;
    }
    current_path = current_path.parent_path();
  }

  if (root_path.empty()) {
    std::cerr << "Error: HandsClapping directory not found in " << std::filesystem::current_path()
              << ". Please run the program under the project directory."
              << std::endl;
    exit(1);
  }

  return root_path;
}

std::filesystem::path GetModelPath(const std::string &name) {
  std::filesystem::path root_path = FindRootPath();
  std::filesystem::path policy_path = root_path / "data" / "AI" / "HDP" / name;
  return policy_path;
}

Model::Model(const Model &m)
    : name_(m.name_),
      id_(m.id_),
      policy_(m.policy_),
      state_winning_rate_(m.state_winning_rate_),
      action_winning_rate_(m.action_winning_rate_) {
}

Model::Model(const std::string &name) : name_(name) {
  policy_ = Policy(this);
  id_ = policy_.GetId();
  state_winning_rate_ = StateWinningRate(this);
  action_winning_rate_ = ActionWinningRate(this);
}

Model &Model::operator=(const Model &m) {
  if (this != &m) {
    name_ = m.name_;
    id_ = m.id_;
    policy_ = m.policy_;
    state_winning_rate_ = m.state_winning_rate_;
    action_winning_rate_ = m.action_winning_rate_;
  }
  return *this;
}

void Model::Store() {
  policy_.Store();
  state_winning_rate_.Store();
  action_winning_rate_.Store();
}

void Model::Rewrite() {
  std::filesystem::path path = GetModelPath(name_);
  std::filesystem::remove_all(path);
  std::filesystem::rename(path.parent_path() / (name_ + "-tmp"), path);
}

void Model::Abort() {
  std::filesystem::path path =
      GetModelPath(name_).parent_path() / (name_ + "-tmp");
  std::filesystem::remove_all(path);
}

void Model::Update() {
  if (action_winning_rate_.GetId() != id_) {
    std::cerr << "Error: Model and ActionWinningRate do not match."
              << std::endl;
    exit(1);
  }
  action_winning_rate_.Update();
  std::cout << "action_winning_rate update finished." << std::endl;

  if (policy_.GetId() != id_) {
    std::cerr << "Error: Model and NewPolicy do not match." << std::endl;
    exit(1);
  }
  policy_.Update();
  policy_.IsNormalized();
  std::cout << "Policy update finished." << std::endl;

  action_winning_rate_.Update();
  action_winning_rate_.SetId(action_winning_rate_.GetId() - 1);
  std::cout << "action_winning_rate update finished." << std::endl;

  if (state_winning_rate_.GetId() != id_) {
    std::cerr << "Error: Model and StateWinningRate do not match." << std::endl;
    exit(1);
  }
  state_winning_rate_.Update();
  std::cout << "state_winning_rate update finished." << std::endl;

  id_++;
}

float Model::Similarity(const Model &m) {
  float policy_similarity = policy_.Similarity(m.policy_);
  float state_winning_rate_similarity =
      state_winning_rate_.Similarity(m.state_winning_rate_);
  float action_winning_rate_similarity =
      action_winning_rate_.Similarity(m.action_winning_rate_);

  return policy_similarity * state_winning_rate_similarity *
         action_winning_rate_similarity;
}

}  // namespace HDP

}  // namespace AI