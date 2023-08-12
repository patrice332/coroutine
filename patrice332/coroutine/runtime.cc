#include "patrice332/coroutine/runtime.hh"

#include <sys/ucontext.h>
#include <ucontext.h>

#include "absl/base/optimization.h"
#include "absl/status/status.h"
#include "patrice332/coroutine/runtime_internal.hh"

namespace patrice332::coroutine {

namespace {

struct RuntimeState {
  std::deque<Task*> TaskList;
  Task* RunningTask{nullptr};
  Task* (*OnSuspendedCB)(Task*){nullptr};
  ucontext_t SchedulerContext{};
};

thread_local RuntimeState runtime_state;

}  // namespace

namespace internal {
Task* OnSuspendedDefaultCB(Task* task) { return task; }
}  // namespace internal

void Schedule(Task* task) { runtime_state.TaskList.emplace_back(task); }

absl::Status LoopOnce() {
  if (ABSL_PREDICT_FALSE(runtime_state.TaskList.empty())) {
    return absl::OkStatus();
  }
  if (ABSL_PREDICT_FALSE(runtime_state.RunningTask != nullptr)) {
    return absl::FailedPreconditionError("scheduler is already running");
  }
  runtime_state.RunningTask = runtime_state.TaskList.front();
  runtime_state.TaskList.pop_front();
  auto status =
      runtime_state.RunningTask->ResumeFrom(runtime_state.SchedulerContext);
  if (runtime_state.OnSuspendedCB != nullptr) {
    auto* local_on_suspended = runtime_state.OnSuspendedCB;
    runtime_state.OnSuspendedCB = nullptr;
    auto* new_task = local_on_suspended(runtime_state.RunningTask);
    if (new_task != nullptr) {
      if (new_task == runtime_state.RunningTask) {
        runtime_state.TaskList.emplace_back(runtime_state.RunningTask);
      } else {
        runtime_state.TaskList.emplace_front(new_task);
      }
    }
  } else {
    Schedule(runtime_state.RunningTask);
  }
  runtime_state.RunningTask = nullptr;
  return status;
}

absl::Status Loop() {
  while (!runtime_state.TaskList.empty()) {
    auto status = LoopOnce();
    if (!status.ok()) {
      return status;
    }
  }
  return absl::OkStatus();
}

absl::Status Yield(Task* (*on_suspended)(Task*)) {
  runtime_state.OnSuspendedCB = on_suspended;
  swapcontext(runtime_state.RunningTask->GetContext(),
              &runtime_state.SchedulerContext);
  return absl::OkStatus();
}

namespace internal {
void TaskExit() {
  (void)Yield([](Task*) -> Task* { return nullptr; });
  ABSL_UNREACHABLE();
}
}  // namespace internal

}  // namespace patrice332::coroutine