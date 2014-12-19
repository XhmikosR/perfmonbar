/**!
 * make.js, script to build the website for Perfmonbar
 * Released under the terms of MIT license
 *
 * https://github.com/XhmikosR/perfmonbar
 *
 * Copyright (C) 2013 XhmikosR
 */

(function () {
    "use strict";

    require("shelljs/make");
    var fs = require("fs"),
        CleanCSS = require("clean-css"),
        UglifyJS = require("uglify-js");

    //
    // make minify
    //
    target.minify = function () {
        cd(__dirname);
        echo();
        echo("### Minifying css files...");

        var inCss = cat(["css/normalize.css",
                         "css/jquery.fancybox.css",
                         "css/stylesheet.css"
        ]);

        var minifier = new CleanCSS({
                keepSpecialComments: 0,
                compatibility: "ie8"
            });

        fs.writeFileSync("css/pack.css", minifier.minify(inCss).styles, "utf8");

        echo();
        echo("### Finished css/pack.css.");


        echo();
        echo("### Minifying js files...");

        var inJs = cat(["js/jquery.fancybox.js"]);

        var minifiedJs = UglifyJS.minify(inJs, {
            compress: true,
            fromString: true, // this is needed to pass JS source code instead of filenames
            mangle: true,
            warnings: false
        });

        fs.writeFileSync("js/jquery.fancybox.min.js", minifiedJs.code, "utf8");

        echo();
        echo("### Finished js/jquery.fancybox.min.js.");
    };


    //
    // make all
    //
    target.all = function () {
        target.minify();
    };

    //
    // make help
    //
    target.help = function () {
        echo("Available targets:");
        echo("  minify  Creates the minified CSS and JS");
        echo("  help    shows this help message");
    };

}());
