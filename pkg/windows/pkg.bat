@if "%~1"=="" goto error
@if "%~2"=="" goto error
@if "%~3"=="" goto error

@set NAME=HeborisC7EX-SDL2
@set VCPKG_TOOLCHAIN="%~1"
@set SOURCE_DIRECTORY="%~2"
@set BUILD_DIRECTORY="%~3"

@rmdir /s /q "%BUILD_DIRECTORY%"

@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\build" -DPACKAGE_TYPE=Portable --toolchain "%VCPKG_TOOLCHAIN%" -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\%NAME%" -DVCPKG_TARGET_TRIPLET=x64-windows
@cmake --build "%BUILD_DIRECTORY%\build" --config Release
@cmake --install "%BUILD_DIRECTORY%\build"

@cd "%BUILD_DIRECTORY%"
@tar -c -a -f "%NAME%"-Windows.zip "%NAME%"
@cd ..

@rmdir /s /q "%BUILD_DIRECTORY%\build"
@rmdir /s /q "%BUILD_DIRECTORY%\%NAME%"

@exit

:error
@echo Usage: pkg.bat [path-to-vcpkg.cmake] [source-directory] [build-directory]