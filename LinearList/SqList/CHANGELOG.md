# 更新日志（CHANGELOG）

SqList（顺序表）的变更记录。格式参照 [Keep a Changelog](https://keepachangelog.com/)，
每条对应一次 git commit（细节用 `git show <commit>` 查）。

## [1.2] - 2026-10-12

> 自 1.1 分叉后的首次集中修订（三法则学习成果落地 + 日常打磨）。

### 变更
- **三法则落地（RAII）**：新增析构函数 `~SqList() { free(data); }` 与 `SqList() = default;`，
  拷贝构造/拷贝赋值 `=delete`——`SqList b = a;` 从运行期 double free 变成编译报错。
  资源释放交给作用域，不再靠人记（见测试节）
- 结构体成员默认初始化（`MaxSize=0` / `data=nullptr` / `len=0`），裸声明 `SqList L;` 安全
- 只读函数 `GetElem` / `LocateElem` 及辅助函数 `IsAccessPos` / `IsInsertPos` 改 `const SqList&` 传参
- `InitList` 重复调用语义 = **清空重置**：先分配成功、再释放旧空间，失败路径上原对象不变；
  拒绝"先 free 后 malloc"的写法（分配失败时表已被掏空）
- `InitSize` 由 `#define` 改 `constexpr`，根治宏尾分号陷阱（教学笔记保留在注释里）
- `ListInsert` 扩容兜底：`extra_size` 传 `MaxSize > 0 ? MaxSize : InitSize`，
  `MaxSize == 0` 的裸声明表直接插入也能真正开出空间
- `IncreaseListLength(0)` 成为真 no-op（直接返回，不再分配-复制-释放）
- 清脏数据循环注释停用（省开销；注意：malloc 区脏数据仍在，仅 `len` 界内不可见）

### 测试
- 新增 `REQUIRE` 致命断言宏：前置条件失败立即中止当前测试，
  防"分配失败被误报成一堆逻辑错误"（REQUIRE 内含 return，仅限 void 测试函数）
- 删除全部手写 `free(L.data)`——释放时机交给析构函数（RAII），杜绝中途 return 漏 free
- 填表测试补 `MaxSize` 不变断言（填表不触发扩容）

### 已知问题
- `IncreaseListLength` 无 `INT_MAX` 溢出防护（容量加法应先于乘法检查）——待 1.3 处理

## [1.1] - 2026-09-30

### 修复
- 插入移位循环起点 `L.len - 1` → `L.len`：修复每次移位插入吞掉表尾元素
  （逻辑 bug，ASan 检不出，由头插压力测试暴露）

### 测试
- 新增 main.cpp：5 组边界测试（空表/上下界/满表删除/扩容参数/头插压力）

## [1.0] - 2026-09-30

### 变更
- 接口位序化（1 基），参数序改为 `(L, pos, e)` 与王道一致
- `LocateElem` 返回位序 `i + 1`，与 `GetElem` 可正确往返

## [0.9] - 2026-09-29

### 已知问题
- 头插读 `data[-1]` 越界、表满删除读 `data[MaxSize]` 越界（初版两个 bug，后续版本修复）
