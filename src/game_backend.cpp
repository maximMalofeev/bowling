#include "game_backend.h"
#include "input.h"
#include <map>
#include <list>

namespace bowling {

struct GameBackend::Implementation {
  Input input;
  State state = State::NotStarted;

  int frame = -1;
  int player = -1;
  int ball = -1;
  int pins = -1;

  int expectedFrame = 1;
  int expectedPlayer = 1;
  int expectedBall = 1;
  int availablePins = 10;

  typedef std::pair<FrameResult, std::list<int>> frame_result;
  std::map<int, std::map<int, frame_result>> results;
  std::map<int, int> scores;

  bool checkIfRulesObserved(){
    if (frame != expectedFrame || frame > 10 || player != expectedPlayer ||
        ball != expectedBall || pins > availablePins) {
      return false;
    }
    return true;
  }

  void processStep(){
    results[frame][player].second.push_back(pins);
    if(ball == 1){
      if(pins == 10){
        // This is strike
        results[frame][player].first = FrameResult::Strike;
        nextPlayer();
      }else{
        availablePins -= pins;
        expectedBall++;
      }
    }else{
      if(pins == availablePins){
        // This is spare
        results[frame][player].first = FrameResult::Spare;
      }else{
        results[frame][player].first = FrameResult::NothingEspecial;
      }
      availablePins = 10;
      expectedBall = 1;
      nextPlayer();
    }
  }

  void processTenthFrameStep(){
    results[frame][player].second.push_back(pins);
    if(ball == 1){
      if(pins == 10){
        // This is strike, tenth frame gives 2 extra balls
        results[frame][player].first = FrameResult::Strike;
        expectedBall++;
      }else{
        availablePins -= pins;
        expectedBall++;
      }
    }else{
      // If first ball were strike
      if(results[frame][player].first == FrameResult::Strike){
        if(ball == 2){
          expectedBall++;
          if(pins == 10){
            availablePins = 10;
          }else{
            availablePins -= pins;
          }
        }else{
          expectedBall = 1;
          availablePins = 10;
          nextPlayer();
        }
      }else if(results[frame][player].first == FrameResult::Spare){
        expectedBall = 1;
        availablePins = 10;
        nextPlayer();
      } else{
        if(pins == availablePins){
          // This is spare
          results[frame][player].first = FrameResult::Spare;
          expectedBall++;
        }else{
          results[frame][player].first = FrameResult::NothingEspecial;
          expectedBall = 1;
          nextPlayer();
        }
        availablePins = 10;
      }
    }
  }

  void countScore(){
    for(auto& s : scores){
      s.second = 0;
    }

    for(size_t p = 1; p <= input.playersNum(); p++){
      std::list<int> multipliers;
      for(auto& frame : results){
        // check if player make step
        if(frame.second.find(p) != frame.second.end()){
          for(auto& pins : frame.second[p].second){
            scores[p] += pins;
            for(auto& m : multipliers){
              scores[p] += pins;
              m--;
            }
            multipliers.erase(std::remove(multipliers.begin(), multipliers.end(), 0), multipliers.end());
          }

          if(frame.second[p].first == FrameResult::Strike){
            multipliers.push_back(2);
          }
          if(frame.second[p].first == FrameResult::Spare){
            multipliers.push_back(1);
          }
        }
      }
    }
  }

 private:
  void nextPlayer(){
    if(player == input.playersNum()){
      expectedPlayer = 1;
      expectedFrame++;
    }else{
      expectedPlayer++;
    }
  }
};

GameBackend::GameBackend() { impl_.reset(new Implementation); }

GameBackend::~GameBackend() {}

GameBackend::State GameBackend::state() const { return impl_->state; }

bool GameBackend::start(const fs::path &input) {
  if (impl_->input.setInput(input)) {
    impl_->state = State::Started;
    return true;
  }
  return false;
}

int GameBackend::players() const {
  return impl_->state == State::NotStarted ? -1 : impl_->input.playersNum();
}

int GameBackend::frame() const { return impl_->frame; }

int GameBackend::player() const { return impl_->player; }

int GameBackend::ball() const { return impl_->ball; }

int GameBackend::pins() const { return impl_->pins; }

int GameBackend::score(int player) const
{
  return impl_->scores[player];
}

bool GameBackend::makeStep() {
  if(impl_->input.nextStep(impl_->frame, impl_->player, impl_->ball,
                            impl_->pins)){
    if(!impl_->checkIfRulesObserved()){
      impl_->state = State::RulesBroken;
      return false;
    }
    if(impl_->frame != 10){
      impl_->processStep();
    }else{
      // tenth frame has especial rules
      impl_->processTenthFrameStep();
    }
    impl_->countScore();
    return true;
  }else{
    impl_->state = State::GameOver;
    return false;
  }
}

}  // namespace bowling
