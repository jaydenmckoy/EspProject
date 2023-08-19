#include "WebsocketClient.h"
#include <thread>

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

WebsocketClient::WebsocketClient() : serverResponse(false)
{
   // Set logging to be pretty verbose (everything except message payloads)
   c.set_access_channels(websocketpp::log::alevel::all);
   c.clear_access_channels(websocketpp::log::alevel::frame_payload);

   // Initialize ASIO
   c.init_asio();

   // Register message handler
   c.set_message_handler([this](websocketpp::connection_hdl hdl,
                                message_ptr msg) { onMessage(&c, hdl, msg); });

   // Connect to Server websocket
   websocketpp::lib::error_code ec;
   con = c.get_connection("ws://192.168.4.1:80/ws", ec);

   if (ec) {
      std::cout << "Connection error: " << ec.message() << std::endl;
      // Handle connection error
   }
   c.connect(con);
}

void WebsocketClient::runWebSocket(void) { c.run(); }

void WebsocketClient::ping(void)
{
   CommandPacket_t cmd;
   cmd.apid = PING_CMD_APID;

   for (uint8_t i = 0; i < COMMAND_PACKET_MAX_SIZE; i++) {
      cmd.data[i] = i + 1;
   }

   while (!serverResponse) {
      printf("Sending ping.\n");
      sendCommand(&cmd);
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
   }
}

void WebsocketClient::sendCommand(CommandPacket_t *cmd)
{
   try
   {
      c.send(con, reinterpret_cast<uint8_t *>(cmd), sizeof(CommandPacket_t),
             websocketpp::frame::opcode::binary);
   }
   catch (const std::exception &e)
   {
      std::cout << "WebSocket Exception: " << e.what() << std::endl;
   }
}

void WebsocketClient::onMessage(client *c, websocketpp::connection_hdl hdl,
                                message_ptr msg)
{
   std::string pld = msg->get_payload();
   const uint8_t *rx_data = reinterpret_cast<const uint8_t *>(&pld[0]);
   printf("Received data len: %ld\n", pld.size());
   printf("Data: ");
   for (size_t i = 0; i < pld.size(); i++) {
      printf("0x%02X ", rx_data[i]);
   }
   printf("\n");
   serverResponse = true;
}
