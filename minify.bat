@ECHO OFF
SETLOCAL

ECHO Minifying and combining css and js files...

TYPE css\jquery.fancybox.css css\normalize.css css\stylesheet.css | cleancss --s0 -o css\pack.css
cmd /c uglifyjs js\jquery.fancybox.js --compress --mangle -o js\jquery.fancybox.min.js

ENDLOCAL
rem PAUSE
