#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <memory>

namespace bowling {

class Game : public QObject {
  Q_OBJECT
  Q_PROPERTY(int playersNumber READ playersNumber NOTIFY playersNumberChanged)
  Q_PROPERTY(int currentFrame READ currentFrame NOTIFY currentFrameChanged)
  Q_PROPERTY(int currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)

 public:
  enum Knock{NothingEspecial, Spare, Strike};

  explicit Game(QObject *parent = nullptr);
  ~Game();

  int playersNumber() const;
  int currentFrame() const;
  int currentPlayer() const;
  /**
   * @brief string representation of result
   * @param frame - required frame
   * @param player - required player
  */
  Q_INVOKABLE QString result(int frame, int player) const;
  /**
   * @brief do next step
   */
  Q_INVOKABLE void next();
  /**
   * @brief score of the player
   * @param player
   * @return score of the player
   */
  Q_INVOKABLE int score(int player) const;

  Q_INVOKABLE void setInput(QString input);

 signals:
  void playersNumberChanged();
  void currentFrameChanged();
  void currentPlayerChanged();

  /**
   * @brief gameOver - emit if game over or timeout(eof)
   */
  void gameOver();
  /**
   * @brief rulesBroken - emit if got unexpected values
   */
  void rulesBroken();

  void scoreChanged();
  void knockedDown(int player,int pins);

 private:
  struct Implementation;
  std::unique_ptr<Implementation> impl_;

  void checkIfLastPlayer(int player);
  void countScore();
};

}  // namespace bowling

#endif  // GAME_H
