#include <Windows.h>
#include "IProcessMemory.h"

namespace core
{
	class ProcessMemoryWindows : public IProcessMemory
	{
    public:
		ProcessMemoryWindows() = default;
		~ProcessMemoryWindows() override;

		ProcessMemoryWindows(const ProcessMemoryWindows&) = delete;
		ProcessMemoryWindows& operator=(const ProcessMemoryWindows&) = delete;

        bool attach(pid_t pid) override;
        void detach() override;
        bool isAttached() const override { return m_hProcess != nullptr; }
        pid_t pid() const override { return m_pid; }

        bool read(uintptr_t address, void* buffer, size_t size) override;
        bool write(uintptr_t address, const void* buffer, size_t size) override;

        std::vector<MemoryRegion> queryRegions() override;
        size_t pointerSize() const override { return sizeof(void*); }
        bool isTarget64bit() const override { return m_target64; }

    private:
        HANDLE   m_hProcess = nullptr;
        uint32_t m_pid = 0;
	};
}