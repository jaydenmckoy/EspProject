#include <iostream>
#include <chrono>
#include <thread>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <thread> // For std::thread
#include <vector>
#include <string>
// #include "WebsocketClient.h"


using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

#define COMMAND_PACKET_MAX_SIZE 3

enum eCommandApid
{
   PING_CMD_APID,
   TAKE_IMAGE_CMD_APID
};

#pragma pack(1)
struct CommandPacket_t
{
   uint16_t apid;
   uint8_t  data[COMMAND_PACKET_MAX_SIZE];
};

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

client c;
client::connection_ptr con;

bool serverResponse = false;

void SendCommand(CommandPacket_t * cmd)
{
   try
   {
      c.send(con, (uint8_t *) cmd, sizeof(CommandPacket_t), websocketpp::frame::opcode::binary);
   }
   catch(const std::exception& e)
   {
      std::cout << "WebSocket Exception: " << e.what() << std::endl;
   }
   
}

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
   std::string pld = msg->get_payload();
   const uint8_t * rx_data = reinterpret_cast<const uint8_t*>(&pld[0]);
   printf("Received data len: %ld\n",pld.size());
   printf("Data: ");
   int i;
   for(i=0; i < (int)pld.size(); i++) printf("0x%02X ",rx_data[i]);
   printf("\n");
   //  std::cout << "Received data: " << msg->get_payload() << std::endl;
    serverResponse = true;
}

void runWebSocket(void) {
    c.run();
}

int main(int argc, char **argv){
   printf("EspClient started.\n");

   // Set logging to be pretty verbose (everything except message payloads)
   c.set_access_channels(websocketpp::log::alevel::all);
   c.clear_access_channels(websocketpp::log::alevel::frame_payload);
   // Initialize ASIO
   c.init_asio();
   // Register our message handler
   c.set_message_handler(bind(&on_message,&c,::_1,::_2));
   //  c.set_message_handler(&onBinaryMessage);

    websocketpp::lib::error_code ec;
    con = c.get_connection("ws://192.168.4.1:80/ws", ec);

    if (ec) {
        std::cout << "Connection error: " << ec.message() << std::endl;
        return 1;
    }
    
   // Note that connect here only requests a connection. No network messages are
   // exchanged until the event loop starts running in the next line.
    c.connect(con);
    printf("Running...\n");    
    // Use std::thread to run the WebSocket event loop in a separate thread
    std::thread websocketThread(runWebSocket);
    printf("After run.\n");

    // Send take image command
    while(con->get_state() != websocketpp::session::state::open)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    CommandPacket_t cmd;
    cmd.apid = TAKE_IMAGE_CMD_APID;
    cmd.apid = PING_CMD_APID;
    uint8_t i;
    for(i=0; i < COMMAND_PACKET_MAX_SIZE; i++)
    {
      cmd.data[i] = i+1;
    }

    while(!serverResponse)
    {
    printf("Sending ping.\n");
      SendCommand(&cmd);
      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    // Wait for the WebSocket client thread to finish (optional)
    websocketThread.join();

    return 0;
}
