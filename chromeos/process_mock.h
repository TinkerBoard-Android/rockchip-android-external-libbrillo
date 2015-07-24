// Copyright (c) 2012 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBCHROMEOS_CHROMEOS_PROCESS_MOCK_H_
#define LIBCHROMEOS_CHROMEOS_PROCESS_MOCK_H_

#include <string>

#include <gmock/gmock.h>

#include "chromeos/process.h"

namespace chromeos {

class ProcessMock : public Process {
 public:
  ProcessMock() {}
  virtual ~ProcessMock() {}

  MOCK_METHOD1(AddArg, void(const std::string& arg));
  MOCK_METHOD1(RedirectOutput, void(const std::string& output_file));
  MOCK_METHOD2(RedirectUsingPipe, void(int child_fd, bool is_input));
  MOCK_METHOD2(BindFd, void(int parent_fd, int child_fd));
  MOCK_METHOD1(SetUid, void(uid_t));
  MOCK_METHOD1(SetGid, void(gid_t));
  MOCK_METHOD1(SetInheritParentSignalMask, void(bool));
  MOCK_METHOD1(SetPreExecCallback, void(const PreExecCallback&));
  MOCK_METHOD1(SetSearchPath, void(bool));
  MOCK_METHOD1(GetPipe, int(int child_fd));
  MOCK_METHOD0(Start, bool());
  MOCK_METHOD0(Wait, int());
  MOCK_METHOD0(Run, int());
  MOCK_METHOD0(pid, pid_t());
  MOCK_METHOD2(Kill, bool(int signal, int timeout));
  MOCK_METHOD1(Reset, void(pid_t));
  MOCK_METHOD1(ResetPidByFile, bool(const std::string& pid_file));
  MOCK_METHOD0(Release, pid_t());
};

}  // namespace chromeos

#endif  // LIBCHROMEOS_CHROMEOS_PROCESS_MOCK_H_
