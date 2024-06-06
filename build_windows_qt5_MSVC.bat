@echo off

REM Date     Version   Author                Changes

echo ************************************
echo ***     DLT Sim                  ***
echo ************************************
echo ************************************
echo ***         Configuration        ***
echo ************************************

set NAME=dlt-sim
set TARGET_NAME=%NAME%

rem parameter of this batch script can be either x86 or x86_amd64
if "%ARCHITECTURE%"=="" (
    if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
        set ARCHITECTURE=x86_amd64
    ) else (
        set ARCHITECTURE=x86
    )

    set USE_ARCH_PARAM=false
    if "%1" NEQ "" (
        if "%1"=="x86" set USE_ARCH_PARAM=true
        if "%1"=="x86_amd64" set USE_ARCH_PARAM=true
    )
    if "!USE_ARCH_PARAM!"=="true" set ARCHITECTURE=%1
)

echo Target architecture is %ARCHITECTURE%

echo *** Setting up environment ***

if "%QTDIR%"=="" (
    if "%ARCHITECTURE%"=="x86_amd64" (
        set QTDIR=C:\Qt\5.15.2\msvc2019_64
    ) else (set QTDIR=C:\Qt\5.15.2\msvc2019)
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\%MSVC_VER%\Enterprise\VC\Auxiliary\Build" (
    REM Visual Studio Community Edition 2019
	if "%MSVC_DIR%"=="" set "MSVC_DIR=C:\Program Files (x86)\Microsoft Visual Studio\%MSVC_VER%\Enterprise\VC\Auxiliary\Build"
) else (
    REM Vidual Studio Professional 2019
	if "%MSVC_DIR%"=="" set "MSVC_DIR=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build"
)

set PATH=%QTDIR%\bin;%MSVC_DIR%;%MSVC_DIR%\bin;%PATH%
set QTSDK=%QTDIR%

if '%INSTALLATION_DIR%'=='' (
	set INSTALLATION_DIR=c:\%TARGET_NAME%
)

set SOURCE_DIR=%CD%
set BUILD_DIR=%CD%\build

echo ************************************
echo * QTDIR     = %QTDIR%
echo * QTSDK     = %QTSDK%
echo * QWT_DIR   = %QWT_DIR%
echo * MSVC_DIR  = %MSVC_DIR%
echo * PATH      = %PATH%
echo * INSTALLATION_DIR = %INSTALLATION_DIR%
echo * SOURCE_DIR         = %SOURCE_DIR%
echo * BUILD_DIR          = %BUILD_DIR%
echo ************************************

if exist build (
echo ************************************
echo ***  Delete old build Directory  ***
echo ************************************

    rmdir /s /q build
    if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

)

echo ************************************
echo ***  Configure MSVC environment  ***
echo ************************************

call vcvarsall.bat %ARCHITECTURE%
if %ERRORLEVEL% NEQ 0 goto error
echo configuring was successful

if exist %INSTALLATION_DIR% (
echo ************************************
echo ***   Delete old Directory       ***
echo ************************************

    rmdir /s /q %INSTALLATION_DIR%
    if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
)

echo ************************************
echo ***       Build                  ***
echo ************************************

mkdir build
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

cd build
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

qmake ../%NAME%.pro
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

nmake
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

echo ************************************
echo ***         Create SDK           ***
echo ************************************

echo *** Create directories %INSTALLATION_DIR% ***

if not exist %INSTALLATION_DIR% mkdir %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\platforms
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\doc
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\doc\DLTRelaisPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\doc\DLTCanPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\doc\DLTMultimeterPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\doc\DLTPowerPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\doc\DLTTestRobotPluginPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTRelaisPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTRelais\PluginWemosD1MiniRelais3Button
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTRelais\PluginWemosD1Relais4Shield
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTCanPlugin
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTCanPlugin\WemosD1MiniCAN
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTCanPlugin\WemosD1R1CANDiymore
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\arduino\DLTCanPlugin\WemosD1R1CANKeyestudio
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\plugins
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

mkdir %INSTALLATION_DIR%\custom_plugins
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

echo *** Copy files ***
copy %QTDIR%\bin\Qt5Core.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5Gui.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5Network.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5Svg.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5Widgets.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5PrintSupport.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5Xml.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5OpenGL.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\bin\Qt5SerialPort.dll %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %QTDIR%\plugins\platforms\qwindows.dll %INSTALLATION_DIR%\platforms
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\plugins\DLTRelaisPlugin\arduino\WemosD1MiniRelais3Button\WemosD1MiniRelais3Button.ino %INSTALLATION_DIR%\arduino\DLTRelaisPlugin\WemosD1MiniRelais3Button
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\plugins\DLTRelaisPlugin\arduino\WemosD1Relais4Shield\WemosD1Relais4Shield.ino %INSTALLATION_DIR%\arduino\DLTRelaisPlugin\WemosD1Relais4Shield
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\plugins\DLTCanPlugin\arduino\WemosD1MiniCAN\WemosD1MiniCAN.ino %INSTALLATION_DIR%\arduino\DLTCanPlugin\WemosD1MiniCAN
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\plugins\DLTCanPlugin\arduino\WemosD1R1CANDiymore\WemosD1R1CANDiymore.ino %INSTALLATION_DIR%\arduino\DLTCanPlugin\WemosD1R1CANDiymore
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\plugins\DLTCanPlugin\arduino\WemosD1R1CANKeyestudio\WemosD1R1CANKeyestudio.ino %INSTALLATION_DIR%\arduino\DLTCanPlugin\WemosD1R1CANKeyestudio
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %BUILD_DIR%\plugins\*.dll %INSTALLATION_DIR%\plugins
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %BUILD_DIR%\custom_plugins\*.dll %INSTALLATION_DIR%\custom_plugins
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %BUILD_DIR%\%NAME%.exe %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\README.md %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

copy %SOURCE_DIR%\LICENSE %INSTALLATION_DIR%
if %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

GOTO QUIT

:ERROR_HANDLER
echo ####################################
echo ###       ERROR occured          ###
echo ####################################
set /p name= Continue
exit 1


:QUIT
echo ************************************
echo ***       SUCCESS finish         ***
echo ************************************
echo Installed in: %INSTALLATION_DIR%
set /p name= Continue
exit 0
