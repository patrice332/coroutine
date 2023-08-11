#include "patrice332/coroutine/stack.hh"

#include <gtest/gtest.h>

#include <cstdlib>
#include <type_traits>

namespace patrice332::coroutine {

namespace {

TEST(Stack, AlignedAllocIsOk) {
  constexpr std::size_t kPageSize = 4096;
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, kMemSize));
  auto task = MakeStack(std::span<std::byte>{mem, kMemSize});

  EXPECT_TRUE(task.ok());

  free(mem);
}

TEST(Stack, AlignedAllocSizeIsWrong) {
  constexpr std::size_t kPageSize = 4096;
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, kMemSize));
  auto task = MakeStack(std::span<std::byte>{mem, kMemSize - 2});

  EXPECT_FALSE(task.ok());

  EXPECT_EQ(task.status().message(),
            "Stack is not an integer multiple of 4096");

  free(mem);
}

TEST(Stack, AlignedAllocIsNotOk) {
  constexpr std::size_t kPageSize = 4096;
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, kMemSize));
  auto task = MakeStack(std::span<std::byte>{mem + 2, kMemSize});

  EXPECT_FALSE(task.ok());

  EXPECT_EQ(task.status().message(), "Stack is not aligned to a page");

  free(mem);
}

TEST(Stack, TooSmall) {
  constexpr std::size_t kPageSize = 4096;
  constexpr std::size_t kMemSize = 2 * kPageSize;
  auto* mem =
      reinterpret_cast<std::byte*>(std::aligned_alloc(kPageSize, kMemSize));
  auto task = MakeStack(std::span<std::byte>{mem + 2, kMemSize / 2});

  EXPECT_FALSE(task.ok());

  EXPECT_EQ(task.status().message(), "Stack is not large enough");

  free(mem);
}

}  // namespace

}  // namespace patrice332::coroutine