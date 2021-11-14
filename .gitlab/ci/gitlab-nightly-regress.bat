echo off
rem -----------------------------------------------------------------------------------------------
rem File  : gitlab-nightly-regress.bat
rem Date  : Friday, Aug 14, 2020, 14:20 pm
rem Author: Kelly Thompson <kgt@lanl.gov>
rem Note  : Copyright (C) 2020, Triad National Security, LLC., All rights are reserved.
rem -------------------------------------------------------------------------------------------------------------------

echo This is windows...
rem set

echo ---------------------------------------
echo VCVARS       = %VCVARS%
echo CMAKE_TOOLCHAIN_FILE = %CMAKE_TOOLCHAIN_FILE%
echo MINGW64PATH  = %MINGW64PATH%
echo VCPKGLOC     = %VCPKGLOC%
echo CSK_SOURCE_DIR = %CSK_SOURCE_DIR%
echo CSK_BINARY_DIR = %CSK_BINARY_DIR%
echo ---------------------------------------
rem -----------------------------------------------------------------------------------------------
rem Environment setup and checking
rem -----------------------------------------------------------------------------------------------

:setupmsvccommenv

dir %VCVARS% > %TEMP%/glr.log
if %ERRORLEVEL% == 0 (echo   vcvars64.bat   : found) else ( goto errnovcvars )
@call "%VCVARS%" %* > %TEMP%/glr.log
dir %CMAKE_TOOLCHAIN_FILE% > %TEMP%/glr.log
if %ERRORLEVEL% == 0 (echo   toolchain.cmake: found) else ( goto errnotoolchain )
dir %VCPKGLOC%  > %TEMP%/glr.log
if %ERRORLEVEL% == 0 (echo   vcpkg.cmake    : found) else ( goto errnovcpkg )
where /q cmake
if %ERRORLEVEL% == 0 (echo   cmake          : found) else ( goto errnocmake )

rem -----------------------------------------------------------------------------------------------
rem Main tests
rem -----------------------------------------------------------------------------------------------

:maintests

echo ctest -VV -C %CMAKE_BUILD_TYPE% -S %CSK_SOURCE_DIR%\.gitlab\ci\csk-nightly.cmake
ctest -VV -C %CMAKE_BUILD_TYPE% -S %CSK_SOURCE_DIR%\.gitlab\ci\csk-nightly.cmake

goto :done

rem -----------------------------------------------------------------------------------------------
rem Error handling
rem -----------------------------------------------------------------------------------------------

:errnovcvars
echo FATAL ERROR :: VCVARS = %VCVARS%
echo                was not found.  This batch file is required to setup the
echo                Visual Studio build environment.
goto:eof

:errnotoolchain
echo FATAL ERROR :: CMAKE_TOOLCHAIN_FILE = %CMAKE_TOOLCHAIN_FILE%
echo                was not found. This file is required.
goto:eof

:errnovcpkg
echo FATAL ERROR :: VCPKGLOC = %VCPKGLOC%
echo                was not found. This file is required.
goto:eof

:errnocmake
echo FATAL ERROR :: cmake was not found in PATH.
goto:eof

:done
echo
echo ======== end gitlab-nightly-regress.bat ==========
echo

rem -----------------------------------------------------------------------------------------------
rem End .gitlab/ci/gitlab-nightly-regress.bat
rem -----------------------------------------------------------------------------------------------
