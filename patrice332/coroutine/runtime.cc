#include "patrice332/coroutine/runtime.hh"

#include <sys/ucontext.h>
#include <ucontext.h>

#include "absl/base/optimization.h"
#include "absl/status/status.h"

namespace patrice332::coroutine {

namespace {
thread_local std::deque<Task*> task_list;
thread_local Task* running_task{nullptr};
thread_local Task* (*on_suspended_cb)(Task*){nullptr};
thread_local ucontext_t scheduler_context;
}  // namespace

namespace internal {
Task* OnSuspendedDefaultCB(Task* task) { return task; }
}  // namespace internal

void Schedule(Task* task) { task_list.emplace_back(task); }

absl::Status LoopOnce() {
  if (ABSL_PREDICT_FALSE(task_list.empty())) {
    return absl::OkStatus();
  }
  if (ABSL_PREDICT_FALSE(running_task != nullptr)) {
    return absl::FailedPreconditionError("scheduler is already running");
  }
  running_task = task_list.front();
  task_list.pop_front();
  auto status = running_task->ResumeFrom(scheduler_context);
  if (on_suspended_cb != nullptr) {
    auto* local_on_suspended = on_suspended_cb;
    on_suspended_cb = nullptr;
    auto* new_task = local_on_suspended(running_task);
    if (new_task != nullptr) {
      if (new_task == running_task) {
        task_list.emplace_back(running_task);
      } else {
        task_list.emplace_front(new_task);
      }
    }
  } else {
    Schedule(running_task);
  }
  running_task = nullptr;
  return status;
}

absl::Status Loop() {
  while (!task_list.empty()) {
    auto status = LoopOnce();
    if (!status.ok()) {
      return status;
    }
  }
  return absl::OkStatus();
}

absl::Status Yield(Task* (*on_suspended)(Task*)) {
  on_suspended_cb = on_suspended;
  swapcontext(running_task->GetContext(), &scheduler_context);
  return absl::OkStatus();
}

void TaskExit() {
  on_suspended_cb = [](Task*) -> Task* { return nullptr; };
  swapcontext(running_task->GetContext(), &scheduler_context);
}

}  // namespace patrice332::coroutine