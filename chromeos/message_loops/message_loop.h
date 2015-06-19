// Copyright 2015 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBCHROMEOS_CHROMEOS_MESSAGE_LOOPS_MESSAGE_LOOP_H_
#define LIBCHROMEOS_CHROMEOS_MESSAGE_LOOPS_MESSAGE_LOOP_H_

#include <string>

#include <base/callback.h>
#include <base/location.h>
#include <base/time/time.h>
#include <chromeos/chromeos_export.h>

namespace chromeos {

class CHROMEOS_EXPORT MessageLoop {
 public:
  virtual ~MessageLoop();

  // A unique task identifier used to refer to scheduled callbacks.
  using TaskId = uint64_t;

  // The kNullEventId is reserved for an invalid task and will never be used
  // to refer to a real task.
  static const TaskId kTaskIdNull;

  // Return the MessageLoop for the current thread.
  static MessageLoop* current();

  // Set this message loop as the current thread main loop. Only one message
  // loop can be set at a time. Use ReleaseFromCurrent() to release it.
  void SetAsCurrent();

  // Release this instance from the current thread. This instance must have
  // been previously set with SetAsCurrent().
  void ReleaseFromCurrent();

  // Schedule a Closure |task| to be executed after a |delay|. Returns a task
  // identifier for the scheduled task that can be used to cancel the task
  // before it is fired by passing it to CancelTask().
  // In case of an error scheduling the task, the kTaskIdNull is returned.
  // Note that once the call is executed or canceled, the TaskId could be reused
  // at a later point.
  // This methond can only be called from the same thread running the main loop.
  virtual TaskId PostDelayedTask(const tracked_objects::Location& from_here,
                                 const base::Closure &task,
                                 base::TimeDelta delay) = 0;
  // Variant without the Location for easier usage.
  TaskId PostDelayedTask(const base::Closure &task,
                         base::TimeDelta delay) {
    return PostDelayedTask(tracked_objects::Location(), task, delay);
  }

  // A convenience method to schedule a call with no delay.
  // This methond can only be called from the same thread running the main loop.
  TaskId PostTask(const base::Closure &task) {
    return PostDelayedTask(task, base::TimeDelta());
  }
  TaskId PostTask(const tracked_objects::Location& from_here,
                  const base::Closure &task) {
    return PostDelayedTask(from_here, task, base::TimeDelta());
  }

  // Cancel a scheduled task. Returns whether the task was canceled. For
  // example, if the callback was already executed (or is being executed) or was
  // already canceled this method will fail. Note that the TaskId can be reused
  // after it was executed or cancelled.
  virtual bool CancelTask(TaskId task_id) = 0;

  // ---------------------------------------------------------------------------
  // Methods used to run and stop the message loop.

  // Run one iteration of the message loop, dispatching up to one task. The
  // |may_block| tells whether this method is allowed to block waiting for a
  // task to be ready to run. Returns whether it ran a task. Note that even
  // if |may_block| is true, this method can return false immediately if there
  // are no more tasks registered.
  virtual bool RunOnce(bool may_block) = 0;

  // Run the main loop until there are no more registered tasks.
  virtual void Run();

  // Quit the running main loop immediately. This method will make the current
  // running Run() method to return right after the current task returns back
  // to the message loop without processing any other task.
  virtual void BreakLoop();

 protected:
  MessageLoop() = default;

 private:
  // Tells whether Run() should quit the message loop in the default
  // implementation.
  bool should_exit_ = false;

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace chromeos

#endif  // LIBCHROMEOS_CHROMEOS_MESSAGE_LOOPS_MESSAGE_LOOP_H_
