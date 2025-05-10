//g++ -o process_thread_info.exe process_thread_info.cpp -mconsole
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Kullaným: process_thread_info <pid>" << std::endl;
        return 1;
    }

    // PID'yi al
    DWORD pid = atoi(argv[1]);

    // Thread snapshot oluþtur
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Thread snapshot oluþturulamadý." << std::endl;
        return 1;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    // Thread bilgilerini al
    if (!Thread32First(hThreadSnap, &te32)) {
        std::cerr << "Ýlk thread bilgisi alýnamadý." << std::endl;
        CloseHandle(hThreadSnap);
        return 1;
    }

    std::cout << "PID: " << pid << " için thread bilgileri:" << std::endl;

    // Thread'leri listele
    do {
        if (te32.th32OwnerProcessID == pid) {
            std::cout << "Thread ID: " << te32.th32ThreadID << std::endl;
        }
    } while (Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);
    return 0;
}
