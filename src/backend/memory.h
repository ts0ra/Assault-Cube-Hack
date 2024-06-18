// Credit cazz for the original code

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>
#include <string_view>
#include <iostream>
#include <thread>
#include <vector>

class Memory
{
private:
    std::uintptr_t processId = 0;
    void* processHandle = nullptr;

public:
    // Constructor that finds the process id
    // and opens a handle
    Memory(const std::string_view processName) noexcept
    {
        std::cout << "[+] Memory constructor called, finding process handle: " << processName << std::endl;

        while (processId == 0)
        {
            ::PROCESSENTRY32 entry = { };
            entry.dwSize = sizeof(::PROCESSENTRY32);

            const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snapShot == INVALID_HANDLE_VALUE)
            {
                std::cout << "[-] Failed to create process snapshot, error code: " << GetLastError() << std::endl;
                exit(EXIT_FAILURE);
            }

            while (::Process32Next(snapShot, &entry))
            {
                if (!processName.compare(entry.szExeFile))
                {
                    processId = entry.th32ProcessID;
                    processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
                    if (processHandle == NULL)
                    {
                        std::cout << "[-] Failed to open process handle, error code: " << GetLastError() << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "[+] Procces handle found" << std::endl;
                    break;
                }
            }

            // Free handle
            if (::CloseHandle(snapShot) == 0)
            {
                std::cout << "[-] Failed to close process snapshot handle, error code: " << GetLastError() << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // Destructor that frees the opened handle
    ~Memory()
    {
        std::cout << "[-] Memory destructor called" << std::endl;
        if (processHandle)
        {
            if (::CloseHandle(processHandle) == 0)
            {
                std::cout << "[-] Failed to close process handle, error code: " << GetLastError() << std::endl;
            }
        }
    }

    // Returns the base address of a module by name
    const std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
    {
        ::MODULEENTRY32 entry = { };
        entry.dwSize = sizeof(::MODULEENTRY32);

        const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
        if (snapShot == INVALID_HANDLE_VALUE)
        {
            std::cout << "[-] Failed to create module snapshot, error code: " << GetLastError() << std::endl;
            return 0;
        }

        std::uintptr_t result = 0;
        while (::Module32Next(snapShot, &entry))
        {
            if (!moduleName.compare(entry.szModule))
            {
                result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
                break;
            }
        }

        if (::CloseHandle(snapShot) == 0)
        {
            std::cout << "[-] Failed to close module snapshot handle, error code: " << GetLastError() << std::endl;
        }

        if (result == 0)
        {
            std::cout << "[-] Module not found" << std::endl;
            exit(EXIT_FAILURE);
        }

        return result;
    }

    // Readstring process memory
    template <typename T>
    constexpr const void ReadChar(const std::uintptr_t& address, T* value, int size) const noexcept
    {
        if (!::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), value, sizeof(T) * size, NULL))
        {
            std::cout << "[-] Failed to read process memory, error code: " << GetLastError() << std::endl;
        }
    }

    // Read process memory
    template <typename T>
    constexpr const T Read(const std::uintptr_t& address) const noexcept
    {
        T value = { };
        if (!::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL))
        {
            std::cout << "[-] Failed to read process memory, error code: " << GetLastError() << std::endl;
        }
        return value;
    }

    template <typename T>
    constexpr const T Read(const std::uintptr_t& address, const std::vector<uintptr_t> &offsets) const noexcept
    {

        uintptr_t currentAddress = address;

        for (size_t i = 0; i < offsets.size(); ++i)
        {
            if (!::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(currentAddress + offsets[i]), &currentAddress, sizeof(uintptr_t), NULL))
            {
                std::cout << "[-] Failed to read process memory, error code: " << GetLastError() << std::endl;
            }
        }

        T value = { };
        if (!::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(currentAddress), &value, sizeof(T), NULL))
        {
            std::cout << "[-] Failed to read process memory, error code: " << GetLastError() << std::endl;
        }
        return value;
    }

    // Write process memory
    template <typename T>
    constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
    {
        if (!::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL))
        {
            std::cout << "[-] Failed to write process memory, error code: " << GetLastError() << std::endl;
        }
    }

    template <size_t N>
    constexpr void Write(const std::uintptr_t& address, const char(&value)[N]) const noexcept
    {
        if (!::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), value, N, NULL))
        {
            std::cout << "[-] Failed to write process memory, error code: " << GetLastError() << std::endl;
        }
    }

    const std::uintptr_t GetProcessId() const noexcept
    {
        return processId;
    }

    const std::uintptr_t GetHandleAddress() const noexcept
    {
        return reinterpret_cast<uintptr_t>(processHandle);
    }

};
