@if "%~1"=="" goto usage
@if "%~2"=="" goto usage
@if "%~3"=="" goto usage


@set NAME=HeborisC7EX-SDL2
@set VCPKG_TOOLCHAIN="%~1"
@set SOURCE_DIRECTORY="%~2"
@set BUILD_DIRECTORY="%~3"


@rmdir /s /q "%BUILD_DIRECTORY%"
@if %ERRORLEVEL% NEQ 0 goto error


@echo Building for x64
@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\x64\build" -DPACKAGE_TYPE=Portable -DHIDE_WINDOWS_CONSOLE=1 --toolchain "%VCPKG_TOOLCHAIN%" -DVCPKG_TARGET_TRIPLET=x64-windows -A x64 -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\x64\%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --build "%BUILD_DIRECTORY%\x64\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --install "%BUILD_DIRECTORY%\x64\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error

@echo Building for x86
@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\x86\build" -DPACKAGE_TYPE=Portable -DHIDE_WINDOWS_CONSOLE=1 --toolchain "%VCPKG_TOOLCHAIN%" -DVCPKG_TARGET_TRIPLET=x86-windows -A Win32 -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\x86\%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --build "%BUILD_DIRECTORY%\x86\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --install "%BUILD_DIRECTORY%\x86\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error


@cd "%BUILD_DIRECTORY%"
@if %ERRORLEVEL% NEQ 0 goto error

@cd x64
@if %ERRORLEVEL% NEQ 0 goto error
@tar -c -a -f "..\%NAME%-Windows-x64.zip" "%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cd ..
@if %ERRORLEVEL% NEQ 0 goto error

@cd x86
@if %ERRORLEVEL% NEQ 0 goto error
@tar -c -a -f "..\%NAME%-Windows-x86.zip" "%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cd ..
@if %ERRORLEVEL% NEQ 0 goto error

@rmdir /s /q x64
@if %ERRORLEVEL% NEQ 0 goto error

@rmdir /s /q x86
@if %ERRORLEVEL% NEQ 0 goto error


@goto end


:usage
@echo Usage: pkg.bat [path-to-vcpkg.cmake] [source-directory] [build-directory]
@goto end


:error
@echo Error encountered
@goto end


:end