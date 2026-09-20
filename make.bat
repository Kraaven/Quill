@echo off
setlocal EnableDelayedExpansion

:: ============================================
:: make.bat for Quill
:: 
:: Usage:
::   make            → build + run
::   make build      → build + run
::   make run        → run existing binary
::   make clean      → delete Build folder
::   make reconfigure→ force reconfigure
:: ============================================

set "BUILD_DIR=Build"

:: NOTE: uses the default generator (Visual Studio) - it auto-detects MSVC
:: from any shell. Do NOT force -G Ninja here: Ninja can't find cl.exe
:: outside a VS developer prompt and leaves a broken cache on failure.
:: A real `make` (GNU Make + Makefile) supersedes this file - see Makefile.

:: Default action
if "%~1"=="" (
    set "ACTION=build"
) else (
    set "ACTION=%~1"
)

:: --------------------------------------------
:: clean
:: --------------------------------------------
if /i "%ACTION%"=="clean" (
    if exist "%BUILD_DIR%" (
        echo [make] Cleaning...
        rmdir /s /q "%BUILD_DIR%"
        echo [make] Done.
    ) else (
        echo [make] Nothing to clean.
    )
    exit /b 0
)

:: --------------------------------------------
:: reconfigure
:: --------------------------------------------
if /i "%ACTION%"=="reconfigure" (
    if exist "%BUILD_DIR%" (
        rmdir /s /q "%BUILD_DIR%"
    )
    echo [make] Configuring with %GENERATOR%...
    cmake -S . -B %BUILD_DIR% -G %GENERATOR%
    exit /b %errorlevel%
)

:: --------------------------------------------
:: Configure if needed
:: --------------------------------------------
if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo [make] Configuring with %GENERATOR%...
    cmake -S . -B %BUILD_DIR% -G %GENERATOR%
    if errorlevel 1 (
        echo [make] Configuration failed!
        exit /b 1
    )
)

:: --------------------------------------------
:: build (build + run)
:: --------------------------------------------
if /i "%ACTION%"=="build" (
    echo [make] Building and running...
    cmake --build %BUILD_DIR% --target build-and-run
    exit /b %errorlevel%
)

:: --------------------------------------------
:: run
:: --------------------------------------------
if /i "%ACTION%"=="run" (
    echo [make] Running...
    cmake --build %BUILD_DIR% --target run
    exit /b %errorlevel%
)

:: Unknown command
echo.
echo Available commands:
echo   make              Build and run
echo   make build        Build and run
echo   make run          Run existing binary
echo   make clean        Delete Build folder
echo   make reconfigure  Force reconfigure
echo.
exit /b 1
