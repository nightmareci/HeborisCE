@if "%~1"=="" goto error

@del /s /q build-windows-msvc HeborisC7EX-SDL2 HeborisC7EX-SDL2-Windows.zip

@cmake -B build-windows-msvc -DPACKAGE_TYPE=Portable --toolchain %1 -DCMAKE_INSTALL_PREFIX=HeborisC7EX-SDL2 -DVCPKG_TARGET_TRIPLET=x64-windows
@cmake --build build-windows-msvc --config Release
@cmake --install build-windows-msvc

@tar -c -a -f HeborisC7EX-SDL2-Windows.zip HeborisC7EX-SDL2

@exit

:error
@echo Usage: .\pkg\windows\pkg.bat path\to\vcpkg.cmake