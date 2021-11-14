echo off
rem -----------------------------------------------------------------------------------------------
rem File  : gitlab-ci-run-tests.bat
rem Date  : Thursday, Jun 11, 2020, 15:53 pm
rem Author: Kelly Thompson <kgt@lanl.gov>
rem Note  : Copyright (C) 2020, Triad National Security, LLC., All rights are reserved.
rem -----------------------------------------------------------------------------------------------

echo This is windows...
rem set

echo ---------------------------------------
echo VCVARS       = %VCVARS%
echo CMAKE_TOOLCHAIN_FILE = %CMAKE_TOOLCHAIN_FILE%
echo MINGW64PATH  = %MINGW64PATH%
echo VCPKGLOC     = %VCPKGLOC%
echo DRACO_SOURCE_DIR   = %DRACO_SOURCE_DIR%
echo DRACO_BINARY_DIR   = %DRACO_BINARY_DIR%
echo JAYENNE_SOURCE_DIR = %JAYENNE_SOURCE_DIR%
echo JAYENNE_BINARY_DIR = %JAYENNE_BINARY_DIR%
echo NUMBER_OF_PROCS    = %NUMBER_OF_PROCESSORS%
echo ---------------------------------------

rem Fix LANL proxy issues
rem - git must use a proxy (settings found in %USERPROFILE%/.gitconfig
rem - To allow cdash to post results, the environment variables HTTP[S] must not be set
git.exe config --global http.proxy http://proxyout.lanl.gov:8080
git.exe config --global https.proxy https://proxyout.lanl.gov:8080
set HTTP_PROXY=
set HTTPS_PROXY=

rem gfortran runtimes
set PATH=%PATH%;C:\msys64\mingw64\bin;=%
rem numdiff
set PATH=%PATH%;C:\work\vendors64\bin;=%

echo %PATH%

rem -----------------------------------------------------------------------------------------------
rem Environment setup and checking
rem -----------------------------------------------------------------------------------------------

:setupmsvccommenv

dir %VCVARS% > %CI_PROJECT_DIR%/glr.log
if %ERRORLEVEL% == 0 (echo   vcvars64.bat   : found) else ( goto errnovcvars )
@call "%VCVARS%" %* > %CI_PROJECT_DIR%/glr.log
dir %CMAKE_TOOLCHAIN_FILE% > %CI_PROJECT_DIR%/glr.log
if %ERRORLEVEL% == 0 (echo   toolchain.cmake: found) else ( goto errnotoolchain )
dir %VCPKGLOC%  > %CI_PROJECT_DIR%/glr.log
if %ERRORLEVEL% == 0 (echo   vcpkg.cmake    : found) else ( goto errnovcpkg )
where /q cmake
if %ERRORLEVEL% == 0 (echo   cmake          : found) else ( goto errnocmake )
where /q numdiff
if %ERRORLEVEL% == 0 (echo   numdiff        : found) else ( goto errnonumdiff )

rem -----------------------------------------------------------------------------------------------
rem Install Draco (develop branch)
rem -----------------------------------------------------------------------------------------------

:installdraco

echo .
echo Clone draco (develop) at %DRACO_SOURCE_DIR%
echo .

set HTTP_PROXY=http://proxyout.lanl.gov:8080

cd /d %CI_PROJECT_DIR%
rem LANL's self-signed certifiates are causing issues, so ignore ssl warnings.
set GIT_SSL_NO_VERIFY=true
rem https://stackoverflow.com/questions/53391229/clone-another-gitlab-repository-in-gitlab-ci-script
git clone https://gitlab-ci-token:%CI_JOB_TOKEN%@asc-git.lanl.gov/draco/draco.git

set HTTP_PROXY=

echo .
echo Switch to build directory = %DRACO_BINARY_DIR%
echo .

if not exist %DRACO_BINARY_DIR% mkdir %DRACO_BINARY_DIR%
cd /d %DRACO_BINARY_DIR%

echo cmake -G"Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" -DBUILD_TESTING=OFF -DDRACO_LIBRARY_TYPE=SHARED "%DRACO_SOURCE_DIR%"
cmake -G"Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" -DBUILD_TESTING=OFF -DDRACO_LIBRARY_TYPE=SHARED "%DRACO_SOURCE_DIR%"
if %errorlevel% NEQ 0 exit /b 255

echo cmake --build . --target install --config %CMAKE_BUILD_TYPE% -j %NUMBER_OF_PROCESSORS%
cmake --build . --target install --config %CMAKE_BUILD_TYPE% -j %NUMBER_OF_PROCESSORS%
if %errorlevel% NEQ 0 exit /b 255

rem -----------------------------------------------------------------------------------------------
rem Main tests
rem -----------------------------------------------------------------------------------------------

:maintests

echo .
echo Switch to build directory = %JAYENNE_BINARY_DIR%
echo .

if not exist %JAYENNE_BINARY_DIR% mkdir %JAYENNE_BINARY_DIR%
cd /d %JAYENNE_BINARY_DIR%

echo cmake -G"Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" -DDRACO_LIBRARY_TYPE=SHARED "%JAYENNE_SOURCE_DIR%"
cmake -G"Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" -DDRACO_LIBRARY_TYPE=SHARED "%JAYENNE_SOURCE_DIR%"
if %errorlevel% NEQ 0 exit /b 255

echo cmake --build . --target all_build --config %CMAKE_BUILD_TYPE% -j %NUMBER_OF_PROCESSORS%
cmake --build . --target all_build --config %CMAKE_BUILD_TYPE% -j %NUMBER_OF_PROCESSORS%
if %errorlevel% NEQ 0 exit /b 255

echo ctest -j %NUMBER_OF_PROCESSORS% --output-on-failure --stop-on-failure -C %CMAKE_BUILD_TYPE%
ctest -j %NUMBER_OF_PROCESSORS% --output-on-failure --stop-on-failure -C %CMAKE_BUILD_TYPE%
if %errorlevel% NEQ 0 exit /b 255

goto:eof

rem -----------------------------------------------------------------------------------------------
rem Error handling
rem -----------------------------------------------------------------------------------------------

:errnovcvars
echo FATAL ERROR :: VCVARS = %VCVARS%
echo                was not found.  This batch file is required to setup the
echo                Visual Studio build environment.
exit /b 250
goto:eof

:errnotoolchain
echo FATAL ERROR :: CMAKE_TOOLCHAIN_FILE = %CMAKE_TOOLCHAIN_FILE%
echo                was not found. This file is required.
exit /b 250
goto:eof

:errnovcpkg
echo FATAL ERROR :: VCPKGLOC = %VCPKGLOC%
echo                was not found. This file is required.
exit /b 250
goto:eof

:errnocmake
echo FATAL ERROR :: cmake was not found in PATH.
exit /b 250
goto:eof

:errnonumdiff
echo FATAL ERROR :: numdiff was not found in PATH.
exit /b 250
goto:eof

rem -----------------------------------------------------------------------------------------------
rem End .gitlab/ci/gitlab-ci-run-tests.bat
rem -----------------------------------------------------------------------------------------------
