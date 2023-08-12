#pragma once

#include <functional>

#include "patrice332/coroutine/task.hh"

namespace patrice332::coroutine {

namespace internal {
Task* OnSuspendedDefaultCB(Task* task);
}

// Add a task to run on the scheduler
void Schedule(Task* task);

// Yield to another Task
absl::Status Yield(
    Task* (*on_suspended_cb)(Task*) = &internal::OnSuspendedDefaultCB);

// Run the scheduler once
absl::Status LoopOnce();

// Run loop till |stop| is true
absl::Status Loop();

void TaskExit();

}  // namespace patrice332::coroutine