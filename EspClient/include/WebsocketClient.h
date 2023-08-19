#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <string>
#include <vector>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

class WebsocketClient
{
 public:
   WebsocketClient();
   void RunWebSocket(void);
   void SendServerData(uint8_t *data, size_t size);

 private:
   typedef websocketpp::client<websocketpp::config::asio_client> client;
   typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

   void ServerRxHandler(client *c, websocketpp::connection_hdl hdl, message_ptr msg);

   client c;
   client::connection_ptr con;
   bool serverResponse;
};

#endif // WEBSOCKET_CLIENT_H
