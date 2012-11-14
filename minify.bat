@ECHO OFF
SETLOCAL

ECHO Minifying and combining css and js files...

TYPE css\jquery.fancybox.css css\normalize.css css\stylesheet.css | cleancss --s0 -o css\pack.css
uglifyjs --no-copyright -o js\jquery.fancybox.min.js js\jquery.fancybox.js

ENDLOCAL
PAUSE