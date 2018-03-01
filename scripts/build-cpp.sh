g++ -g -std=c++11 \
    -I include \
    -I node_modules/zpl.c/code \
    -I node_modules/enet.c/include \
    $(sdl2-config --cflags --libs) \
    test/$1.cpp \
    -o build/$1 && build/$1
