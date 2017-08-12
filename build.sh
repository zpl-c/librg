clang -g -I vendor/zpl/include -I vendor/enet/include -I include  test/$1.c -o build/$1 && build/$1
