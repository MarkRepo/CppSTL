#include <memory>
using std::unique_ptr;
using std::shared_ptr;

class Widget {
 public:
  Widget();
  ~Widget();

  Widget(Widget&& rhs);
  Widget& operator=(Widget&& rhs);

  Widget(const Widget& rhs);
  Widget& operator=(const Widget& rhs);

 private:
  struct Impl;
  unique_ptr<Impl> pImpl;
};