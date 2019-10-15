#include <input.h>
#include <QtTest>
#include <fstream>

class TestInput : public QObject {
  Q_OBJECT

 private slots:
  void initTestCase();
  void test_defaultInput();
  void test_setUnexistingInput();
  void test_setInput();
  void test_nextStep();
  void cleanupTestCase();

 private:
  const size_t playersNum_ = 1;
  bowling::Input* input_{};
  fs::path path_{"file.txt"};
  std::fstream file_;
};

void TestInput::initTestCase() {
  input_ = new bowling::Input;
  file_.open(path_, std::fstream::out);
  QVERIFY(file_);
  file_ << playersNum_ << std::endl;
  file_ << 1 << " " << 1 << " " << 1 << " " << 1 << std::endl;
  file_.close();
}

void TestInput::test_defaultInput() { QCOMPARE(input_->input(), fs::path{}); }

void TestInput::test_setUnexistingInput() {
  fs::path path{"c:/some/unexisting/path/file.txt"};
  QCOMPARE(input_->setInput(path), false);
  QCOMPARE(input_->input(), fs::path{});
}

void TestInput::test_setInput() {
  QVERIFY(input_->setInput(path_));
  QCOMPARE(input_->input(), path_);
  QCOMPARE(input_->playersNum(), playersNum_);
}

void TestInput::test_nextStep() {
  int player, frame, ball, pins;
  QVERIFY(input_->nextStep(frame, player, ball, pins));
  QCOMPARE(player, 1);
  QCOMPARE(frame, 1);
  QCOMPARE(ball, 1);
  QCOMPARE(pins, 1);
  QVERIFY(!input_->nextStep(frame, player, ball, pins));
}

void TestInput::cleanupTestCase() {
  delete input_;
  fs::remove(path_);
}

QTEST_APPLESS_MAIN(TestInput)

#include "tst_input.moc"
