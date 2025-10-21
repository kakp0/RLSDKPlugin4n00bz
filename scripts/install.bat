@echo off
setlocal

set "bmPluginsFolder=%APPDATA%\bakkesmod\bakkesmod\plugins"
set "dllFile=%~dp0..\plugins\PluginTemplate.dll"

echo Copying .dll to bakkesmod plugins folder...

xcopy "%dllFile%" "%bmPluginsFolder%" /Y

if errorlevel 1 (
    echo Error occurred while copying .dll file.
    pause
    exit /b 1
) else (
    echo Plugin .dll successfully copied.
)

set "cfgFolder=%APPDATA%\bakkesmod\bakkesmod\cfg"
set "newLine=plugin load plugintemplate"
findstr /C:"%newLine%" "%cfgFolder%\plugins.cfg" >nul 2>&1
if %errorlevel% neq 0 (
    echo %newLine%>>"%cfgFolder%\plugins.cfg"
    echo Added 'plugin load' to plugins.cfg
)

echo.
echo Installation complete.
pause
endlocal