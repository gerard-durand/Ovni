time /T > 00_test.txt
REM setup.h dans lib\gcc_dll\mswu\wx modifié pour activer Direct2D
REM Mettre MinGW32\bin avant MinGW64\bin
set PATH=C:\MinGW32\bin;%PATH%
REM ren C:\Utilitaires_Msys-Unix\bin\sh.exe shx.exe
ren C:\msys64\usr\bin\sh.exe shx.exe
REM <=> VENDOR=cb_64

REM *** Version dll monolitique (-fno-keep-inline-dllexport indispensable pour gcc >= 4.6, mais uniquement sur version dll monolitique apparemment)
REM echo _ >> log.txt
REM echo *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* >> log.txt
REM echo _ >> log.txt
set CB_ID=cb

echo %time% >> 00_test.txt
mingw32-make -f makefile.gcc -j 4 SHARED=1 MONOLITHIC=1 BUILD=release CFG="_32" VENDOR=%CB_ID% CFLAGS="-m32" CPPFLAGS="-m32" CXXFLAGS="-std=gnu++20" LDFLAGS="-m32"
rem >log.txt 2>&1
echo %time% >> 00_test.txt
strip ..\..\lib\gcc_dll_32\wxmsw32u_gcc_%CB_ID%.dll
strip ..\..\lib\gcc_dll_32\wxmsw32u_gl_gcc_%CB_ID%.dll
echo %time% >> 00_test.txt

ren C:\msys64\usr\bin\shx.exe sh.exe
REM ren C:\Utilitaires_Msys-Unix\bin\shx.exe sh.exe
time /T >> 00_test.txt
