#include "patrice332/coroutine/runtime.hh"

#include <gtest/gtest.h>

namespace patrice332::coroutine {

namespace {

constexpr std::size_t kPageSize = 4096;

struct TestStack {
  TestStack(void* memory, Stack&& new_stack) : Mem{memory}, Stack{new_stack} {}
  ~TestStack() {
    if (Mem != nullptr) {
      free(Mem);
      Mem = nullptr;
    }
  }

  void* Mem;
  Stack Stack;
};

TestStack MakeTestStack(std::size_t mem_size) {
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, mem_size));

  auto stack = MakeStack(std::span<std::byte>{mem, mem_size});
  if (!stack.ok()) {
    std::terminate();
  }
  return TestStack{mem, std::move(stack).value()};
}

TEST(Runtime, Independent) {
  constexpr std::size_t kTaskStackSize = 2 * kPageSize;
  auto task0_stack = MakeTestStack(kTaskStackSize);
  auto task1_stack = MakeTestStack(kTaskStackSize);

  bool task0_called = false;
  bool task1_called = false;

  auto task_func = [](void* data) { *reinterpret_cast<bool*>(data) = true; };

  Task task0{task0_stack.Stack};
  Task task1{task1_stack.Stack};

  ASSERT_TRUE(task0.Init(task_func, &task0_called).ok());
  ASSERT_TRUE(task1.Init(task_func, &task1_called).ok());

  Schedule(&task0);
  Schedule(&task1);

  ASSERT_TRUE(Loop().ok());
  EXPECT_TRUE(task0_called);
  EXPECT_TRUE(task1_called);
}

TEST(Runtime, WithYield) {
  constexpr std::size_t kTaskStackSize = 2 * kPageSize;
  auto task0_stack = MakeTestStack(kTaskStackSize);
  auto task1_stack = MakeTestStack(kTaskStackSize);

  bool task0_called = false;
  bool task1_called = false;

  auto task_func = [](void* data) {
    (void)Yield();
    *reinterpret_cast<bool*>(data) = true;
  };

  Task task0{task0_stack.Stack};
  Task task1{task1_stack.Stack};

  ASSERT_TRUE(task0.Init(task_func, &task0_called).ok());
  ASSERT_TRUE(task1.Init(task_func, &task1_called).ok());

  Schedule(&task0);
  Schedule(&task1);

  if (auto status = Loop(); !status.ok()) {
    ASSERT_TRUE(status.ok()) << status.code() << ": " << status.message();
  }
  EXPECT_TRUE(task0_called);
  EXPECT_TRUE(task1_called);
}

}  // namespace

}  // namespace patrice332::coroutine