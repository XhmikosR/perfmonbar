@ECHO OFF
SETLOCAL

SET PATH=C:\MSYS\bin;%PATH%

ECHO Minifying and combining css and js files...

cat css\stylesheet.css css\jquery.fancybox.css | cleancss --s0 -o css\pack.css && ^
uglifyjs --no-copyright -o js\jquery.fancybox.min.js js\jquery.fancybox.js

ENDLOCAL
PAUSE