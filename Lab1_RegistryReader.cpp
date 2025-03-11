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

void OutputDriverServicesStartingFromKey(HKEY hKey)
{
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
}

void OutputDriverServices()
{
    HKEY hKey;
    LONG lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SERVICES_KEY_PATH, 0, KEY_READ, &hKey);

    if (lRes != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key! Error: " << lRes << std::endl;
        return;
    }
    OutputDriverServicesStartingFromKey(hKey);
    RegCloseKey(hKey);
}

HKEY GetHKeyForPathStartingFromHKLM(const std::wstring& targetPath, HKEY currentKey = HKEY_LOCAL_MACHINE)
{
    HKEY hKey;
    DWORD index = 0;
    TCHAR subKeyName[256];
    DWORD subKeyNameSize = 256;

    if (targetPath.empty()) {
        return currentKey;
    }

    while (RegEnumKeyEx(currentKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
    {
        HKEY hSubKey;
        if (RegOpenKeyEx(currentKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
        {
            std::wstring newPath = targetPath;

            size_t pos = newPath.find(L'\\');
            std::wstring firstSegment = newPath.substr(0, pos);

            if (firstSegment == subKeyName)
            {
                if (pos != std::wstring::npos) {
                    newPath.erase(0, pos + 1);
                    return GetHKeyForPathStartingFromHKLM(newPath, hSubKey);
                }
                else {
                    return hSubKey;
                }
            }
            RegCloseKey(hSubKey);
        }
        subKeyNameSize = 256;
        index++;
    }

    return nullptr;
}

void OutputDriverServicesStartingFromHKLM()
{
    HKEY hKey = GetHKeyForPathStartingFromHKLM(L"SYSTEM\\CurrentControlSet\\Services");

    if (hKey) {
        OutputDriverServicesStartingFromKey(hKey);
        RegCloseKey(hKey);
    }
    else {
        std::wcerr << L"Failed to locate registry path." << std::endl;
    }
}


int main()
{
    //std::wcout << L"Reading registry values: \n";
    //ReadRegistryKeys();

    //std::wcout << L"Reading registry values for drivers: \n";
    //OutputDriverServices();

    std::wcout << L"Reading registry values for drivers, starting from HKLM: \n";
    OutputDriverServicesStartingFromHKLM();

    return 0;
}
