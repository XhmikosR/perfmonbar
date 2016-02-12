#!/usr/bin/env node

/**!
 * run-tests.js, script to run csslint and JSHint for our files
 * Released under the terms of MIT license
 *
 * https://github.com/XhmikosR/perfmonbar
 *
 * Copyright (C) 2013 XhmikosR
 */

require("shelljs/global");

cd(__dirname);

//
// csslint
//
var csslintBin = "./node_modules/csslint/cli.js";

if (!test("-f", csslintBin)) {
    echo("csslint not found. Run `npm install` in the root dir first.");
    exit(1);
}

// csslint doesn't return proper error codes...
/*if (exec("node" + " " + csslintBin + " " + "css/stylesheet.css").code !== 0) {
    echo("*** csslint failed! (return code != 0)");
    echo();
}*/
exec("node" + " " + csslintBin + " " + "css/stylesheet.css");
