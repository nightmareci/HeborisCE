@if "%~1"=="" goto usage
@if "%~2"=="" goto usage
@if "%~3"=="" goto usage

@set NAME=HeborisC7EX-SDL2
@set VCPKG_TOOLCHAIN="%~1"
@set SOURCE_DIRECTORY="%~2"
@set BUILD_DIRECTORY="%~3"

@rmdir /s /q "%BUILD_DIRECTORY%"
@if %ERRORLEVEL% NEQ 0 goto error

@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\build" -DPACKAGE_TYPE=Portable --toolchain "%VCPKG_TOOLCHAIN%" -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\%NAME%" -DVCPKG_TARGET_TRIPLET=x64-windows
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --build "%BUILD_DIRECTORY%\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --install "%BUILD_DIRECTORY%\build"
@if %ERRORLEVEL% NEQ 0 goto error

@cd "%BUILD_DIRECTORY%"
@if %ERRORLEVEL% NEQ 0 goto error
@tar -c -a -f "%NAME%"-Windows.zip "%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cd ..
@if %ERRORLEVEL% NEQ 0 goto error

@rmdir /s /q "%BUILD_DIRECTORY%\build"
@if %ERRORLEVEL% NEQ 0 goto error
@rmdir /s /q "%BUILD_DIRECTORY%\%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error

@goto end

:usage
@echo Usage: pkg.bat [path-to-vcpkg.cmake] [source-directory] [build-directory]
@goto end

:error
@echo Error encountered
@goto end

:end