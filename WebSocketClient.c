#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>

static struct lws *wsi_client = NULL;

static int callback_client(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            // Connection to the server established
            printf("Connected to server\n");
            wsi_client = wsi;
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            // Data received from the server
            printf("Received: %s\n", (char *)in);
            break;

        case LWS_CALLBACK_CLOSED:
            // Connection to the server closed
            wsi_client = NULL;
            break;

        default:
            break;
    }

    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "chat-protocol",
        callback_client,
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
    info.port = -1;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.address = "localhost";  // WebSocket server address
    info.path = "/";             // WebSocket server URL path

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "WebSocket context creation failed\n");
        return 1;
    }

    while (!wsi_client) {
        lws_service(context, 50);
    }

    // Send a message to the server
    const char *message = "Hello, WebSocket Server!";
    lws_write(wsi_client, (unsigned char *)message, strlen(message), LWS_WRITE_TEXT);

    while (1) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return 0;
}
