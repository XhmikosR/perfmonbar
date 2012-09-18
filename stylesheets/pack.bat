@ECHO OFF
SETLOCAL

SET PATH=C:\MSYS\bin;%PATH%

ECHO Minifying and combining css files...

cat pygment_trac.css stylesheet.css | cssc > pack.css
rem cssc > pygment_trac.min.css pygment_trac.css && ^
rem cssc > stylesheet.min.css stylesheet.css

ENDLOCAL
PAUSE