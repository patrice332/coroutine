#include "patrice332/coroutine/task.hh"

#include <ucontext.h>

#include "absl/status/status.h"

namespace patrice332::coroutine {

namespace {
thread_local ucontext_t local_context;

struct TaskSetupOptions {
  void (*Func)(void*);
  void* Data;
  ucontext_t* CurrentContext;
};

void InitTask(void* ptr) {
  auto* options = reinterpret_cast<TaskSetupOptions*>(ptr);
  void (*func)(void*) = options->Func;
  void* data = options->Data;
  ucontext_t* current_context = options->CurrentContext;
  swapcontext(current_context, &local_context);
  func(data);
}

}  // namespace

Task::Task(Stack& stack) : stack_{&stack} {}
Task::Task(Task&& other) noexcept = default;
Task& Task::operator=(Task&& other) noexcept = default;

absl::Status Task::Init(void (*func)(void*), void* data) {
  if (getcontext(&context_) != 0) {
    return absl::ErrnoToStatus(errno, "Could not initialize Task context");
  }

  auto stack = stack_->GetMemory();

  context_.uc_stack.ss_sp = stack.data();
  context_.uc_stack.ss_size = stack.size();
  context_.uc_link = &local_context;

  TaskSetupOptions options{
      .Func = func, .Data = data, .CurrentContext = &context_};

  makecontext(&context_, reinterpret_cast<void (*)()>(&InitTask), 1, &options);
  swapcontext(&local_context, &context_);

  state_ = State::kInitialized;
  return absl::OkStatus();
}

absl::Status Task::Resume() {
  if (state_ != State::kInitialized) {
    return absl::FailedPreconditionError("Task is not in initialized state");
  }
  swapcontext(&local_context, &context_);
  return absl::OkStatus();
}

}  // namespace patrice332::coroutine