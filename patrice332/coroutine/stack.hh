#pragma once

#include <span>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/statusor.h"

namespace patrice332::coroutine {

class Stack {
 public:
  constexpr Stack() = default;

  // Set the stack
  // Stack must be larger than 8k, and integer multiple of 4k and aligned to a
  // 4k boundary.
  absl::Status SetStack(std::span<std::byte> stack) noexcept;

  [[nodiscard]] constexpr std::span<std::byte> GetMemory() const {
    return stack_;
  }

 private:
  static absl::Status validateStack(std::span<std::byte> stack) noexcept;
  static absl::Status validateStack(std::size_t size) noexcept;

  std::span<std::byte> stack_{};
};

absl::StatusOr<Stack> MakeStack(std::span<std::byte> memory) noexcept;

}  // namespace patrice332::coroutine