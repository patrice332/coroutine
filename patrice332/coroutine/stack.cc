#include "patrice332/coroutine/stack.hh"

#include "absl/status/status.h"

namespace patrice332::coroutine {

namespace {
constexpr std::size_t kPageSize = 4096;
}

absl::StatusOr<Stack> MakeStack(std::span<std::byte> memory) noexcept {
  Stack stack;
  if (absl::Status status = stack.SetStack(memory); !status.ok()) {
    return status;
  }
  return stack;
}

absl::Status Stack::SetStack(std::span<std::byte> stack) noexcept {
  if (absl::Status status = validateStack(stack); !status.ok()) {
    return status;
  }
  stack_ = stack;
  return absl::OkStatus();
}

absl::Status Stack::validateStack(std::span<std::byte> stack) noexcept {
  if (absl::Status status = validateStack(stack.size()); !status.ok()) {
    return status;
  }
  if (reinterpret_cast<std::uintptr_t>(stack.data()) % kPageSize != 0) {
    return absl::InvalidArgumentError("Stack is not aligned to a page");
  }
  return absl::OkStatus();
}

absl::Status Stack::validateStack(std::size_t size) noexcept {
  if (size % kPageSize != 0) {
    return absl::InvalidArgumentError(
        "Stack is not an integer multiple of 4096");
  }
  if (size < (2 * kPageSize)) {
    return absl::InvalidArgumentError("Stack is not large enough");
  }
  return absl::OkStatus();
}

}  // namespace patrice332::coroutine