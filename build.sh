clang -g \
    -I include \
    -I node_modules/zpl.c/include \
    -I node_modules/zpl_ent.c/include \
    -I node_modules/zpl_math.c/include \
    -I node_modules/zpl_event.c/include \
    -I node_modules/enet.c/include \
    test/$1.c \
    node_modules/enet.c/callbacks.c \
    node_modules/enet.c/compress.c \
    node_modules/enet.c/host.c \
    node_modules/enet.c/list.c \
    node_modules/enet.c/packet.c \
    node_modules/enet.c/peer.c \
    node_modules/enet.c/protocol.c \
    node_modules/enet.c/win32.c \
    node_modules/enet.c/unix.c \
    -o build/$1 && build/$1
