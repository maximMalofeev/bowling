#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <memory>

namespace bowling {

class Game : public QObject {
  Q_OBJECT
  Q_PROPERTY(int playersNumber READ playersNumber NOTIFY playersNumberChanged)

 public:
  explicit Game(QObject *parent = nullptr);
  ~Game();

  int playersNumber() const;
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
  /**
   * @brief gameOver - emit if game over or timeout(eof)
   */
  void gameOver();
  /**
   * @brief rulesBroken - emit if got unexpected values
   */
  void rulesBroken();

  void stepMade(int frame, int player, int ball, int pins);

  void scoreChanged();

 private:
  struct Implementation;
  std::unique_ptr<Implementation> impl_;
};

}  // namespace bowling

#endif  // GAME_H
