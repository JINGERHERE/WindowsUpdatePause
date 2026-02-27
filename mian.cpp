#include <iostream>
#include <windows.h>

using namespace std;

// 注册表路径和键名（相对路径，不含根键）
const char* SUBKEY = "SOFTWARE\\Microsoft\\WindowsUpdate\\UX\\Settings";
const char* VALUE_NAME = "FlightSettingsMaxPauseDays";

void printMenu() {
    cout << "================================" << endl;
    cout << "   Windows 更新暂停时间工具（X64）" << endl;
    cout << "================================" << endl;
    cout << "1. 设置/修改暂停天数" << endl;
    cout << "2. 清除设置 (恢复系统默认)" << endl;
    cout << "--------------------------------" << endl;
    cout << "请选择操作 [1-2]: ";
}

int main() {
    // 设置 UTF-8 编码，防止中文乱码（如需启用请取消注释）
    // system("chcp 65001 > nul");

    int choice;
    printMenu();
    cin >> choice;

    // 检查输入是否合法
    if (choice != 1 && choice != 2) {
        cout << "\n[错误] 无效的选择，请输入 1 或 2。" << endl;
        system("pause");
        return 1;
    }

    HKEY hKey;
    // 打开注册表项，使用 KEY_WOW64_64KEY 确保访问 64 位视图
    LONG openRes = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        SUBKEY,
        0,
        KEY_SET_VALUE | KEY_WOW64_64KEY,  // 强制写入 64 位注册表
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
        // 操作 1：设置天数
        DWORD days;
        cout << "请输入想要暂停的天数: ";
        cin >> days;

        LONG setRes = RegSetValueExA(hKey, VALUE_NAME, 0, REG_DWORD, (const BYTE*)&days, sizeof(days));
        if (setRes == ERROR_SUCCESS) {
            cout << "\n[成功] 已将最大暂停天数设置为 " << days << " 天。" << endl;
        }
        else {
            cout << "\n[失败] 设置失败，代码: " << setRes << endl;
        }
    }
    else if (choice == 2) {
        // 操作 2：删除键值
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
    }

    RegCloseKey(hKey);
    cout << endl;
    system("pause");
    return 0;
}