#include "game.h"
#include <vector>
#include "game_backend.h"

namespace bowling {

struct Game::Implementation {
  GameBackend gb;
};

Game::Game(QObject *parent) : QObject(parent) {
  impl_.reset(new Implementation);
}

Game::~Game() {}

int Game::playersNumber() const { return impl_->gb.players(); }

void Game::next() {
  if (impl_->gb.makeStep()) {
    emit stepMade(impl_->gb.frame(), impl_->gb.player(), impl_->gb.ball(),
                  impl_->gb.pins());
  } else {
    if (impl_->gb.state() == bowling::GameBackend::State::GameOver) {
      emit gameOver();
    } else {
      emit rulesBroken();
    }
  }
}

int Game::score(int player) const {
  if (player > impl_->gb.players()) {
    return 0;
  }
  return impl_->gb.score(player);
}

void Game::setInput(QString input) {
  input.remove("file:///");
  if (impl_->gb.start(input.toStdString())) {
    emit playersNumberChanged();
  }
}

}  // namespace bowling
