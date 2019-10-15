#include "game.h"
#include <vector>
#include "input.h"

namespace bowling {

struct Game::Implementation {
  Input input;

  int expectedFrame = 1;
  int expectedPlayer = 1;
  int expectedBall = 1;
  int currentPins = 10;

  std::vector<std::vector<std::pair<Knock, std::pair<int, int>>>> results;
  std::vector<int> scores;
};

Game::Game(QObject *parent) : QObject(parent) {
  impl_.reset(new Implementation);
}

Game::~Game() {}

int Game::playersNumber() const { return impl_->input.playersNum(); }

int Game::currentFrame() const { return impl_->expectedFrame; }

int Game::currentPlayer() const { return impl_->expectedPlayer; }

QString Game::result(int frame, int player) const {
  if (impl_->results[player].size() > frame) {
    auto res = impl_->results[player][frame];
    if (res.first == Strike) {
      return QString::number(res.second.first);
    }
    return QString::number(res.second.first) + " " +
           QString::number(res.second.second);
  } else {
    return {};
  }
}

void Game::next() {
  int frame, player, ball, pins;
  if (!impl_->input.nextStep(frame, player, ball, pins)) {
    emit gameOver();
    return;
  }

  if (frame != impl_->expectedFrame || player != impl_->expectedPlayer ||
      ball != impl_->expectedBall || pins > 10) {
    emit rulesBroken();
    return;
  }

  emit knockedDown(player, pins);

  if (ball == 1) {
    if (pins == 10) {
      // This is strike
      impl_->results[player - 1].push_back(
          std::make_pair(Strike, std::make_pair(pins, 0)));
      if (frame != 10) {
        checkIfLastPlayer(player);
      } else {
        // Tenth frame has especial rules
        impl_->expectedBall++;
      }

    } else {
      impl_->results[player - 1].push_back(
          std::make_pair(NothingEspecial, std::make_pair(pins, 0)));
      impl_->currentPins -= pins;
      impl_->expectedBall++;
    }
  } else {
    if (pins > impl_->currentPins) {
      // player couldnt knock more pins then exists
      emit rulesBroken();
      return;
    }
    if (frame != 10) {
      if (pins == impl_->currentPins) {
        // This is spare
        impl_->results[player - 1].back().first = Spare;
      }

      impl_->results[player - 1].back().second.second += pins;
      impl_->currentPins = 10;
      // if there is second ball, next ball has to be first of the second player
      impl_->expectedBall = 1;
      checkIfLastPlayer(player);
    }

    if (frame == 10 && impl_->results[player - 1].size() == 10) {
      // If strike were in tenth frame
      if (impl_->results[player - 1].back().first == Strike) {
        if (ball == 2) {
          impl_->results[player - 1].back().second.second += pins;
          impl_->expectedBall++;
          impl_->currentPins = 10;
        } else if (ball == 3) {
          impl_->results[player - 1].back().second.second += pins;
          impl_->expectedBall = 1;
          impl_->currentPins = 10;
          checkIfLastPlayer(player);
        }
      }
      // If spare were in tenth frame
      else if (impl_->results[player - 1].back().first == Spare) {
        impl_->results[player - 1].back().second.second += pins;
        impl_->expectedBall = 1;
        impl_->currentPins = 10;
        checkIfLastPlayer(player);
      } else {
        if (pins == impl_->currentPins) {
          // This is spare
          impl_->results[player - 1].back().first = Spare;
          impl_->expectedBall++;
          impl_->results[player - 1].back().second.second += pins;
          impl_->currentPins = 10;
        } else {
          impl_->results[player - 1].back().second.second += pins;
          impl_->currentPins = 10;
          // if there is second ball, next ball has to be first of the second
          // player
          impl_->expectedBall = 1;
          checkIfLastPlayer(player);
        }
      }
    }
  }
  countScore();
}

int Game::score(int player) const {
  if (player > impl_->scores.size()) {
    return 0;
  }
  return impl_->scores[player];
}

void Game::setInput(QString input) {
  input.remove("file:///");
  Q_ASSERT(impl_->input.setInput(input.toStdString()));
  for (size_t i = 0; i < impl_->input.playersNum(); i++) {
    impl_->results.emplace_back(
        std::vector<std::pair<Knock, std::pair<int, int>>>{});
    impl_->scores.push_back(0);
  }
  emit playersNumberChanged();
}

void Game::checkIfLastPlayer(int player) {
  if (player == impl_->input.playersNum()) {
    if (impl_->expectedFrame == 10) {
      countScore();
      emit gameOver();
      return;
    }
    impl_->expectedPlayer = 1;
    impl_->expectedFrame++;
    emit currentFrameChanged();
    emit currentPlayerChanged();
  } else {
    impl_->expectedPlayer++;
    emit currentPlayerChanged();
  }
}

void Game::countScore() {
  for (size_t i = 0; i < impl_->results.size(); i++) {
    impl_->scores[i] = 0;
    for (size_t j = 0; j < impl_->results[i].size(); j++) {
      if (impl_->results[i][j].first == Strike) {
        impl_->scores[i] += impl_->results[i][j].second.first;
        if (j + 1 < impl_->results[i].size()) {
          if (impl_->results[i][j + 1].first != Strike) {
            impl_->scores[i] += impl_->results[i][j + 1].second.first;
            impl_->scores[i] += impl_->results[i][j + 1].second.second;
          } else {
            impl_->scores[i] += impl_->results[i][j + 1].second.first;
            if (j + 2 < impl_->results[i].size()) {
              impl_->scores[i] += impl_->results[i][j + 2].second.first;
            }
          }
        }
      } else if (impl_->results[i][j].first == Spare) {
        impl_->scores[i] += impl_->results[i][j].second.first;
        impl_->scores[i] += impl_->results[i][j].second.second;
        if (j + 1 < impl_->results[i].size()) {
          impl_->scores[i] += impl_->results[i][j + 1].second.first;
        }
      } else {
        impl_->scores[i] += impl_->results[i][j].second.first;
        impl_->scores[i] += impl_->results[i][j].second.second;
      }
    }
  }
  emit scoreChanged();
}

}  // namespace bowling
