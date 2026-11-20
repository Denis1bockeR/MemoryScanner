#include "ProcessMemoryWindows.h"

namespace core
{
	void decodeProtect(DWORD protect, MemoryRegion& r) 
	{
		r.readable = r.writable = r.executable = false;

		if (protect == 0 || protect == PAGE_NOACCESS)
			return;

		if (protect & PAGE_GUARD)
			return;

		DWORD base = protect & 0xFF;

		switch (base) {
		case PAGE_READONLY:
			r.readable = true;
			break;
		case PAGE_READWRITE:
		case PAGE_WRITECOPY:
			r.readable = r.writable = true;
			break;
		case PAGE_EXECUTE:
			r.executable = true;
			break;
		case PAGE_EXECUTE_READ:
			r.readable = r.executable = true;
			break;
		case PAGE_EXECUTE_READWRITE:
		case PAGE_EXECUTE_WRITECOPY:
			r.readable = r.writable = r.executable = true;
			break;
		default:
			break;
		}
	}

	ProcessMemoryWindows::~ProcessMemoryWindows()
	{
		detach();
	}

	bool ProcessMemoryWindows::attach(pid_t pid)
	{
		detach();

		if (!pid) return false;

		HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, false, pid);

		if (!handle) handle = ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION, false, pid);

		if (!handle) return false;

		m_hProcess = handle;
		m_pid = pid;
		m_target64 = isProcess64bit(handle);

		int wow64 = 0;
		if (sizeof(void*) == 4 && m_target64)
		{
			detach();
			return false;
		}

		return true;
	}

	void ProcessMemoryWindows::detach()
	{
		if (m_hProcess)
		{
			::CloseHandle(m_hProcess);
			m_hProcess = nullptr;
		}

		m_pid = 0;
		m_target64 = false;
	}

	bool ProcessMemoryWindows::read(uintptr_t address, void* buffer, size_t size)
	{
		if (!m_hProcess || !buffer || size == 0) return false;

		size_t bytesRead = 0;
		auto readOk = ::ReadProcessMemory(m_hProcess, reinterpret_cast<const void*>(address), buffer, size, &bytesRead);

		return readOk && bytesRead == size;
	}

	bool ProcessMemoryWindows::write(uintptr_t address, const void* buffer, size_t size)
	{
		if (!m_hProcess || !buffer || size == 0) return false;

		size_t bytesWrite = 0;
		auto readOk = ::WriteProcessMemory(m_hProcess, reinterpret_cast<void*>(address), buffer, size, &bytesWrite);

		return readOk && bytesWrite == size;
	}

	std::vector<MemoryRegion> ProcessMemoryWindows::queryRegions()
	{
		std::vector<MemoryRegion> regions;
		if (!m_hProcess)
			return regions;

		const uintptr_t maxAddr = m_target64 ? 0x00007FFFFFFFFFFFULL
			: 0x00000000FFFFFFFFULL;

		MEMORY_BASIC_INFORMATION mbi{};
		uintptr_t addr = 0;
		
		regions.reserve(2048);

		while (addr < maxAddr)
		{
			if (::VirtualQueryEx(m_hProcess, reinterpret_cast<const void*>(addr), &mbi, sizeof(mbi)) == 0) break;

			uintptr_t next = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
			if (next <= addr) break;

			if (mbi.State == MEM_COMMIT && mbi.RegionSize > 0) 
			{
				MemoryRegion region;
				region.base = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
				region.size = static_cast<size_t>(mbi.RegionSize);
				region.state = mbi.State;
				region.type = mbi.Type;
				region.protect = mbi.Protect;
				decodeProtect(mbi.Protect, region);
				regions.push_back(std::move(region));
			}
			
			addr = next;

			if (sizeof(void*) == 4 && addr > maxAddr)
				break;
		}

		return regions;
	}
}