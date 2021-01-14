#!/bin/sh

cd $(dirname $0)
node -e 'require("./packager.js").embedIncludes(true)'
