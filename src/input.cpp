#include "input.h"
#include <fstream>

namespace bowling {

struct Input::Implementation {
  fs::path input;
  std::fstream inputFile;
  size_t playersNum{};
};

Input::Input() { impl_.reset(new Implementation); }

Input::~Input() {
  if (impl_->inputFile.is_open()) {
    impl_->inputFile.close();
  }
};

const fs::path &Input::input() const { return impl_->input; }

bool Input::setInput(const fs::path &input) {
  if (fs::exists(input) && fs::is_regular_file(input)) {
    impl_->inputFile.open(input, std::ios_base::in);
    if (impl_->inputFile && impl_->inputFile >> impl_->playersNum && impl_->playersNum <= 5) {
      impl_->input = input;
      return true;
    }
    impl_->playersNum = 0;
    impl_->inputFile.close();
    return false;
  }
  return false;
}

size_t Input::playersNum() const { return impl_->playersNum; }

bool Input::nextStep(int &frame, int &player, int &ball, int &pins) {
  if (impl_->inputFile && impl_->inputFile >> frame && impl_->inputFile >> player &&
      impl_->inputFile >> ball && impl_->inputFile >> pins) {
    return true;
  }
  return false;
}

}  // namespace bowling
