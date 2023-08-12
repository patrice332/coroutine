#pragma once

#include <sys/ucontext.h>
#include <ucontext.h>

#include "absl/status/status.h"
#include "patrice332/coroutine/stack.hh"

namespace patrice332::coroutine {

class Task {
 public:
  explicit Task(Stack& stack);

  Task(Task const& other) = delete;
  Task& operator=(Task const& other) = delete;
  Task(Task&& other) noexcept;
  Task& operator=(Task&& other) noexcept;

  ~Task() = default;

  absl::Status Init(void (*func)(void*), void* data);
  absl::Status ResumeFrom(ucontext_t& from_context);

  constexpr ucontext_t* GetContext() { return &context_; }

 private:
  enum class State {
    kUninitialized,
    kInitialized,
  };

  Stack* stack_{nullptr};
  ucontext_t context_{};
  State state_{State::kUninitialized};
};

}  // namespace patrice332::coroutine