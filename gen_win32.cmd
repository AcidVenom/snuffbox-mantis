@echo off
setlocal enabledelayedexpansion

echo [92msnuffbox-mantis generation tool, please make sure 'git' is installed and accessible from the command line[0m & echo.

if not exist .\deps (
	mkdir .\deps
	echo Created 'deps' folder & echo.
)

cd deps

set /P geteastl="Clone EASTL? (y/n)"
echo.
if /I "%geteastl%" neq "y" goto skipeastl

if exist .\eastl (
	rmdir .\eastl /s /q
	echo Removing old 'eastl' folder as we're regenerating & echo.
)

mkdir .\eastl
echo [32mMade the 'eastl' folder[0m
echo [32mCloning EASTL from git[0m & echo.

call git clone https://github.com/electronicarts/EASTL eastl & echo.

:skipeastl
set /P getglfw="Clone GLFW? (y/n)"
echo.
if /I "%getglfw%" neq "y" goto skipglfw

if exist .\glfw (
	rmdir .\glfw /s /q
	echo Removing old 'glfw' folder as we're regenerating & echo.
)

mkdir .\glfw
echo [32mMade the 'glfw' folder[0m
echo [32mCloning GLFW from git[0m & echo.

call git clone https://github.com/glfw/glfw.git glfw & echo.
cd glfw
call git checkout latest & echo.
cd ../

:skipglfw
set /P getglm="Clone GLM? (y/n)"
echo.
if /I "%getglm%" neq "y" goto skipglm

if exist .\glm (
	rmdir .\glm /s /q
	echo Removing old 'glm' folder as we're regenerating & echo.
)

mkdir .\glm
echo [32mMade the 'glm' folder[0m
echo [32mCloning GLM from git[0m & echo.

call git clone https://github.com/g-truc/glm glm & echo.

:skipglm
echo To download and build V8, depot_tools is required in your system PATH
echo [91mBuilding V8 takes quite some time and is only required with SNUFF_JAVASCRIPT[0m & echo.

set /P buildV8="Download & build V8? (y/n)"
if /I "%buildV8%" neq "y" goto skipv8

set DEPOT_TOOLS_WIN_TOOLCHAIN=0
set GYP_MSVS_VERSION=2015

echo.
echo Start at:
echo 1. Fetch V8
echo 2. Build V8 & echo.
 
choice /c 12 /m "Enter your choice:"
 
if errorlevel 2 (goto buildv8)
if errorlevel 1 (goto fetchv8)

:fetchv8
echo.
if exist .\v8 (
	rmdir .\v8 /s /q
	del /q /f .gclient
	del /q /f .gclient_entries
	echo Removing old 'v8' folder as we're regenerating
)

echo [32mFetching v8 with gclient[0m

call gclient & echo.
call fetch v8 & echo.
cd v8
call gclient sync & echo.
cd ../

:buildv8
echo.
cd v8

call python "tools/dev/v8gen.py" x64.debug
call python "tools/dev/v8gen.py" x64.release

cd ../

copy /Y ..\snuffbox-javascript\gn\x64.debug.win32.gn .\v8\out.gn\x64.debug\args.gn
copy /Y ..\snuffbox-javascript\gn\x64.release.win32.gn .\v8\out.gn\x64.release\args.gn & echo.

cd v8

call gn gen out.gn/x64.debug
call gn gen out.gn/x64.release & echo.

set /P compileV8="Compile V8? (y/n)"
if /I "%compileV8%" neq "y" goto skipcompilev8

call ninja -C "out.gn/x64.debug"
call ninja -C "out.gn/x64.release"

:skipcompilev8
cd ../

:skipv8

echo.
set /P getwx="Download & build wxWidgets? (y/n)"
echo.
if /I "%getwx%" neq "y" goto skipwx

echo.
echo Start at:
echo 1. Download wxWidgets with wget
echo 2. Build wxWidgets & echo.
 
choice /c 12 /m "Enter your choice:"
 
if errorlevel 2 (goto buildwx)
if errorlevel 1 (goto clonewx)

:clonewx
if exist .\wxWidgets (
	rmdir .\wxWidgets /s /q
	del /q /f wxWidgets.zip
	echo Removing old 'wxWidgets' folder as we're regenerating & echo.
)

echo [32mDownloading wxWidgets with wget[0m & echo.

call "../wget.exe" -O wxWidgets.zip https://github.com/wxWidgets/wxWidgets/releases/download/v3.0.3/wxWidgets-3.0.3.zip

echo.
echo [32mExtracting 'wxWidgets.zip' into 'wxWidgets'[0m

call powershell.exe -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('wxWidgets.zip', 'wxWidgets'); }"
del /q /f wxWidgets.zip

echo.
set /P compilewx="Compile wxWidgets? (y/n)"
if /I "%compilewx%" neq "y" goto skipwx

:buildwx

if "%VS_ROOT%"=="" (
	set "vsRoot=C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE"
	echo Set Visual Studio root directory to: '!vsRoot!', add 'VS_ROOT' to your environment variables to override
) else (
	set "vsRoot=%VS_ROOT%"
)

set "vsOpen=!vsRoot!\devenv.exe"
set "vsBuild=!vsRoot!\devenv.com"
set "wxPath=wxWidgets\build\msw\wx_vc12.sln"

echo [91mBefore compiling wxWidgets, please set all projects under 'Debug - x64' and 'Release - x64' to /MTd and /MT respectively[0m & echo.

call "!vsOpen!" "!wxPath!"

call "!vsBuild!" "!wxPath!" /build "Debug|x64"
call "!vsBuild!" "!wxPath!" /build "Release|x64"

:skipwx

echo.
cd ../
echo [92mAll done generating[0m

set /p dummy=Hit 'Enter' to continue..
