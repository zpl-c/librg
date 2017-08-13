clang -g \
    -I include \
    -I vendor/zpl/include \
    -I vendor/enet/include \
    test/$1.c \
    vendor/enet/callbacks.c \
    vendor/enet/compress.c \
    vendor/enet/host.c \
    vendor/enet/list.c \
    vendor/enet/packet.c \
    vendor/enet/peer.c \
    vendor/enet/protocol.c \
    vendor/enet/win32.c \
    vendor/enet/unix.c \
    -o build/$1 && build/$1
