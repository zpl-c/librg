if [ -z ${1+x} ]; then
    echo "no test name provided, using 'main.c'";
    set $1 'main.c';
fi

mkdir -p build;
gcc -std=c99 -g -I include -I vendor test/$1 -o build/test;
./build/test
