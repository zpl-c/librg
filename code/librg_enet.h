/* not yet sure when the work on this will start */

/* net module () */

// LIBRG_HANDSHAKE_INIT,
// LIBRG_HANDSHAKE_REQUEST,
// LIBRG_HANDSHAKE_ACCEPT,
// LIBRG_HANDSHAKE_REFUSE,

// LIBRG_CONNECTION_CONNECT,
// LIBRG_CONNECTION_DISCONNECT,

// LIBRG_API int           librg_config_tickdelay_set(librg_ctx *, float time_ms);
// LIBRG_API float         librg_config_tickdelay_get(librg_ctx *);

// LIBRG_API int           librg_server_start(librg_ctx *, const char *hostname, uint16_t port);
// LIBRG_API int           librg_server_stop(librg_ctx *);
// LIBRG_API int           librg_server_tick(librg_ctx *);
// LIBRG_API int           librg_server_status(librg_ctx *);
// LIBRG_API int           librg_server_disconnect(librg_ctx *, librg_client);
// LIBRG_API int           librg_server_userdata_set(librg_ctx *, librg_client, void *);
// LIBRG_API void *        librg_server_userdata_get(librg_ctx *, librg_client);
// LIBRG_API int           librg_server_send_single(librg_ctx *, librg_client, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_multiple(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_all(librg_ctx *, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_allexcept(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_chunks(librg_ctx *, librg_chunk *, size_t chunk_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_chunksexcept(librg_ctx *, librg_chunk *, size_t chunk_amount, librg_client *, size_t client_amount, void *data, size_t data_size);

// LIBRG_API int           librg_client_connect(librg_ctx *, const char *hostname, uint16_t port);
// LIBRG_API int           librg_client_disconnect(librg_ctx *);
// LIBRG_API int           librg_client_tick(librg_ctx *);
// LIBRG_API int           librg_client_status(librg_ctx *);
// LIBRG_API int           librg_client_send(librg_ctx *, void *data, size_t data_size);
