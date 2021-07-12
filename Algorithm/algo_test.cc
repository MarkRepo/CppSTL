/**
 * 算法：
 * 1. 相关头文件：<algorithm>, <numeric>, <functional>
 * 2. 所有算法都设计用来处理一个或多个iterator区间
 * 3. STL算法采用覆盖(overwrite)而不是安插(insert)模式
 * 4. 某些算法允许自定义操作：unary predicate, binary predicate
 * 5. 后缀 _if,
 * 如果某个算法有两种形式，参数个数相同，但第一形式要求传入一个值，第二形式要求传入一个函数或func
 * obj，那么后缀_if就派上用场。
 * 6. 后缀 _copy, 表示元素不只被操作，还会被复制到目标区间
 *
 * 分类：
 * 1. 非更易型算法(nonmodifying)
 * 2. 更易型算法（modifying)
 * 3. 移除型算法（removing）
 * 4. 变序型算法（mutating）
 * 5. 排序算法（sorting）
 * 6. 已排序区间算法（sorted-range）
 * 7. 数值算法（numeric）
 */
