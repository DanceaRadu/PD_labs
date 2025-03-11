#include <windows.h>
#include <iostream>
#include <tchar.h>

#define SERVICES_KEY_PATH _T("SYSTEM\\CurrentControlSet\\Services")
#define VALUE_NAME _T("ImagePath")

void ReadRegistryKeys()
{
    HKEY hKey;
    LONG lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SERVICES_KEY_PATH, 0, KEY_READ, &hKey);

    if (lRes != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key! Error: " << lRes << std::endl;
        return;
    }

    TCHAR subKeyName[256];
    DWORD subKeyNameSize = 256;
    DWORD index = 0;

    DWORD totalServices = 0;
    while (RegEnumKeyEx(hKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
        HKEY hSubKey;
        if (RegOpenKeyEx(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
        {
            TCHAR imagePath[512];
            DWORD imagePathSize = sizeof(imagePath);
            DWORD type = REG_SZ;

            if (RegQueryValueEx(hSubKey, VALUE_NAME, NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS)
            {
                totalServices++;
                std::wcout << L"Service: " << subKeyName << L" | ImagePath: " << imagePath << std::endl;
            }
            RegCloseKey(hSubKey);
        }

        subKeyNameSize = 256;
        index++;
    }

    std::wcout << std::endl;
    std::wcout << "Total: " << totalServices << " services" << std::endl;
    RegCloseKey(hKey);
}

void OutputDriverServices() {
    HKEY hKey;
    LONG lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SERVICES_KEY_PATH, 0, KEY_READ, &hKey);

    if (lRes != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key! Error: " << lRes << std::endl;
        return;
    }

    TCHAR subKeyName[256];
    DWORD subKeyNameSize = 256;
    DWORD index = 0;


    DWORD totalDrivers = 0;
    while (RegEnumKeyEx(hKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
        HKEY hSubKey;
        if (RegOpenKeyEx(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
        {
            DWORD typeType = REG_DWORD;
            DWORD typeSize = sizeof(DWORD);
            DWORD type = 0;

            if (RegQueryValueEx(hSubKey, L"Type", NULL, &typeType, (LPBYTE)&type, &typeSize) == ERROR_SUCCESS)
            {
                if (type == 0x01 || type == 0x02)
                {
                    totalDrivers++;
                    TCHAR imagePath[512];
                    DWORD imagePathSize = sizeof(imagePath);
                    DWORD type = REG_SZ;

                    if (RegQueryValueEx(hSubKey, VALUE_NAME, NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS)
                    {
                        std::wcout << L"Service: " << subKeyName << L" | ImagePath: " << imagePath << std::endl;
                    }
                }
                RegCloseKey(hSubKey);
            }

            subKeyNameSize = 256;
            index++;
        }
    }
    std::wcout << std::endl;
    std::wcout << "Total: " << totalDrivers << " drivers" << std::endl;
    RegCloseKey(hKey);
}

void OutputDriverServicesStartingFromHKLM() {
    HKEY hKey;
    LONG lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"", 0, KEY_READ, &hKey);

    if (lRes != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key! Error: " << lRes << std::endl;
        return;
    }

    TCHAR subKeyName[256];
    DWORD subKeyNameSize = 256;
    DWORD index = 0;


    DWORD totalDrivers = 0;
    while (RegEnumKeyEx(hKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
        HKEY hSubKey;
        if (RegOpenKeyEx(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
        {
            DWORD typeType = REG_DWORD;
            DWORD typeSize = sizeof(DWORD);
            DWORD type = 0;

            if (RegQueryValueEx(hSubKey, L"Type", NULL, &typeType, (LPBYTE)&type, &typeSize) == ERROR_SUCCESS)
            {
                if (type == 0x01 || type == 0x02)
                {
                    totalDrivers++;
                    TCHAR imagePath[512];
                    DWORD imagePathSize = sizeof(imagePath);
                    DWORD type = REG_SZ;

                    if (RegQueryValueEx(hSubKey, VALUE_NAME, NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS)
                    {
                        std::wcout << L"Service: " << subKeyName << L" | ImagePath: " << imagePath << std::endl;
                    }
                }
                RegCloseKey(hSubKey);
            }

            subKeyNameSize = 256;
            index++;
        }
    }
    std::wcout << std::endl;
    std::wcout << "Total: " << totalDrivers << " drivers" << std::endl;
    RegCloseKey(hKey);
}

int main()
{
    //std::wcout << L"Reading registry values: \n";
    //ReadRegistryKeys();

    ///*std::wcout << L"Reading registry values for drivers: \n";
    //OutputDriverServices();*/

    std::wcout << L"Reading registry values for drivers, starting from HKLM: \n";
    OutputDriverServicesStartingFromHKLM();

    return 0;
}
