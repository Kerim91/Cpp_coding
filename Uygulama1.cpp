//g++ -o process_launcher.exe process_launcher.cpp
//C:\Windows\SysWOW64\notepad.exe
#include <windows.h>
#include <iostream>


int main() {
    // Komut satırı argümanlarını al
    LPWSTR commandLine = GetCommandLineW();

    // Argümanları ayrıştır
    int argc;
    LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
    if (argv == NULL) {
        std::wcerr << L"Cannot Parse Arguments." << std::endl;
        return 1;
    }


    // Argümanları ekrana yazdır
    std::wcout << L"CommandLine Arguments:" << std::endl;
    for (int i = 1; i < argc; i++) {
        std::wcout << L"Arguments " << i << L": " << argv[i] << std::endl;
    }

    // En az bir argüman varsa (örn. çalıştırılacak uygulama)
    if (argc > 1) {
        STARTUPINFOW startupInfo = { 0 };
        PROCESS_INFORMATION processInfo = { 0 };
        startupInfo.cb = sizeof(startupInfo);

        // Komut satırını oluştur
        LPCWSTR applicationName = argv[1];  // İlk argüman çalıştırılacak process
        LPWSTR arguments = (argc > 2) ? argv[2] : NULL;  // Diğer argümanlar

        // Yeni bir process başlat
        if (CreateProcessW(
            applicationName,       // Çalıştırılacak uygulama adı
            arguments,              // Argümanlar
            NULL,                   // Process güvenlik öznitelikleri
            NULL,                   // Thread güvenlik öznitelikleri
            FALSE,                  // Miras alınan tanıtıcılar
            0,                      // Ek process oluşturma bayrakları
            NULL,                   // Ortam değişkenleri
            NULL,                   // Çalıştırılacak dizin
            &startupInfo,           // Başlatma bilgileri
            &processInfo            // Process bilgileri
        )) {
            std::wcout << L"Process started succesfully!" << std::endl;

            // Process'in bitmesini bekle
            WaitForSingleObject(processInfo.hProcess, INFINITE);

            // Process ve thread handle'larını kapat
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }
        else {
            std::wcerr << L"Process Start Failed. Error Code: " << GetLastError() << std::endl;
        }
    }
    else {
        std::wcerr << L"Please give an process full name!" << std::endl;
    }

    // Belleği serbest bırak
    LocalFree(argv);

    return 0;
}
