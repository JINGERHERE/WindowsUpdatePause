#include <iostream>
#include <windows.h>
#include <cctype>  // 用于 tolower 函数

using namespace std;

const char* SUBKEY = "SOFTWARE\\Microsoft\\WindowsUpdate\\UX\\Settings";
const char* VALUE_NAME = "FlightSettingsMaxPauseDays";

void printMenu() {
    cout << "================================" << endl;
    cout << "   Windows 更新暂停时间工具" << endl;
    cout << "================================" << endl;
    cout << "1. 设置/修改暂停天数" << endl;
    cout << "2. 清除设置 (恢复系统默认)" << endl;
    cout << "--------------------------------" << endl;
}

// 打开 Windows 更新设置页面
void openWindowsUpdate() {
    cout << "正在打开 Windows 更新设置页面..." << endl;
    ShellExecuteA(NULL, "open", "ms-settings:windowsupdate", NULL, NULL, SW_SHOWNORMAL);
}

int main() {
    int choice;
    printMenu();

    while (true) {
        cout << "请选择操作 [1-2]: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            break;
        }
        else {
            cout << "[错误] 无效的选择，请重新输入。" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            system("pause");
            system("cls");
            printMenu();
        }
    }

    HKEY hKey;
    LONG openRes = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        SUBKEY,
        0,
        KEY_SET_VALUE | KEY_WOW64_64KEY,
        &hKey
    );

    if (openRes != ERROR_SUCCESS) {
        if (openRes == ERROR_ACCESS_DENIED) {
            cout << "\n[错误] 权限不足！请右键点击本程序并选择“以管理员身份运行”。" << endl;
        }
        else {
            cout << "\n[错误] 无法打开注册表项，代码: " << openRes << endl;
        }
        system("pause");
        return 1;
    }

    if (choice == 1) {
        DWORD days;
        cout << "请输入想要暂停的天数: ";
        cin >> days;

        LONG setRes = RegSetValueExA(hKey, VALUE_NAME, 0, REG_DWORD, (const BYTE*)&days, sizeof(days));
        if (setRes == ERROR_SUCCESS) {
            cout << "\n[成功] 已将最大暂停天数设置为 " << days << " 天。" << endl;

            // 新增功能：询问是否打开更新页面
            RegCloseKey(hKey); // 先关闭注册表键

            char userChoice;
            while (true) {
                cout << "\n是否打开 Windows 更新设置页面？(y/n): ";
                cin >> userChoice;

                // 转换为小写以便统一判断
                userChoice = tolower(userChoice);

                if (userChoice == 'y') {
                    openWindowsUpdate();
                    break;
                }
                else if (userChoice == 'n') {
                    break;
                }
                else {
                    cout << "[错误] 无效输入，请输入 y 或 n。" << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            }
        }
        else {
            cout << "\n[失败] 设置失败，代码: " << setRes << endl;
            RegCloseKey(hKey);
        }
    }
    else {
        LONG delRes = RegDeleteValueA(hKey, VALUE_NAME);
        if (delRes == ERROR_SUCCESS) {
            cout << "\n[成功] 已删除自定义设置，恢复系统默认限制。" << endl;
        }
        else if (delRes == ERROR_FILE_NOT_FOUND) {
            cout << "\n[提示] 注册表项本就不存在，无需清除。" << endl;
        }
        else {
            cout << "\n[失败] 删除失败，代码: " << delRes << endl;
        }
        RegCloseKey(hKey);
    }

    cout << endl;
    system("pause");
    return 0;
}