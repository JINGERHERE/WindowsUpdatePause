# Windows暂停更新脚本

一个简单的一键暂停或恢复 Windows 10/11 系统更新（支持x86和x86）脚本。

## 使用方法
1. **以管理员身份运行**脚本（右键选择“以管理员身份运行”）。
2. 根据提示确认操作，输入需要暂停的天数或者清除设置。
<img width="1104" height="596" alt="image" src="https://github.com/user-attachments/assets/69fee708-2e35-441f-a33b-bc295530416e" />

## 原理
仅修改注册表，`HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\WindowsUpdate\\UX\\Settings`目录下新增`FlightSettingsMaxPauseDays`文件。

## 注意事项
- 修改注册表有风险，请提前备份重要数据。
- 本脚本仅适用于 Windows 10/11。
