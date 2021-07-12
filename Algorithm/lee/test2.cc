// 递归反转整个单链表
#include <gtest/gtest.h>

#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;

struct Node {
  Node* next;
  int val;
  Node(int v) : val(v) { this->next = NULL; }
};

Node* reverse(Node* head) {
  if (!head->next) return head;
  Node* p = head;
  head = reverse(head->next);
  p->next->next = p;
  p->next = NULL;
  return head;
}

Node* build() {
  Node* head = new Node(0);
  Node* p = head;
  for (auto i = 1; i < 10; i++) {
    p->next = new Node(i);
    p = p->next;
  }

  return head;
}

TEST(Algo, revertSingleList) {
  Node* head = build();
  head = reverse(head);
  while (head != NULL) {
    cout << head->val << ", ";
    head = head->next;
  }
  cout << endl;
}

// 反转链表前N个节点
Node* hNext = NULL;
Node* reverseN(Node* head, int N) {
  if (N == 1) {
    hNext = head->next;
    return head;
  }
  Node* last = reverseN(head->next, N - 1);
  head->next->next = head;
  head->next = hNext;
  return last;
}

TEST(Algo, reverseN) {
  Node* head = build();
  head = reverseN(head, 5);
  while (head != NULL) {
    cout << head->val << ", ";
    head = head->next;
  }
}

// 反转中间m到n的节点
Node* reverseMN(Node* head, int M, int N) {
  if (M == 1) {
    return reverseN(head, N);
  }
  head->next = reverseMN(head->next, M - 1, N - 1);
  return head;
}

TEST(Algo, reverseMN) {
  Node* head = build();
  head = reverseMN(head, 4, 8);
  while (head != NULL) {
    cout << head->val << " ";
    head = head->next;
  }
  cout << endl;
}

Node* reverseK(Node* head, int K) {
  int k = K;
  Node* p = head;
  while (k > 0 && p != NULL) {
    p = p->next;
    k--;
  }
  if (k != 0) {
    return head;
  }
  Node* k1 = reverseN(head, K);
  Node* last = reverseK(p, K);
  head->next = last;
  return k1;
}

TEST(Algo, reverseK) {
  Node* head = build();
  head = reverseK(head, 4);
  while (head != NULL) {
    cout << head->val << " ";
    head = head->next;
  }
  cout << endl;
}

Node* pLeft = NULL;
bool traverse(Node* head) {
  if (head == NULL) return true;
  bool res = traverse(head->next);
  res = res && (head->val == pLeft->val);
  pLeft = pLeft->next;
  return res;
}

bool IsPalindrome(Node* head) {
  pLeft = head;
  return traverse(head);
}

Node* buildPalindrome() {
  Node* head = new Node(0);
  for (auto i = 1; i < 10; i++) {
    head->next = new Node(i);
  }
  for (auto i = 9; i >= 0; i--) {
    head->next = new Node(i);
  }
  return head;
}

TEST(Algo, palindrome) {
  Node* head1 = build();
  std::cout << "build() is palindrome?" << boolalpha << IsPalindrome(head1) << endl;
  Node* head = buildPalindrome();
  std::cout << "build1() is palindrome?" << boolalpha << IsPalindrome(head) << endl;
}

struct MapNode {
  int val;
  std::vector<MapNode*> adjs;
  MapNode(int v) : val(v) {}
  vector<MapNode*> adj() { return this->adjs; }
};

// BFS 寻找从start到target的最短距离
int BFS(MapNode* start, MapNode* target) {
  queue<MapNode*> q;
  set<MapNode*> visited;
  q.push(start);
  visited.insert(start);
  int step = 0;
  while (!q.empty()) {
    int n = q.size();
    for (auto i = 0; i < n; i++) {
      auto cur = q.front();
      q.pop();
      if (cur == target) {
        return step;
      }
      auto adjs = cur->adj();
      for (auto adj : adjs) {
        if (visited.find(adj) == visited.end()) {
          q.push(adj);
          visited.insert(adj);
        }
      }
    }
    step++;
  }
  // 没找到
  return -1;
}

TEST(Algo, BFS) {}

void mySwap(int* i, int* j) {
  auto tmp = *i;
  *i = *j;
  *j = tmp;
}

void bubble_sort(vector<int>& nums) {
  auto n = nums.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++)
      if (nums[j] > nums[j + 1]) mySwap(&nums[j], &nums[j + 1]);
  }
}

void select_sort(vector<int>& nums) {
  auto n = nums.size();
  for (int i = 0; i < n - 1; i++) {
    int max = nums[0];
    int index = 0;
    for (int j = 1; j < n - 1 - i; j++) {
      if (nums[j] > max) {
        max = nums[j];
        index = j;
      }
      if (j == n - 1 - i - 1) {
        mySwap(&nums[index], &nums[j]);
      }
    }
  }
}

void insert_sort(vector<int>& nums) {
  int n = nums.size();
  for (int i = 1; i < n; i++) {
    int key = nums[i];
    int j = i - 1;
    while (j >= 0 && nums[j] > key) {
      nums[j + 1] = nums[j];
      j--;
    }
    nums[j + 1] = key;
  }
}

int partition(vector<int>& nums, int low, int high) {
  int pivot = nums[low];
  while (low < high) {
    // 从后往前找一个小于pivot数放到low
    while (low < high && nums[high] >= pivot) {
      high--;
    }
    cout << "1 low: " << low << ", high: " << high << endl;
    nums[low] = nums[high];
    // 从前往后找一个大于pivot的数放到high位置
    while (low < high && nums[low] < pivot) {
      low++;
    }
    cout << "2 low: " << low << ", high: " << high << endl;
    nums[high] = nums[low];
  }
  nums[low] = pivot;
  return low;
}

void quickSort(vector<int>& nums, int low, int high) {
  if (low < high) {
    int pivot = partition(nums, low, high);
    quickSort(nums, low, pivot - 1);
    quickSort(nums, pivot + 1, high);
  }
}

void quickSort(vector<int>& nums) { quickSort(nums, 0, nums.size() - 1); }

// 堆排序
void max_heapify(vector<int>& arr, int start, int end) {
  int dad = start;
  int son = 2 * dad + 1;
  while (son < end) {
    // 找出较大的儿子
    if (son + 1 < end && arr[son] < arr[son + 1]) son++;
    if (arr[dad] > arr[son])
      return;
    else {
      mySwap(&arr[dad], &arr[son]);
      dad = son;
      son = 2 * dad + 1;
    }
  }
}

void head_sort(vector<int>& arr) {
  int n = arr.size();
  for (int i = n / 2 - 1; i >= 0; i--) {
    max_heapify(arr, i, n - 1);
  }
  for (int i = n - 1; i > 0; i--) {
    mySwap(&arr[0], &arr[i]);
    max_heapify(arr, 0, i - 1);
  }
}

void Merge(vector<int>& Array, int front, int mid, int end) {
  // preconditions:
  // Array[front...mid] is sorted
  // Array[mid+1 ... end] is sorted
  // Copy Array[front ... mid] to LeftSubArray
  // Copy Array[mid+1 ... end] to RightSubArray
  vector<int> LeftSubArray(Array.begin() + front, Array.begin() + mid + 1);
  vector<int> RightSubArray(Array.begin() + mid + 1, Array.begin() + end + 1);
  int idxLeft = 0, idxRight = 0;
  // 巧妙运用max(),简化后续的处理
  LeftSubArray.insert(LeftSubArray.end(), numeric_limits<int>::max());
  RightSubArray.insert(RightSubArray.end(), numeric_limits<int>::max());
  // Pick min of LeftSubArray[idxLeft] and RightSubArray[idxRight], and put into Array[i]
  for (int i = front; i <= end; i++) {
    if (LeftSubArray[idxLeft] < RightSubArray[idxRight]) {
      Array[i] = LeftSubArray[idxLeft];
      idxLeft++;
    } else {
      Array[i] = RightSubArray[idxRight];
      idxRight++;
    }
  }
}

void MergeSort(vector<int>& Array, int front, int end) {
  if (front >= end) return;
  int mid = (front + end) / 2;
  MergeSort(Array, front, mid);
  MergeSort(Array, mid + 1, end);
  Merge(Array, front, mid, end);
}

TEST(Algo, bubble_sort) {
  vector<int> nums{9, 3, 4, 56, 7, 1, 3, 5, 78};
  MergeSort(nums, 0, nums.size() - 1);
  for (auto n : nums) {
    cout << n << " ";
  }
  cout << endl;
}

// 合并n个有序数组
struct num {
  int val;
  int index;  // 第几个数组
  int k;      // 第几个元素
  num(int v, int i, int k) : val(v), index(i), k(k) {}
};

vector<int> mergeNK(vector<vector<int>> arr) {
  auto greater = [](num n1, num n2) { return n1.val > n2.val; };
  priority_queue<num, vector<num>, decltype(greater)> pq(greater);
  for (int i = 0; i < arr.size(); i++) {
    pq.push(num{arr[i][0], i, 0});
  }

  vector<int> result;
  while (!pq.empty()) {
    auto tmp = pq.top();
    pq.pop();
    // 如果该数组还有元素，则插入最小堆
    if (tmp.k < arr[tmp.index].size() - 1) {
      pq.push(num{arr[tmp.index][tmp.k + 1], tmp.index, tmp.k + 1});
    }
    result.push_back(tmp.val);
  }
  return result;
}

TEST(Algo, mergeNK) {
  vector<vector<int>> nums{vector<int>{2, 4, 6, 8, 9},
                           vector<int>{1, 2, 4, 6, 8, 12, 56},
                           vector<int>{4, 6, 8, 40, 46, 90}};
  auto result = mergeNK(nums);
  for (auto n : result) {
    cout << n << ", ";
  }
  cout << endl;
}

int robber(vector<int> val) {
  int dp_i_1 = 0, dp_i_2 = 0, dp_i = 0;
  for (int i = 2; i < val.size() + 2; i++) {
    dp_i = max(dp_i_1, dp_i_2 + val[i - 2]);
    dp_i_2 = dp_i_1;
    dp_i_1 = dp_i;
  }

  return dp_i;
}

int cycleRobber(vector<int> val) {
  vector<int> val1(val.begin(), val.end() - 1);
  vector<int> val2(val.begin() + 1, val.end());
  return max(robber(val1), robber(val2));
}

struct BNode{
  BNode* left, *right;
  int val;
};

int binaryRobber(BNode* root) {
  if (root == NULL) return 0;
  // 1. 抢root
  int rob = root->val;
  if (root->left != NULL) {
    rob += binaryRobber(root->left->left) + binaryRobber(root->left->right);
  }
  if (root->right != NULL) {
    rob += binaryRobber(root->right->left) + binaryRobber(root->right->right);
  }

  int not_rob = binaryRobber(root->left) + binaryRobber(root->right);
  return max(rob, not_rob);
}

TEST(Algo, robber) {
  vector<int> val1{1, 2, 3, 1};
  cout << robber(val1) << endl;
  vector<int> val2{2, 7, 9, 3, 1};
  cout << robber(val2) << endl;
  vector<int> val3{2, 3, 2};
  cout << cycleRobber(val3) << endl;
  vector<int> val4{1, 2, 3, 1};
  cout << cycleRobber(val4) << endl;
}