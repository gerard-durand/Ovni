time /T > 00_test.txt
REM setup.h dans lib\gcc_dll\mswu\wx modifi� pour activer Direct2D
REM Mettre MinGW64\bin avant MinGW32\bin
set PATH=C:\MinGW64\bin;%PATH%
ren C:\Utilitaires_Msys-Unix\bin\sh.exe shx.exe
ren C:\msys64\usr\bin\sh.exe shx.exe
REM <=> VENDOR=cb_64
REM Ne pas mettre DEBUG_FLAG=0 sinon probl�mes � l'�dition de liens de Ovni en statique. Sinon dans Ovni ajouter le define wxDEBUG_LEVEL=0 (et peut-�tre NDEBUG)

REM *** Version librairie statique
REM echo _ >> log.txt
REM echo *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* >> log.txt
REM echo _ >> log.txt
set CB_ID=cb_64

echo %time% >> 00_test.txt
mingw32-make -f makefile.gcc -j 4 SHARED=0 MONOLITHIC=1 BUILD=release VENDOR=%CB_ID% CXXFLAGS="-std=gnu++20"
rem >>log.txt 2>&1
echo %time% >> 00_test.txt

ren C:\msys64\usr\bin\shx.exe sh.exe
ren C:\Utilitaires_Msys-Unix\bin\shx.exe sh.exe
time /T >> 00_test.txt
