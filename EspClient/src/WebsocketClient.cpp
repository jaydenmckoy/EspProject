#include "WebsocketClient.h"
#include <thread>

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

WebsocketClient::WebsocketClient(TelemetryManager *manager) : tmManager(manager)
{
   // Set logging to be pretty verbose (everything except message payloads)
   c.set_access_channels(websocketpp::log::alevel::all);
   c.clear_access_channels(websocketpp::log::alevel::frame_payload);

   // Initialize ASIO
   c.init_asio();

   // Register message handler
   c.set_message_handler([this](websocketpp::connection_hdl hdl,
                                message_ptr msg) { ServerRxHandler(&c, hdl, msg); });

   // Connect to Server websocket
   websocketpp::lib::error_code ec;
   con = c.get_connection("ws://192.168.4.1:80/ws", ec);

   if (ec) {
      std::cout << "Connection error: " << ec.message() << std::endl;
      // Handle connection error
   }
   c.connect(con);
}

void WebsocketClient::RunWebSocket(void) { c.run(); }

void WebsocketClient::ServerRxHandler(client *c, websocketpp::connection_hdl hdl,
                                message_ptr msg)
{
   std::string pld = msg->get_payload();
   // const uint8_t *rx_data = reinterpret_cast<const uint8_t *>(&pld[0]);
   // printf("Received data len: %ld\n", pld.size());
   // printf("Data: ");
   // for (size_t i = 0; i < pld.size(); i++) {
   //    printf("0x%02X ", rx_data[i]);
   // }
   // printf("\n");
   // Add data as Telemetry packet
   DataPacket_t pkt = {0};
   pkt.size = pld.size();
   pkt.data = reinterpret_cast<uint8_t *>(&pld[0]);
   tmManager->AddPacket(pkt);
}

void WebsocketClient::SendServerData(uint8_t *data, size_t size)
{
   try
   {
      c.send(con, data, size,websocketpp::frame::opcode::binary);
   }
   catch (const std::exception &e)
   {
      std::cout << "WebSocket Exception: " << e.what() << std::endl;
   }
}

