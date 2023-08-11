#include "patrice332/coroutine/task.hh"

#include <gtest/gtest.h>

namespace patrice332::coroutine {

namespace {

TEST(Task, Sanity) {
  constexpr std::size_t kPageSize = 4096;
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, kMemSize));
  {
    auto stack = MakeStack(std::span<std::byte>{mem, kMemSize});

    ASSERT_TRUE(stack.ok());

    Task task{stack.value()};
  }
  free(mem);
}

TEST(Task, Called) {
  constexpr std::size_t kPageSize = 4096;
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, kMemSize));
  {
    auto stack = MakeStack(std::span<std::byte>{mem, kMemSize});

    ASSERT_TRUE(stack.ok());

    Task task{stack.value()};

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
  free(mem);
}

}  // namespace

}  // namespace patrice332::coroutine