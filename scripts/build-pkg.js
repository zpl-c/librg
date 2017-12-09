const fs = require('fs')

const libs = [
    [ "enet.c", "enet.h" ],
    [ "zpl.c", "zpl.h" ],
    [ "zpl_cull.c", "zpl_cull.h" ],
    [ "zpl_event.c", "zpl_event.h" ],
    [ "zpl_math.c", "zpl_math.h" ],
]

const files = [
    'CMakeLists.txt',
    'LICENSE',
]

const pkg = './pkg/'
const pkg_includes = './pkg/librg/'

// recreate pkg dir
fs.rmdirSync(pkg)
fs.mkdirSync(pkg)
fs.mkdirSync(pkg_includes)

// copy header files
libs.map(lib => fs.copyFileSync(
    './node_modules/' + lib[0] + '/include/' + lib[1],
    pkg_includes + lib[1]
))

// copy librg
fs.copyFileSync('./include/librg.h', pkg_includes + 'librg.h')

// copy files else
files.map(file => fs.copyFileSync(file, pkg + file))

