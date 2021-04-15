time /T > 00_test.txt
REM setup.h dans lib\gcc_dll\mswu\wx modifié pour activer Direct2D
REM Mettre MinGW64\bin avant MinGW32\bin
set PATH=C:\MinGW64\bin;%PATH%
ren C:\Utilitaires_Msys-Unix\bin\sh.exe shx.exe
REM <=> VENDOR=cb_64

REM *** Version dll monolitique (-fno-keep-inline-dllexport indispensable pour gcc >= 4.6, mais uniquement sur version dll monolitique apparemment)
REM echo _ >> log.txt
REM echo *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* >> log.txt
REM echo _ >> log.txt
set CB_ID=cb_64

echo %time% >> 00_test.txt
mingw32-make -f makefile.gcc -j 4 USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=release UNICODE=1 USE_OPENGL=1 VENDOR=%CB_ID% clean
echo %time% >> 00_test.txt
mingw32-make -f makefile.gcc -j 4 USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=release UNICODE=1 USE_OPENGL=1 VENDOR=%CB_ID% CXXFLAGS="-fpermissive -fno-keep-inline-dllexport -std=gnu++20 -Wno-deprecated-declarations" LDFLAGS="-Wl,--allow-multiple-definition"
rem >log.txt 2>&1
echo %time% >> 00_test.txt
strip ..\..\lib\gcc_dll\wxmsw315u_gcc_%CB_ID%.dll
strip ..\..\lib\gcc_dll\wxmsw315u_gl_gcc_%CB_ID%.dll
echo %time% >> 00_test.txt

ren C:\Utilitaires_Msys-Unix\bin\shx.exe sh.exe
time /T >> 00_test.txt
