#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

vector<vector<int>> result;

void traceBack(vector<int>& nums, vector<int>& track, set<int>& strack) {
  if (track.size() == nums.size()) {
    result.push_back(track);
    return;
  }
  for (auto n : nums) {
    if (strack.count(n) != 0) {
      continue;
    }

    track.push_back(n);
    strack.insert(n);
    traceBack(nums, track, strack);
    strack.erase(n);
    track.pop_back();
  }
}

vector<vector<int>> permute(vector<int> nums) {
  vector<int> track;
  set<int> strack;
  traceBack(nums, track, strack);
  return result;
}

vector<vector<string>> solves;

bool isValid(vector<string>& board, int low, int column) {
  // 判断0~low上每一列是否冲突
  for (auto i = 0; i < low; i++) {
    if (board[i][column] == 'Q') {
      return false;
    }
  }

  // 判断0~low上右上方是否冲突
  for (auto i = low - 1, j = column + 1; i >= 0 && j < board[0].size(); i--, j++) {
    if (board[i][j] == 'Q') {
      return false;
    }
  }

  // 判断0~low左上方是否冲突
  for (auto i = low - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
    if (board[i][j] == 'Q') {
      return false;
    }
  }

  return true;
}

void trace(vector<string>& board, int low) {
  if (low == board.size()) {
    solves.push_back(board);
    return;
  }
  for (auto i = 0; i < board[0].size(); i++) {
    if (!isValid(board, low, i)) {
      continue;
    }

    board[low][i] = 'Q';
    trace(board, low + 1);
    board[low][i] = '.';
  }
}

void quene(int n) {
  vector<string> board(n, string(n, '.'));
  trace(board, 0);
}

// 凑零钱问题
// 1. 明确状态，amount
// 2. 定义dp函数，dp(n) 表示 筹到n最小需要dp(n)枚硬币
// 3. 确定选择，并择优, 选择一个coin放进来
// 4. 状态转移：for (coin: coins) dp(n) = min(dp(n), 1+dp(n-coin))
int minCoin(vector<int> coins, int amount) {
  vector<int> dp(amount + 1, amount + 1);
  dp[0] = 0;
  for (auto i = 1; i < amount + 1; i++) {
    for (auto coin : coins) {
      if (i - coin < 0) continue;
      dp[i] = std::min(dp[i], 1 + dp[i - coin]);
    }
  }
  return dp[amount];
}

TEST(Algo, permute) {
  vector<int> nums{1, 2, 3};
  auto p = permute(nums);
  cout << "len: " << p.size() << endl;
  for (auto& v : p) {
    for (auto vv : v) {
      cout << vv << " ";
    }
    cout << endl;
  }
}

TEST(Algo, quene) {
  solves.clear();
  quene(8);
  for (auto& v : solves) {
    cout << "-------------------" << endl;
    for (auto& s : v) {
      for (auto c : s) {
        cout << c << " ";
      }
      cout << endl;
    }
  }
}

TEST(Algo, minCoins) {
  vector<int> coins{2, 3, 5, 7, 9};
  cout << minCoin(coins, 100) << endl;
}

//给你一个可装载重量为`W`的背包和`N`个物品，每个物品有重量和价值两个属性。其中第`i`个物品的重量为`wt[i]`，价值为`val[i]`，现在让你用这个背包装物品，最多能装的价值是多少？（**这个题目中的物品不可以分割，要么装进包里，要么不装，不能说切成两块装一半。**这也许就是
// 0-1 背包这个名词的来历。）

// 1. 状态 背包容量W，可选择的物品
// dp[N][W]: 前N个物品，容量w，最大值为dp[n][w]
// 选择： 装或者不装
// 状态转移： dp[n][w] = max(dp[n-1][w], dp[n-1][w-w[n]] + val[n])

int knapsack(int W, int N, vector<int>& wt, vector<int>& val) {
  vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));
  for (int i = 1; i <= N; i++) {
    for (int w = 1; w <= W; w++) {
      if (w - wt[i - 1] < 0) {
        dp[i][w] = dp[i - 1][w];
      } else {
        // i 有偏移1位
        dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - wt[i - 1]] + val[i - 1]);
      }
    }
  }
  return dp[N][W];
}

TEST(Algo, knapsack) {
  vector<int> wt{10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  vector<int> val{30, 20, 10, 15, 25, 45, 33, 43, 22, 18};
  cout << knapsack(100, 10, wt, val) << endl;
}

// 子集背包问题
bool canPartition(vector<int> nums) {
  int sum = 0;
  for (auto n : nums) {
    sum += n;
  }
  if (sum % 2 != 0) {
    return false;
  }
  int len = nums.size();
  vector<vector<bool>> dp(len + 1, vector<bool>(sum / 2 + 1, false));
  for (auto i = 0; i <= nums.size(); i++) {
    dp[i][0] = true;
  }

  for (auto i = 1; i <= nums.size(); i++) {
    for (auto j = 1; j <= sum / 2; j++) {
      if (j - nums[i - 1] < 0)
        dp[i][j] = dp[i - 1][j];
      else
        dp[i][j] = dp[i - 1][j] || dp[i - 1][j - nums[i - 1]];
    }
  }
  return dp[len][sum / 2];
}

TEST(Algo, canPartition) {
  vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  cout << canPartition(nums) << endl;
}

// LIS 最长递增子序列
// 状态：下标
// dp[i]: 以array[i]结尾的lis长度
// 状态转移：if(nums[j] < nums[i]) dp[i] = max(dp[i], dp[j]+1)
int lis(vector<int> nums) {
  vector<int> dp(nums.size(), 1);
  for (int i = 0; i < nums.size(); i++) {
    for (int j = 0; j < i; j++) {
      if (nums[j] < nums[i]) {
        dp[i] = max(dp[i], dp[j] + 1);
      }
    }
  }
  int res = dp[0];
  for (int i = 0; i < dp.size(); i++) {
    if (dp[i] > res) {
      res = dp[i];
    }
  }
  return res;
}

TEST(Algo, lis) {
  vector<int> nums{5, 7, 9, 2, 4, 3, 1, 5, 6, 8};
  cout << "lis: " << lis(nums) << endl;
}

// 最长回文子序列
// dp[i][j]  表示 s[i..j]的 lpss
int lpss(string s) {
  vector<vector<int>> dp(s.length(), vector<int>(s.length(), 0));
  for (int i = 0; i < s.length(); i++) {
    dp[i][i] = 1;
  }
  for (int i = s.length() - 1; i >= 0; i--) {
    for (int j = i + 1; j < s.length(); j++) {
      if (s[i] == s[j]) {
        dp[i][j] = dp[i + 1][j - 1] + 2;
      } else {
        dp[i][j] = max(dp[i][j - 1], dp[i + 1][j]);
      }
    }
  }
  return dp[0][s.length() - 1];
}

TEST(Algo, lpss) {
  string s = "abbbddegdgdgehgkasdhgawkheihgdkghkdghdf";
  cout << lpss(s) << endl;
}

// 最长公共子序列
// dp[i][j] 表示 s1[0...i-1] 和 s2[0...j-1] 的 lcs
int lcs(string s1, string s2) {
  vector<vector<int>> dp(s1.length() + 1, vector<int>(s2.length() + 1, 0));
  for (int i = 1; i <= s1.length(); i++) {
    for (int j = 1; j <= s2.length(); j++) {
      if (s1[i - 1] == s2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  return dp[s1.length()][s2.length()];
}

TEST(Algo, lcs) {
  string s1 = "abbbeddggggddeggg";
  string s2 = "bbbdegggggdddeggg";
  cout << "lcs:  " << lcs(s1, s2) << endl;
}

// minDistance 最短编辑距离
// dp[i][j] 表示 s1[0...i-1] 与s2[0...j-1] 的minDistance
int minDistance(string s1, string s2) {
  vector<vector<int>> dp(s1.length() + 1, vector<int>(s2.length() + 1, 0));
  for (int i = 0; i <= s1.length(); i++) {
    dp[i][0] = i;
  }
  for (int j = 0; j <= s2.length(); j++) {
    dp[0][j] = j;
  }

  for (int i = 1; i <= s1.length(); i++) {
    for (int j = 1; j <= s2.length(); j++) {
      if (s1[i - 1] == s2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = min(
            //插入
            dp[i][j - 1] + 1,
            min(
                //删除
                dp[i - 1][j] + 1,
                //替换
                dp[i - 1][j - 1] + 1));
      }
    }
  }
  return dp[s1.length()][s2.length()];
}

TEST(Algo, minDistance) {
  string s1 = "kfsdhfasdhf";
  string s2 = "kfjiewioetyrfdkfhd";
  cout << "minDistance: " << minDistance(s1, s2) << endl;
}

// 最大子数组和
// dp[i] 表示以 array[i] 结尾的最大子数组和为 dp[i]
int maxSubArray(vector<int> nums) {
  vector<int> dp(nums.size(), 0);
  dp[0] = nums[0];
  for (int i = 1; i < nums.size(); i++) {
    // 要么接上以i-1结尾的子数组，要么自成一派
    dp[i] = max(nums[i] + dp[i - 1], nums[i]);
  }
  int res = dp[0], index = 0;
  for (int i = 0; i < nums.size(); i++) {
    if (dp[i] > res) {
      res = dp[i];
      index = i;
    }
  }
  cout << "maxSubArray index: " << index << endl;
  return res;
}

TEST(Algo, maxSubArray) {
  vector<int> nums{1, 2, 3, -4, 6, -2, 9, -3, -7};
  cout << "maxSubArray: " << maxSubArray(nums) << endl;
}

int binarySearch(vector<int> nums, int target) {
  int left = 0, right = nums.size() - 1;
  while (left <= right) {
    auto mid = left + (right - left) / 2;
    if (nums[mid] == target) {
      return mid;
    } else if (target < nums[mid]) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return -1;
}

int lowerBound(vector<int> nums, int target) {
  int left = 0, right = nums.size() - 1;
  while (left <= right) {
    auto mid = left + (right - left) / 2;
    if (target == nums[mid]) {
      // 收缩右侧边界
      right = mid - 1;
    } else if (target < nums[mid]) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return (left >= nums.size() || nums[left] != target) ? -1 : left;
}

int upperBound(vector<int> nums, int target) {
  int left = 0, right = nums.size() - 1;
  while (left <= right) {
    auto mid = left + (right - left) / 2;
    if (target == nums[mid]) {
      // 收缩左侧边界
      left = mid + 1;
    } else if (target < nums[mid]) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return (right < 0 || nums[right] != target) ? -1 : right;
}

TEST(Algo, binarySearch) {
  vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9};
  cout << "binary search 2: " << binarySearch(nums, 2) << endl;
  vector<int> nums2{1, 2, 2, 2, 2, 3, 4, 5, 6};
  cout << "lower bound 2: " << lowerBound(nums2, 2) << endl;
  vector<int> nums3{1, 2, 3, 4, 5, 6};
  cout << "lower bound 7: " << lowerBound(nums3, 7) << endl;
  cout << "lower bound 0: " << lowerBound(nums3, 0) << endl;
  cout << "upper bound 2: " << upperBound(nums2, 2) << endl;
  cout << "upper bound 7: " << upperBound(nums3, 7) << endl;
  cout << "upper bound 0: " << upperBound(nums3, 0) << endl;
}

// LCA  最近公共祖
struct Node {
  Node *left, *right;
  char c;
  Node(char c) : c(c) {
    this->left = NULL;
    this->right = NULL;
  }
};

Node* LCA(Node* root, Node* p, Node* q) {
  // base case
  if (root == NULL) return NULL;
  if (root == p || root == q) return root;
  Node* left = LCA(root->left, p, q);
  Node* right = LCA(root->right, p, q);
  if (left && right) {
    return root;
  }
  if (!left && !right) {
    return NULL;
  }
  return left ? left : right;
}

Node* build() {
  Node* a = new Node('a');
  a->left = new Node('b');
  a->right = new Node('c');
  a->left->left = new Node('d');
  a->left->right = new Node('e');
  a->right->left = new Node('f');
  a->right->right = new Node('g');
  return a;
}

TEST(Algo, LCA) {
  Node* root = build();
  cout << "d and e lca should be b: " << LCA(root, root->left->left, root->left->right)->c << endl;
  cout << "e and f lca should be a: " << LCA(root, root->left->right, root->right->left)->c << endl;
}

// getRandom 水塘随机抽样算法

int getRandom(Node* root) { return 0; }

TEST(Algo, getRandom) {}

// aplusb
int aplusb(int a, int b) {
  auto sum = a ^ b;
  auto carry = (a & b) << 1;
  if (carry == 0) return sum;
  return aplusb(sum, carry);
}

TEST(Algo, aplusb) {
  cout << "999 + 888 = " << aplusb(999, 888) << endl;
  cout << "-111 + 666 = " << aplusb(-111, 666) << endl;
}

int getMax(vector<int> piles) {
  int max = -1;
  for (auto pile : piles) {
    if (pile > max) {
      max = pile;
    }
  }
  return max;
}

int timeOf(int pile, int speed) { return pile / speed + (pile % speed == 0 ? 0 : 1); }

bool canFinish(vector<int>& piles, int speed, int H) {
  auto times = 0;
  for (auto pile : piles) {
    times += timeOf(pile, speed);
  }
  if (times <= H) return true;
  return false;
}

// piles N 堆香蕉
int minEatingSpeed(vector<int> piles, int H) {
  int min = 1, max = getMax(piles) + 1;
  while (min < max) {
    auto mid = min + (max - min) / 2;
    if (canFinish(piles, mid, H)) {
      max = mid;
    } else {
      min = mid + 1;
    }
  }
  return min;
}

bool canComplete(vector<int>& weight, int D, int w) {
  int i = 0;
  for (int d = 0; d < D; d++) {
    int max = w;
    while (max - weight[i] >= 0) {
      i++;
      if (i == weight.size()) return true;
    }
  }
  return false;
}

int minShipWeight(vector<int>& weight, int D) {
  int min = getMax(weight);
  int sum = 0;
  for (auto w : weight) {
    sum += w;
  }
  int max = sum + 1;
  while (min < max) {
    auto mid = min + (max - min) / 2;
    if (canComplete(weight, D, min))
      max = mid;
    else
      min = mid + 1;
  }
  return min;
}

int rank = 0;
int getRanK(Node* root, int k) {
  if (root == NULL) {
    return;
  }
  getRank(root->left, k);
  rank++;
  if (rank == k) return root->c;
  getRank(root->right, k);
}

bool isValidBSTHelper(Node* root, Node* min, Node* max) {
  if (!root) return true;
  if (min && root->c <= min->) return false;
  if (max && root->c >= min->c) return false;
  return isValidBSTHelper(root->left, min, root) && isValidBSTHelper(root->right, root, max);
}

bool isValidBST(Node* root) { return isValidBSTHelper(root, NULL, NULL); }

Node* insertToBST(Node* root, char val) {
  if (root == NULL) return new Node(val);
  if (val < root->val) {
    root->left = insertToBST(root->left, val);
  }
  if (val > root->val) {
    root->right = insertToBST(root->right, val);
  }

  return root;
}

// 图搜索最短距离
int BFS(Node* root, Node* target) {
  queue<Node*> q;
  set<Node*> visited;
  q.push(root);
  visited.insert(root);
  int step = 0;
  while(!q.empty()) {
    int n = q.size();
    for(int i = 0;i < n; i++) {
      auto cur = q.top();
      q.pop();
      if (root->c == target->c) {
        return step;
      }
      if (cur->left && visited.count(cur->left) == 0) {
        q.push(cur->left);
      }
      if (cur_.right && visited.count(cur->right) == 0) {
        q.push(cur->right);
      }
    }
    step++;
  }
}

// 滑动窗口
void slidingWindow(string s, string t) {
  int len = s.length();
  int left = 0, right = 0;
  while (right < len) {
    auto c = s[right];
    right++;
    // 更新窗口，计算结果
    // ...
    while(need shrink) {
      // 更新条件
      // ...
      auto d = s[left];
      left++;

      // 跟你像你窗口，计算结果
    }
  }
}

// 回溯
result = []
def backtrack(路径，选择列表)
  if (满足条件)
    result.add(路径)
    return

  for 选择 ： 选择列表
    选择
    backtrack（路径‘， 选择列表’）
    撤销选择