#define CATCH_CONFIG_MAIN
#include <game_backend.h>
#include <fstream>
#include <memory>
#include "catch.hpp"

TEST_CASE("Test not started state", "[NotStartedState]") {
  bowling::GameBackend gb;
  REQUIRE(gb.state() == bowling::GameBackend::State::NotStarted);
  REQUIRE(gb.players() == -1);
  REQUIRE(gb.frame() == -1);
  REQUIRE(gb.player() == -1);
  REQUIRE(gb.ball() == -1);
  REQUIRE(gb.pins() == -1);
}

TEST_CASE("Test start not existing file", "[StartNotExistingFile]") {
  bowling::GameBackend gb;
  REQUIRE_FALSE(gb.start("c:/not/existing/file"));
  REQUIRE(gb.state() == bowling::GameBackend::State::NotStarted);
  REQUIRE(gb.players() == -1);
  REQUIRE(gb.frame() == -1);
  REQUIRE(gb.player() == -1);
  REQUIRE(gb.ball() == -1);
  REQUIRE(gb.pins() == -1);
}

TEST_CASE("Test one player", "[OnePlayer]") {
  std::filesystem::path inputPath = "input.txt";
  std::unique_ptr<std::fstream, std::function<void(std::fstream*)>> input(
      new std::fstream(inputPath, std::fstream::out),
      [inputPath](std::fstream* f) {
        if (f->is_open()) {
          f->close();
        }
        std::filesystem::remove(inputPath);
        delete f;
      });
  REQUIRE(input->is_open());
  *input << "1" << std::endl;
  // first frame nothing especial
  *input << "1" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "1" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  // second frame strike
  *input << "2" << " " << "1" << " " << "1" << " " << "10" << std::endl;
  // third frame spare
  *input << "3" << " " << "1" << " " << "1" << " " << "1" << std::endl;
  *input << "3" << " " << "1" << " " << "2" << " " << "9" << std::endl;
  // fourth frame
  *input << "4" << " " << "1" << " " << "1" << " " << "1" << std::endl;
  *input << "4" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  // fifth frame no pins knocked down at all
  *input << "5" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "5" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  // all subsequent frames brings one pin
  *input << "6" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "6" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "7" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "7" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "8" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "8" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "9" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "9" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "10" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "10" << " " << "1" << " " << "2" << " " << "1" << std::endl;

  input->close();

  bowling::GameBackend gb;
  REQUIRE(gb.start(inputPath));
  REQUIRE(gb.state() == bowling::GameBackend::State::Started);
  REQUIRE(gb.players() == 1);

  // first frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 1);

  // second frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 11);

  // third frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 13);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 31);

  // fourth frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 33);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);

  // 5
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);

  // 6
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 35);
  // 7
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 35);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 36);
  // 8
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 36);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 37);
  // 9
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 37);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 38);
  // 10
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 38);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 39);

  REQUIRE_FALSE(gb.makeStep());
}

TEST_CASE("Test one player last strike", "[OnePlayerLastStrike]") {
  std::filesystem::path inputPath = "input.txt";
  std::unique_ptr<std::fstream, std::function<void(std::fstream*)>> input(
      new std::fstream(inputPath, std::fstream::out),
      [inputPath](std::fstream* f) {
        if (f->is_open()) {
          f->close();
        }
        std::filesystem::remove(inputPath);
        delete f;
      });
  REQUIRE(input->is_open());
  *input << "1" << std::endl;
  *input << "1" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "1" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "2" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "2" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "3" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "3" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "4" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "4" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "5" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "5" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "6" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "6" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "7" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "7" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "8" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "8" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "9" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "9" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "10" << " " << "1" << " " << "1" << " " << "10" << std::endl;
  *input << "10" << " " << "1" << " " << "2" << " " << "10" << std::endl;
  *input << "10" << " " << "1" << " " << "3" << " " << "10" << std::endl;

  input->close();

  bowling::GameBackend gb;
  REQUIRE(gb.start(inputPath));
  REQUIRE(gb.state() == bowling::GameBackend::State::Started);
  REQUIRE(gb.players() == 1);

  // first frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);

  // second frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);

  // third frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);

  // fourth frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 5
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 6
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 7
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 8
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 9
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 10
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 10);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 20);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 30);

  REQUIRE_FALSE(gb.makeStep());
}

TEST_CASE("Test one player last spare", "[OnePlayerLastSpare]") {
  std::filesystem::path inputPath = "input.txt";
  std::unique_ptr<std::fstream, std::function<void(std::fstream*)>> input(
      new std::fstream(inputPath, std::fstream::out),
      [inputPath](std::fstream* f) {
        if (f->is_open()) {
          f->close();
        }
        std::filesystem::remove(inputPath);
        delete f;
      });
  REQUIRE(input->is_open());
  *input << "1" << std::endl;
  *input << "1" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "1" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "2" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "2" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "3" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "3" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "4" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "4" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "5" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "5" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "6" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "6" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "7" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "7" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "8" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "8" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "9" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "9" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "10" << " " << "1" << " " << "1" << " " << "1" << std::endl;
  *input << "10" << " " << "1" << " " << "2" << " " << "9" << std::endl;
  *input << "10" << " " << "1" << " " << "3" << " " << "10" << std::endl;

  input->close();

  bowling::GameBackend gb;
  REQUIRE(gb.start(inputPath));
  REQUIRE(gb.state() == bowling::GameBackend::State::Started);
  REQUIRE(gb.players() == 1);

  // first frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);

  // second frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);

  // third frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);

  // fourth frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 5
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 6
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 7
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 8
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 9
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  // 10
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 1);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 10);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 20);

  REQUIRE_FALSE(gb.makeStep());
}

TEST_CASE("Test two players", "[TwoPlayers]") {
  std::filesystem::path inputPath = "input.txt";
  std::unique_ptr<std::fstream, std::function<void(std::fstream*)>> input(
      new std::fstream(inputPath, std::fstream::out),
      [inputPath](std::fstream* f) {
        if (f->is_open()) {
          f->close();
        }
        std::filesystem::remove(inputPath);
        delete f;
      });
  REQUIRE(input->is_open());
  *input << "2" << std::endl;
  // first frame nothing especial
  *input << "1" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "1" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "1" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "1" << " " << "2" << " " << "2" << " " << "1" << std::endl;
  // second frame strike
  *input << "2" << " " << "1" << " " << "1" << " " << "10" << std::endl;
  *input << "2" << " " << "2" << " " << "1" << " " << "10" << std::endl;
  // third frame spare
  *input << "3" << " " << "1" << " " << "1" << " " << "1" << std::endl;
  *input << "3" << " " << "1" << " " << "2" << " " << "9" << std::endl;
  *input << "3" << " " << "2" << " " << "1" << " " << "1" << std::endl;
  *input << "3" << " " << "2" << " " << "2" << " " << "9" << std::endl;
  // fourth frame
  *input << "4" << " " << "1" << " " << "1" << " " << "1" << std::endl;
  *input << "4" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "4" << " " << "2" << " " << "1" << " " << "1" << std::endl;
  *input << "4" << " " << "2" << " " << "2" << " " << "1" << std::endl;
  // fifth frame no pins knocked down at all
  *input << "5" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "5" << " " << "1" << " " << "2" << " " << "0" << std::endl;
  *input << "5" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "5" << " " << "2" << " " << "2" << " " << "0" << std::endl;
  // all subsequent frames brings one pin
  *input << "6" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "6" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "6" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "6" << " " << "2" << " " << "2" << " " << "1" << std::endl;
  *input << "7" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "7" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "7" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "7" << " " << "2" << " " << "2" << " " << "1" << std::endl;
  *input << "8" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "8" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "8" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "8" << " " << "2" << " " << "2" << " " << "1" << std::endl;
  *input << "9" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "9" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "9" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "9" << " " << "2" << " " << "2" << " " << "1" << std::endl;
  *input << "10" << " " << "1" << " " << "1" << " " << "0" << std::endl;
  *input << "10" << " " << "1" << " " << "2" << " " << "1" << std::endl;
  *input << "10" << " " << "2" << " " << "1" << " " << "0" << std::endl;
  *input << "10" << " " << "2" << " " << "2" << " " << "1" << std::endl;

  input->close();

  bowling::GameBackend gb;
  REQUIRE(gb.start(inputPath));
  REQUIRE(gb.state() == bowling::GameBackend::State::Started);
  REQUIRE(gb.players() == 2);

  // first frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 1);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 0);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 1);

  // second frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 11);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 11);

  // third frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 13);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 31);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 13);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 31);

  // fourth frame
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 33);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 33);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 34);

  // 5
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 34);

  // 6
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 35);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 34);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 35);
  // 7
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 35);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 36);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 35);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 36);
  // 8
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 36);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 37);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 36);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 37);
  // 9
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 37);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 38);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 37);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 38);
  // 10
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 38);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(1) == 39);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 38);
  REQUIRE(gb.makeStep());
  REQUIRE(gb.score(2) == 39);

  REQUIRE_FALSE(gb.makeStep());
}
