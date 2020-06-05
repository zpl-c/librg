/* this is a client-server demo for librg with enet */
/* it is inteded to show how librg can be used for managing a simple world */
/* it consists of 2 parts */
/* 1. server side, runs a server, hosts a world, creates a entity for each new client, handles position changes */
/* 2. client size, runs a client, connects to the server, gets updates about other entities, and sends updates about its own entity */
/* application hosts a server and MAX_CLIENTS amount of clients from within a single thread, runs for N ticks, and then exits */
/* client side will be constantly printing messages about entities being created or removed, that behavior is inteded */
/* it demonstrates how entities that are changing positions on the server, getiing in or out of the each other view range */

#define LIBRG_IMPL
#include "librg.h"

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#define ENET_IMPLEMENTATION
#include "vendor/enet.h"

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif

#define MAX_CLIENTS 4
typedef struct { float x, y, z; } vec3;

// =======================================================================//
// !
// ! Server-side
// !
// =======================================================================//

librg_world *server_world = NULL;

int32_t server_write_update(librg_world *w, librg_event *e) {
    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);

    /* prevent sending updates to users who own that entity */
    /* since they will be responsible on telling where that entity is supposed to be */
    if (librg_entity_owner_get(w, entity_id) == owner_id) {
        return LIBRG_WRITE_REJECT;
    }

    /* read our current position */
    ENetPeer *peer = (ENetPeer *)librg_entity_userdata_get(w, entity_id);

    char *buffer = librg_event_buffer_get(w, e);
    size_t max_length = librg_event_size_get(w, e);

    /* check if we have enough space to write and valid position */
    if (sizeof(vec3) > max_length || !peer->data) {
        return LIBRG_WRITE_REJECT;
    }

    /* write data and return how much we've written */
    memcpy(buffer, peer->data, sizeof(vec3));
    return sizeof(vec3);
}

int32_t server_read_update(librg_world *w, librg_event *e) {
    int64_t entity_id = librg_event_entity_get(w, e);
    size_t actual_length = librg_event_size_get(w, e);

    if (actual_length != sizeof(vec3)) {
        printf("[server] Invalid data size coming from client\n");
        return 0;
    }

    ENetPeer *peer = (ENetPeer *)librg_entity_userdata_get(w, entity_id);
    char *buffer = librg_event_buffer_get(w, e);

    /* read and update actual position */
    vec3 position = {0};
    memcpy(peer->data, buffer, actual_length);
    memcpy(&position, buffer, actual_length);

    /* and update librg actual chunk id */
    librg_chunk chunk = librg_chunk_from_realpos(w, position.x, position.y, position.z);
    librg_entity_chunk_set(w, entity_id, chunk);

    return 0;
}

int server_start(int port) {
    ENetAddress address = {0};

    address.host = ENET_HOST_ANY; /* Bind the server to the default localhost.     */
    address.port = port; /* Bind the server to port . */

    /* create a server */
    ENetHost *server = enet_host_create(&address, MAX_CLIENTS, 2, 0, 0);

    if (server == NULL) {
        printf("[server] An error occurred while trying to create an ENet server host.\n");
        return 1;
    }

    printf("[server] Started an ENet server...\n");
    server_world = librg_world_create();

    if (server_world == NULL) {
        printf("[server] An error occurred while trying to create a server world.\n");
        return 1;
    }

    printf("[server] Created a new server world\n");

    /* store our host to the userdata */
    librg_world_userdata_set(server_world, server);

    /* config our world grid */
    librg_config_chunksize_set(server_world, 16, 16, 16);
    librg_config_chunkamount_set(server_world, 9, 9, 9);
    librg_config_chunkoffset_set(server_world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    librg_event_set(server_world, LIBRG_WRITE_UPDATE, server_write_update);
    librg_event_set(server_world, LIBRG_READ_UPDATE, server_read_update);

    return 0;
}

int server_update() {
    if (!librg_world_valid(server_world))
        return 1;

    ENetHost *server = (ENetHost *)librg_world_userdata_get(server_world);
    ENetEvent event = {0};

    while (enet_host_service(server, &event, 2) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                printf("[server] A new user %d connected.\n", event.peer->incomingPeerID);
                int64_t entity_id = event.peer->incomingPeerID;

                /* we create an entity for our client */
                /* in our case it is going to have same id as owner id */
                /* since we do not really plan on adding more entities per client for now */
                /* and place his entity right in the centerl of the world */
                librg_entity_track(server_world, entity_id);
                librg_entity_owner_set(server_world, entity_id, event.peer->incomingPeerID);
                librg_entity_chunk_set(server_world, entity_id, 1);
                librg_entity_radius_set(server_world, entity_id, 2); /* 2 chunk radius visibility */
                librg_entity_userdata_set(server_world, entity_id, event.peer); /* save ptr to peer */

                /* allocate and store entity position in the data part of peer */
                vec3 entity_position = {0};
                event.peer->data = malloc(sizeof(vec3));
                *((vec3*)event.peer->data) = entity_position;

            } break;
            case ENET_EVENT_TYPE_DISCONNECT:
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
                printf("[server]  A user %d disconnected.\n", event.peer->incomingPeerID);
                int64_t entity_id = event.peer->incomingPeerID;
                librg_entity_untrack(server_world, entity_id);
                free(event.peer->data);
            } break;

            case ENET_EVENT_TYPE_RECEIVE: {
                /* handle a newly received event */
                librg_world_read(
                    server_world,
                    event.peer->incomingPeerID,
                    (char *)event.packet->data,
                    event.packet->dataLength,
                    NULL
                );

                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);
            } break;

            case ENET_EVENT_TYPE_NONE: break;
        }
    }

    /* iterate peers and send them updates */
    ENetPeer *currentPeer;
    for (currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
        if (currentPeer->state != ENET_PEER_STATE_CONNECTED) {
            continue;
        }

        char buffer[1024] = {0};
        size_t buffer_length = 1024;

        /* serialize peer's the world view to a buffer */
        librg_world_write(
            server_world,
            currentPeer->incomingPeerID,
            buffer,
            &buffer_length,
            NULL
        );

        /* create packet with actual length, and send it */
        ENetPacket *packet = enet_packet_create(buffer, buffer_length, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(currentPeer, 0, packet);
    }

    return 0;
}

int server_stop() {
    if (!librg_world_valid(server_world))
        return 1;

    ENetHost *server = (ENetHost *)librg_world_userdata_get(server_world);

    enet_host_destroy(server);
    librg_world_destroy(server_world);
    server_world = NULL;

    return 0;
}

// =======================================================================//
// !
// ! Client-side
// !
// =======================================================================//

int32_t client_read_create(librg_world *w, librg_event *e) {
    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);
    printf("[client] An entity %d was created for owner: %d\n", (int)entity_id, (int)owner_id);
    return 0;
}

int32_t client_read_remove(librg_world *w, librg_event *e) {
    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);
    printf("[client] An entity %d was removed for owner: %d\n", (int)entity_id, (int)owner_id);
    return 0;
}

int32_t client_read_update(librg_world *w, librg_event *e) {
    // int64_t entity_id = librg_event_entity_get(w, e);
    size_t actual_length = librg_event_size_get(w, e);
    char *buffer = librg_event_buffer_get(w, e);
    if (actual_length != sizeof(vec3)) return 0;

    vec3 position = {0};
    memcpy(&position, buffer, actual_length);

    return 0;
}

int32_t client_write_update(librg_world *w, librg_event *e) {
    // int64_t entity_id = librg_event_entity_get(w, e);
    // int64_t owner_id = librg_event_owner_get(w, e);
    char *buffer = librg_event_buffer_get(w, e);
    size_t max_length = librg_event_size_get(w, e);

    /* check if we have enough space to write and valid position */
    if (sizeof(vec3) > max_length) {
        return LIBRG_WRITE_REJECT;
    }

    zpl_random r = {0};
    zpl_random_init(&r);
    vec3 position = {0};

    /* write our new random position */
    u32 x = (zpl_random_gen_u32(&r)%40); position.x = (float)(x-20.0f);
    u32 y = (zpl_random_gen_u32(&r)%40); position.y = (float)(y-20.0f);
    u32 z = (zpl_random_gen_u32(&r)%40); position.z = (float)(z-20.0f);

    memcpy(buffer, &position, sizeof(vec3));
    return sizeof(vec3);
}

librg_world *client_start(int port) {
    ENetAddress address = {0}; address.port = port;
    enet_address_set_host(&address, "127.0.0.1");

    ENetHost *host = enet_host_create(NULL, 1, 2, 0, 0);
    ENetPeer *peer = enet_host_connect(host, &address, 2, 0);

    if (peer == NULL) {
        printf("[client] Cannot connect\n");
        return NULL;
    }

    librg_world *world = librg_world_create();
    librg_world_userdata_set(world, peer);

    librg_event_set(world, LIBRG_READ_CREATE, client_read_create);
    librg_event_set(world, LIBRG_READ_UPDATE, client_read_update);
    librg_event_set(world, LIBRG_READ_REMOVE, client_read_remove);
    librg_event_set(world, LIBRG_WRITE_UPDATE, client_write_update);

    return world;
}

int client_update(int ID, librg_world *world) {
    if (!librg_world_valid(world))
        return 1;

    ENetEvent event = {0};

    ENetPeer *peer = (ENetPeer *)librg_world_userdata_get(world);
    ENetHost *host = peer->host;

    while (enet_host_service(host, &event, 2) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                printf("[client] User %d has connected to the server.\n", ID);
            } break;
            case ENET_EVENT_TYPE_DISCONNECT:
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
                printf("[client] A user %d has disconnected from server.\n", ID);
            } break;

            case ENET_EVENT_TYPE_RECEIVE: {
                /* handle a newly received event */
                librg_world_read(
                    world,
                    ID,
                    (char *)event.packet->data,
                    event.packet->dataLength,
                    NULL
                );

                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);
            } break;

            case ENET_EVENT_TYPE_NONE: break;
        }
    }

    /* send our data to the server */
    char buffer[1024] = {0};
    size_t buffer_length = 1024;

    /* serialize peer's the world view to a buffer */
    librg_world_write(
        world,
        ID,
        buffer,
        &buffer_length,
        NULL
    );

    /* create packet with actual length, and send it */
    ENetPacket *packet = enet_packet_create(buffer, buffer_length, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);

    return 0;
}

int client_stop(librg_world *world) {
    if (!librg_world_valid(world))
        return 1;

    ENetPeer *peer = (ENetPeer *)librg_world_userdata_get(world);
    ENetHost *host = peer->host;

    librg_world_destroy(world);

    enet_peer_disconnect_now(peer, 0);
    enet_host_destroy(host);

    return 0;
}

// =======================================================================//
// !
// ! Main application
// !
// =======================================================================//

int main() {
    if (enet_initialize() != 0) {
        printf("[app] An error occurred while initializing ENet.\n");
        return 1;
    }

    int port = 27019;
    server_start(port);

    librg_world *clients[MAX_CLIENTS] = {0};
    for (int i = 0; i < MAX_CLIENTS; ++i)
        clients[i] = client_start(port);

    /* program will make N iterations, and then exit */
    for (int i = 0; i < 500; ++i) {
        server_update();

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            client_update(i, clients[i]);
        }

        zpl_sleep_ms(10);
    }

    printf("[app] Stopping everyting\n");

    for (int i = 0; i < MAX_CLIENTS; ++i)
        client_stop(clients[i]);

    server_stop();
    enet_deinitialize();

    printf("[app] Done!\n");
    return 0;
}
