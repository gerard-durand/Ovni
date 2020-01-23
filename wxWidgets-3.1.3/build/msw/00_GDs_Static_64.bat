time /T > 00_test.txt
REM setup.h dans lib\gcc_dll\mswu\wx modifié pour activer Direct2D
REM Mettre MinGW64\bin avant MinGW32\bin
set PATH=C:\MinGW64-8.1.0\bin;%PATH%
ren C:\Utilitaires_Msys-Unix\bin\sh.exe shx.exe

REM *** Version librairie statique
REM echo _ >> log.txt
REM echo *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* >> log.txt
REM echo _ >> log.txt
mingw32-make -f makefile.gcc USE_XRC=1 SHARED=0 MONOLITHIC=1 BUILD=release UNICODE=1 USE_OPENGL=1 DEBUG_LEVEL=0 VENDOR=cb_64 clean
mingw32-make -f makefile.gcc USE_XRC=1 SHARED=0 MONOLITHIC=1 BUILD=release UNICODE=1 USE_OPENGL=1 DEBUG_LEVEL=0 VENDOR=cb_64 CXXFLAGS="-fpermissive -std=gnu++11 -Wno-deprecated-declarations" LDFLAGS="-Wl,--allow-multiple-definition"
rem >>log.txt 2>&1

ren C:\Utilitaires_Msys-Unix\bin\shx.exe sh.exe
time /T >> 00_test.txt
