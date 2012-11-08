@ECHO OFF
SETLOCAL

ECHO Minifying and combining css and js files...

TYPE css\stylesheet.css css\jquery.fancybox.css | cleancss --s0 -o css\pack.css
uglifyjs --no-copyright -o js\jquery.fancybox.min.js js\jquery.fancybox.js

ENDLOCAL
PAUSE