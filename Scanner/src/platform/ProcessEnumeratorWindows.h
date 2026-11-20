#pragma once

#include "IProcessEnumerator.h"

namespace core
{
	class ProcessEnumeratorWindows : public IProcessEnumerator
	{
	public:
		ProcessEnumeratorWindows() = default;
		~ProcessEnumeratorWindows() = default;

		ProcessEnumeratorWindows(const ProcessEnumeratorWindows&) = delete;
		ProcessEnumeratorWindows& operator=(const ProcessEnumeratorWindows&) = delete;

		std::vector<ProcessInfo> list() override;
		bool queryByPid(pid_t pid, ProcessInfo& out) override;
	};
}