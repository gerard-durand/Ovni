time /T > 00_test.txt
REM Mettre MinGW64\bin avant MinGW32\bin
set PATH=C:\MinGW64-8.1.0\bin;%PATH%
ren C:\Utilitaires_Msys-Unix\bin\sh.exe shx.exe
REM les strip ne marchent que si VENDOR=cb_64, sinon les adapter

REM *** Version dll monolitique (-fno-keep-inline-dllexport indispensable pour gcc >= 4.6, mais uniquement sur version dll monolitique apparemment)
REM Ajouter DEBUG_FLAG=0 n'est pas compatible avec C::B apparemment !
REM -std=gnu++11 requis par tdm 4.9.2 pour wxWidgets 3.0.2 (c++11 n'est pas suffisant)
mingw32-make -f makefile.gcc USE_XRC=1 SHARED=1 MONOLITHIC=1 BUILD=release UNICODE=1 USE_OPENGL=1 DEBUG_LEVEL=0 VENDOR=cb_64 CXXFLAGS="-fpermissive -fno-keep-inline-dllexport -std=gnu++11 -Wno-deprecated-declarations" LDFLAGS="-Wl,--allow-multiple-definition"
rem >log.txt 2>&1
strip ..\..\lib\gcc_dll\wxmsw313u_gcc_cb_64.dll
strip ..\..\lib\gcc_dll\wxmsw313u_gl_gcc_cb_64.dll

ren C:\Utilitaires_Msys-Unix\bin\shx.exe sh.exe
time /T >> 00_test.txt
