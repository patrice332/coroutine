#include "patrice332/coroutine/task.hh"

#include <ucontext.h>

#include "absl/status/status.h"
#include "patrice332/coroutine/runtime.hh"
#include "patrice332/coroutine/runtime_internal.hh"

namespace patrice332::coroutine {

namespace {
thread_local ucontext_t task_creation_context;

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
  swapcontext(current_context, &task_creation_context);
  func(data);
  internal::TaskExit();
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
  context_.uc_link = &task_creation_context;

  TaskSetupOptions options{
      .Func = func, .Data = data, .CurrentContext = &context_};

  makecontext(&context_, reinterpret_cast<void (*)()>(&InitTask), 1, &options);
  swapcontext(&task_creation_context, &context_);

  state_ = State::kInitialized;
  return absl::OkStatus();
}

absl::Status Task::ResumeFrom(ucontext_t& from_context) {
  if (state_ != State::kInitialized) {
    return absl::FailedPreconditionError("Task is not in initialized state");
  }
  swapcontext(&from_context, &context_);
  return absl::OkStatus();
}

}  // namespace patrice332::coroutine