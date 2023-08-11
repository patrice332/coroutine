#include "patrice332/coroutine/task.hh"

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

TEST(Task, Sanity) {
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto stack = MakeTestStack(kMemSize);

  Task task{stack.Stack};
}

TEST(Task, Called) {
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto stack = MakeTestStack(kMemSize);

  Task task{stack.Stack};

  bool called = false;

  if (auto status = task.Init(
          [](void* ptr) { *reinterpret_cast<bool*>(ptr) = true; }, &called);
      !status.ok()) {
    ASSERT_TRUE(status.ok());
  }

  EXPECT_FALSE(called);
  if (auto status = task.Resume(); !status.ok()) {
    ASSERT_TRUE(status.ok());
  }

  EXPECT_TRUE(called);
}

}  // namespace

}  // namespace patrice332::coroutine