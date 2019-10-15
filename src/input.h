#ifndef INPUT_H
#define INPUT_H

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

namespace bowling {

/**
 * @brief The Input class read game process
 * Expected format:
 * first line players number
 * next lines: <frame_#> <player_#> <ball_#> <pins><'/n'>
 */
class Input {
 public:
  Input();
  ~Input();

  const fs::path& input() const;
  /**
   * @brief setInput - set input file
   * @param input - input file
   * @return true if success, false otherwise(file not exists or parse error)
   */
  bool setInput(const fs::path& input);
  /**
   * @brief playersNum
   * @return number of players, 0 if no input set
   */
  size_t playersNum() const;
  /**
   * @brief nextStep - read next step from input
   * @param player - player number
   * @param frame - frame number
   * @param ball - ball number
   * @param pins - pins knocked down
   * @return true if succes, false otherwise(eof, parsing error)
   */
  bool nextStep(int& frame, int& player, int& ball, int& pins);

 private:
  Input(const Input&) = delete;
  Input(const Input&&) = delete;
  Input& operator=(const Input&) = delete;
  Input& operator=(const Input&&) = delete;

  struct Implementation;
  std::unique_ptr<Implementation> impl_;
};

}  // namespace bowling

#endif  // INPUT_H
