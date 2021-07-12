#include "widget.h"

#include <string>
#include <vector>

#include "gadget.h"

struct Widget::Impl {
  std::string name;
  std::vector<double> data;
  Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

Widget::Widget(const Widget& rhs) : pImpl(std::make_unique<Impl>(*rhs.pImpl)) {}
Wdiget& Widget::operator=(const Widget& rhs) {
  *pImpl = *rhs.pImpl;
  return *this;
}
