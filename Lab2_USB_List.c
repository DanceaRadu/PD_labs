#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>

#pragma comment(lib, "setupapi.lib")

void print_device_property(HDEVINFO hDevInfo, SP_DEVINFO_DATA devInfoData, DWORD property, const char* label) {
    char buffer[1024];
    DWORD requiredSize = 0;
    DWORD dataType;

    if (SetupDiGetDeviceRegistryPropertyA(
        hDevInfo,
        &devInfoData,
        property,
        &dataType,
        (PBYTE)buffer,
        sizeof(buffer),
        &requiredSize)) {
        printf("  %s: %s\n", label, buffer);
    }
    else {
        printf("  %s: (Unavailable)\n", label);
    }
}

int main() {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD deviceIndex = 0;

    deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB,
        NULL,
        NULL,
        DIGCF_PRESENT);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        printf("Failed to get device list.\n");
        return 1;
    }

    printf("USB Devices Connected:\n");

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        printf("Device %d:\n", deviceIndex + 1);

        print_device_property(deviceInfoSet, deviceInfoData, SPDRP_DEVICEDESC, "Description");
        print_device_property(deviceInfoSet, deviceInfoData, SPDRP_HARDWAREID, "Hardware ID");
        print_device_property(deviceInfoSet, deviceInfoData, SPDRP_MFG, "Manufacturer");

        printf("\n");
        deviceIndex++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return 0;
}
