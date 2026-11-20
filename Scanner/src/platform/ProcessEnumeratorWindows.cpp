#include <Windows.h>
#include <TlHelp32.h>

#include "ProcessEnumeratorWindows.h"

namespace core
{
    std::string wideToUtf8(const std::wstring& w) 
    {
        if (w.empty()) return {};
        int len = ::WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(),
            nullptr, 0, nullptr, nullptr);
        std::string out(len, '\0');
        ::WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(),
            out.data(), len, nullptr, nullptr);
        return out;
    }

    std::string queryImagePath(HANDLE hProc) 
    {
        wchar_t buf[MAX_PATH * 4] = {};
        DWORD size = static_cast<DWORD>(std::size(buf));
        if (::QueryFullProcessImageNameW(hProc, 0, buf, &size) && size > 0)
            return wideToUtf8(std::wstring(buf, size));
        return {};
    }

	std::vector<ProcessInfo> ProcessEnumeratorWindows::list()
	{
		std::vector<ProcessInfo> result;

		auto hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) return result;

		PROCESSENTRY32W pe{};
		pe.dwSize = sizeof(pe);

        if (::Process32FirstW(hSnap, &pe)) 
        {
            do 
            {
                ProcessInfo info;
                info.pid = pe.th32ProcessID;
                info.parentPid = pe.th32ParentProcessID;
                info.name = wideToUtf8(pe.szExeFile);


                if (HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, info.pid)) 
                {
                    info.path = queryImagePath(hProcess);
                    info.is64bit = isProcess64bit(hProcess);
                    ::CloseHandle(hProcess);
                }

                result.push_back(std::move(info));
            } while (::Process32NextW(hSnap, &pe));
        }

        ::CloseHandle(hSnap);
        return result;
    }

    bool ProcessEnumeratorWindows::queryByPid(uint32_t pid, ProcessInfo& out)
    {
        HANDLE h = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (!h) return false;

        out = ProcessInfo{};
        out.pid = pid;
        out.path = queryImagePath(h);
        out.is64bit = isProcess64bit(h);

        auto pos = out.path.find_last_of("\\/");
        out.name = (pos == std::string::npos) ? out.path : out.path.substr(pos + 1);

        ::CloseHandle(h);
        return true;
    }

#ifdef _WIN32
    std::unique_ptr<IProcessEnumerator> IProcessEnumerator::create() 
    {
        return std::make_unique<ProcessEnumeratorWindows>();
    }
#endif
}