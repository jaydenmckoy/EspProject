#include <iostream>
#include <chrono>
#include <thread>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <thread> // For std::thread

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
    std::cout << "Received data: " << msg->get_payload() << std::endl;
}

void runWebSocket(void) {
    c.run();
}

int main() {
   printf("EspClient started.\n");

   c.init_asio();

    websocketpp::lib::error_code ec;
    con = c.get_connection("ws://192.168.4.1:80/ws", ec);

    if (ec) {
        std::cout << "Connection error: " << ec.message() << std::endl;
        return 1;
    }

    // Set up connection settings here        
    
    // Set the message handler callback
    c.set_message_handler(std::bind(&on_message, &c, std::placeholders::_1, std::placeholders::_2));
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
    printf("Sending TakeImage command.\n");
    CommandPacket_t cmd;
    cmd.apid = TAKE_IMAGE_CMD_APID;
    uint8_t i;
    for(i=0; i < COMMAND_PACKET_MAX_SIZE; i++)
    {
      cmd.data[i] = i+1;
    }
    SendCommand(&cmd);

    // Wait for the WebSocket client thread to finish (optional)
    websocketThread.join();

    return 0;
}
