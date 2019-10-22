#ifndef GAMEBACKEND_H
#define GAMEBACKEND_H

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

namespace bowling{

/**
 * @brief The GameBackend class contains all game logic
 */
class GameBackend final
{
 public:
  /**
   * @brief The State enum describes current game state
   */
  enum class State{NotStarted, Started, GameOver, RulesBroken};
  /**
   * @brief The FrameResult enum describes frame result
   */
  enum class FrameResult{NothingEspecial, Strike, Spare};

  GameBackend();
  ~GameBackend();

  /**
   * @brief state
   * @return current game state
   */
  State state() const;
  /**
   * @brief start game
   * @param input - file contains input data
   * @return true if succes, false otherwise
   */
  bool start(const fs::path& input);
  /**
   * @brief players count
   * @return players count or -1 if game not started
   */
  int players() const;
  /**
   * @brief frame
   * @return current frame or -1 if game not started
   */
  int frame() const;
  /**
   * @brief player
   * @return current player or -1 if game not started
   */
  int player() const;
  /**
   * @brief ball
   * @return current ball or -1 if game not started
   */
  int ball() const;
  /**
   * @brief pins
   * @return knocked down pins or -1 if game not started
   */
  int pins() const;
  /**
   * @brief score
   * @param player - player num
   * @return current score of the player
   */
  int score(int player) const;

  /**
   * @brief makeStep - do next player step
   * @return true if success, false otherwise
   */
  bool makeStep();

 private:
  struct Implementation;
  std::unique_ptr<Implementation> impl_;
};

}

#endif // GAMEBACKEND_H
