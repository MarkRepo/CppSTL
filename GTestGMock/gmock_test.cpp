#include "gmock/gmock.h"
#include "turtle.h"
#include "gtest/gtest.h"

using namespace test;
class MockTurtle : public Turtle {
public:
  MOCK_METHOD(void, PenUp, (), (override));
  MOCK_METHOD(void , PenDown, (), (override));
  MOCK_METHOD(void, Forward, (int distance), (override));
  MOCK_METHOD(void, Turn, (int degrees), (override));
  MOCK_METHOD(void, Goto, (int x, int y), (override));
  MOCK_METHOD(int, GetX, (), (const, override));
  MOCK_METHOD(int, GetY, (), (const, override));
} ;

class Painter{
public:
  Painter(Turtle* turtle) {
    turtle_ = turtle;
  }
  bool DrawCircle(int,int, int) {
    //turtle_->PenDown();
    return true;
  }
private:
  Turtle* turtle_;
};

using ::testing::AtLeast;

TEST(PainterTest, CanDrawSomething) {
  MockTurtle turtle;                              // #2
  EXPECT_CALL(turtle, PenDown())                  // #3
      .Times(AtLeast(1));

  Painter painter(&turtle);                       // #4

  EXPECT_TRUE(painter.DrawCircle(0, 0, 10));      // #5
}