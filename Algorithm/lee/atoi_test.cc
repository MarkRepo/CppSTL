#include <gtest/gtest.h>

namespace test {

int atoi(char* str) {
  if (!str) return 0;
  char* p = str;
  int ret = 0, s = 1;
  if (*p == '+' || *p == '-' || (*p >= '0' && *p <= '9')) {
    if (*p == '-') {
      p++;
      s = -1;
    }
    if (*p == '+') p++;
    while (*p >= '0' && *p <= '9') {
      ret = (ret * 10) + (*p - '0');
      p++;
    }
    ret = s * ret;
    return ret;
  } else {
    return 0;
  }
}

struct TreeNode {
  int value;
  struct TreeNode *left, *right;
};

int haspath(struct TreeNode* root, int value) {
  if (root->left == NULL && root->right == NULL) {
    if (root->value == value)
      return 1;
    else
      return 0;
  }
  int left_has = 0, right_has = 0;
  if (root->left) left_has = haspath(root->left, value - root->value);
  if (root->right) right_has = haspath(root->right, value - root->value);
  if (left_has == 1 || right_has == 1)
    return 1;
  else
    return 0;
}

}  // namespace test

TEST(atoiTest, test1) {
  ASSERT_EQ(test::atoi("1234"), 1234);
  ASSERT_EQ(test::atoi("-3425"), -3425);
  ASSERT_EQ(test::atoi("abcd"), 0);
}
