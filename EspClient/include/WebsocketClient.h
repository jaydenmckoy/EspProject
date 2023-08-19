#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <vector>
#include <string>

#define COMMAND_PACKET_MAX_SIZE 3

enum eCommandApid
{
    PING_CMD_APID,
    TAKE_IMAGE_CMD_APID,
};

#pragma pack(1)
struct CommandPacket_t
{
    uint16_t apid;
    uint8_t data[COMMAND_PACKET_MAX_SIZE];
};

class WebsocketClient
{
public:
    WebsocketClient();
    void runWebSocket(void);
    void ping(void);

private:
    typedef websocketpp::client<websocketpp::config::asio_client> client;
    typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

    void sendCommand(CommandPacket_t *cmd);
    void onMessage(client *c, websocketpp::connection_hdl hdl, message_ptr msg);

    client c;
    client::connection_ptr con;
    bool serverResponse;
};

#endif // WEBSOCKET_CLIENT_H
