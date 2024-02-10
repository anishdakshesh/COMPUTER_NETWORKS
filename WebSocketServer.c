#include <stdio.h>
#include <libwebsockets.h>

static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            // New client connection established
            printf("Client connected\n");
            break;

        case LWS_CALLBACK_RECEIVE:
            // Data received from a client
            // Broadcast the received message to all clients
            lws_broadcast(wsi, in, len, 0);
            break;

        default:
            break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "chat-protocol",
        callback_echo,
        0,
        4096,  // Max frame size
    },
    { NULL, NULL, 0, 0 }
};

int main(int argc, char *argv[])
{
    struct lws_context *context;
    struct lws_context_creation_info info;

    memset(&info, 0, sizeof(info));
    info.port = 8765;  // WebSocket server port
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "WebSocket context creation failed\n");
        return 1;
    }

    printf("WebSocket server started. Listening on port 8765...\n");

    while (1) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return 0;
}
