@if "%~1"=="" goto usage
@if "%~2"=="" goto usage
@if "%~3"=="" goto usage


@set NAME=HeborisCE
@set SOURCE_DIRECTORY=%~1
@set BUILD_DIRECTORY=%~2
@set ARCHITECTURE=%~3


@if %ARCHITECTURE%==x64 (
@del "%BUILD_DIRECTORY%\%NAME%-Windows-x64.zip"
@echo Building for x64
@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\x64\build" -DPACKAGE_TYPE=Portable -DHIDE_WINDOWS_CONSOLE=1 -DUSE_VENDOR_LIBRARIES=1 -DSDL_LIBC=1 -A x64 -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\x64\%NAME%" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --build "%BUILD_DIRECTORY%\x64\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --install "%BUILD_DIRECTORY%\x64\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
)

@if %ARCHITECTURE%==x86 (
@del "%BUILD_DIRECTORY%\%NAME%-Windows-x86.zip"
@echo Building for x86
@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\x86\build" -DPACKAGE_TYPE=Portable -DHIDE_WINDOWS_CONSOLE=1 -DUSE_VENDOR_LIBRARIES=1 -DSDL_LIBC=1 -A Win32 -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\x86\%NAME%" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --build "%BUILD_DIRECTORY%\x86\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --install "%BUILD_DIRECTORY%\x86\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
)

@if %ARCHITECTURE%==ARM64 (
@del "%BUILD_DIRECTORY%\%NAME%-Windows-ARM64.zip"
@echo Building for ARM64
@cmake "%SOURCE_DIRECTORY%" -B "%BUILD_DIRECTORY%\ARM64\build" -DPACKAGE_TYPE=Portable -DHIDE_WINDOWS_CONSOLE=1 -DUSE_VENDOR_LIBRARIES=1 -DSDL_LIBC=1 -A ARM64 -DCMAKE_INSTALL_PREFIX="%BUILD_DIRECTORY%\ARM64\%NAME%" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --build "%BUILD_DIRECTORY%\ARM64\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
@cmake --install "%BUILD_DIRECTORY%\ARM64\build" --config Release
@if %ERRORLEVEL% NEQ 0 goto error
)


@cd "%BUILD_DIRECTORY%"
@if %ERRORLEVEL% NEQ 0 goto error

@if %ARCHITECTURE%==x64 (
@cd x64
@if %ERRORLEVEL% NEQ 0 goto error
@tar -c -a -f "..\%NAME%-Windows-x64.zip" "%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cd ..
@if %ERRORLEVEL% NEQ 0 goto error
)

@if %ARCHITECTURE%==x86 (
@cd x86
@if %ERRORLEVEL% NEQ 0 goto error
@tar -c -a -f "..\%NAME%-Windows-x86.zip" "%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cd ..
@if %ERRORLEVEL% NEQ 0 goto error
)

@if %ARCHITECTURE%==ARM64 (
@cd ARM64
@if %ERRORLEVEL% NEQ 0 goto error
@tar -c -a -f "..\%NAME%-Windows-ARM64.zip" "%NAME%"
@if %ERRORLEVEL% NEQ 0 goto error
@cd ..
@if %ERRORLEVEL% NEQ 0 goto error
)


@goto end


:usage
@echo Usage: pkg.bat [source-directory] [build-directory] [architecture]
@echo [architecture] can be x64, x86, or ARM64
@goto end


:error
@echo Error encountered
@goto end


:end
