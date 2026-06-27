:start
@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: PlotTronix VI Lab - Arduino CLI Helper Script
:: Compile, Upload, Monitor with Output Logging
:: ============================================================================

title PlotTronix VI Lab - Arduino CLI Helper
color 0A
cls

set "LOG_DIR=%CD%\logs"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
for /f "usebackq" %%i in (`powershell -NoProfile -command "Get-Date -Format 'yyyyMMdd_HHmmss'"`) do set "TIMESTAMP=%%i"
set "LOG_FILE=%LOG_DIR%\serial_output_%TIMESTAMP%.log"


echo.
echo ============================================================================
echo  PlotTronix VI Lab - Arduino Nano (COM5 @ 9600 baud)
echo ============================================================================
echo.
echo Options:
echo   [1] Compile only
echo   [2] Compile and Upload to Arduino
echo   [3] Monitor Serial Output (9600 baud) - Real-time view
echo   [4] Monitor and Log Output - Save to file
echo   [5] Compile + Upload + Monitor with Logging
echo   [6] View Latest Log File
echo   [7] Exit
echo.

set /p choice="Select option (1-7): "

if "%choice%"=="1" goto compile
if "%choice%"=="2" goto compile_upload
if "%choice%"=="3" goto monitor
if "%choice%"=="4" goto monitor_log
if "%choice%"=="5" goto compile_upload_monitor_log
if "%choice%"=="6" goto view_log
if "%choice%"=="7" goto end
goto invalid

:compile
echo.
echo [*] Compiling for Arduino Nano...
echo.
arduino-cli compile --fqbn arduino:avr:nano Plotronix_VI_Lab.ino
if %errorlevel% equ 0 (
    echo.
    echo [OK] Compilation successful!
) else (
    echo.
    echo [ERROR] Compilation failed!
    pause
    goto menu
)
goto menu

:upload
echo.
echo [*] Uploading to Arduino on COM5...
echo.
arduino-cli upload --fqbn arduino:avr:nano --port COM5 Plotronix_VI_Lab.ino
if %errorlevel% equ 0 (
    echo.
    echo [OK] Upload successful!
    echo [*] Waiting 2 seconds for device to initialize...
    timeout /t 2 /nobreak
) else (
    echo.
    echo [ERROR] Upload failed! Check connection on COM5.
    pause
    goto menu
)
goto menu

:compile_upload
echo.
echo [*] Step 1/2: Compiling...
echo.
arduino-cli compile --fqbn arduino:avr:nano Plotronix_VI_Lab.ino
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed!
    pause
    goto menu
)
echo.
echo [OK] Compilation successful!
echo.
echo [*] Step 2/2: Uploading to Arduino on COM5...
echo.
arduino-cli upload --fqbn arduino:avr:nano --port COM5 Plotronix_VI_Lab.ino
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Upload failed! Check connection on COM5.
    pause
    goto menu
)
echo.
echo [OK] Upload successful!
echo [*] Waiting 2 seconds for device to initialize...
timeout /t 2 /nobreak
goto menu

:monitor
echo.
echo [*] Monitoring serial output on COM5 (Press Ctrl+C to stop)...
echo [*] Watching for DATA frames from controller...
echo.
arduino-cli monitor --port COM5
goto menu

:monitor_log
echo.
echo [*] Monitoring serial output and saving to log file...
echo [*] Log file: %LOG_FILE%
echo [*] Press Ctrl+C to stop monitoring
echo.
echo ============================================================================
echo  LIVE CONTROLLER OUTPUT (Timestamp | Data)
echo ============================================================================
echo.
powershell -NoProfile -Command "arduino-cli monitor --port COM5 | Tee-Object -FilePath '%LOG_FILE%'"
if %errorlevel% equ 0 (
    echo.
    echo [OK] Monitoring complete. Output saved to log file.
    echo [*] Log location: %LOG_FILE%
) else (
    echo.
    echo [ERROR] Monitoring interrupted or failed.
)
timeout /t 2 /nobreak
goto menu

:compile_upload_monitor_log
echo.
echo [*] Step 1/3: Compiling...
echo.
arduino-cli compile --fqbn arduino:avr:nano Plotronix_VI_Lab.ino
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed!
    pause
    goto menu
)
echo.
echo [OK] Compilation successful!
echo.
echo [*] Step 2/3: Uploading to Arduino on COM5...
echo.
arduino-cli upload --fqbn arduino:avr:nano --port COM5 Plotronix_VI_Lab.ino
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Upload failed!
    pause
    goto menu
)
echo.
echo [OK] Upload successful!
echo [*] Waiting 2 seconds for device to initialize...
timeout /t 2 /nobreak
echo.
echo [*] Step 3/3: Monitoring serial output with logging...
echo [*] Log file: %LOG_FILE%
echo [*] Press Ctrl+C to stop monitoring
echo.
echo ============================================================================
echo  LIVE CONTROLLER OUTPUT
echo ============================================================================
echo.
powershell -NoProfile -Command "arduino-cli monitor --port COM5 | Tee-Object -FilePath '%LOG_FILE%'"
echo.
echo [OK] Monitoring complete. Output saved to: %LOG_FILE%
timeout /t 2 /nobreak
goto menu

:view_log
echo.
echo [*] Searching for latest log files...
echo.
if exist "%LOG_DIR%\" (
    echo Recent log files:
    powershell -NoProfile -Command "Get-ChildItem -Path '%LOG_DIR%' -Filter '*.log' | Sort-Object LastWriteTime | Select-Object -Last 5 | ForEach-Object { Write-Host $_.Name }"
    echo.
    set /p logfile="Enter log filename to view (or press Enter to skip): "
    if not "!logfile!"=="" (
        if exist "%LOG_DIR%\!logfile!" (
            echo.
            echo ============================================================================
            echo  Viewing: !logfile!
            echo ============================================================================
            echo.
            type "%LOG_DIR%\!logfile!"
            echo.
        ) else (
            echo [ERROR] File not found: !logfile!
        )
    )
) else (
    echo [ERROR] No logs directory found. Run monitoring first to create logs.
)
echo.
pause
goto menu

:invalid
echo.
echo [ERROR] Invalid choice. Please select 1-7.
echo.
pause
goto menu

:menu
echo.
set /p cont="Run again? (y/n): "
if /i "%cont%"=="y" (
    cls
    goto :start
)
goto end

:end
echo.
echo [*] Exiting...
echo.
endlocal
exit /b 0

