@ECHO OFF
SETLOCAL

SET PATH=C:\MSYS\bin;%PATH%

ECHO Minifying and combining css files...

cleancss css\stylesheet.css -o css\stylesheet.min.css

ENDLOCAL
PAUSE