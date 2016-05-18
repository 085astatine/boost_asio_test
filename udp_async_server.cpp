#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string make_daytime_string(){
    std::ostringstream sout;
    sout << boost::posix_time::microsec_clock::local_time() << std::endl;
    return sout.str();
}

class UDPServer{
public:
    // constructor
    explicit UDPServer(
                boost::asio::io_service& io_service);
private:
    // Start Service
    void start_service();
    // Handle Receive
    void handle_receive(
                const boost::system::error_code& error,
                std::size_t bytes_transferred);
    // Handle Send
    void handle_send(
                const std::shared_ptr<std::string>& message,
                const boost::system::error_code& error,
                std::size_t bytes_transferred);
    
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1> receive_buffer_;
};
// constructor
//   Initialize Socket to listen on UDP port 13
UDPServer::UDPServer(
            boost::asio::io_service& io_service)
      : socket_(io_service,
                boost::asio::ip::udp::endpoint(
                            boost::asio::ip::udp::v4(),
                            13)){
    start_service();
}
// Start Service
void UDPServer::start_service(){
    std::cout << "Start Service: " << make_daytime_string();
    socket_.async_receive_from(
                boost::asio::buffer(receive_buffer_),
                remote_endpoint_,
                [this](const boost::system::error_code& error,
                       std::size_t bytes_transferred){
                    this->handle_receive(error, bytes_transferred);
                });
    std::cout << "Receive from : " << make_daytime_string();
}
// Handle Receive
void UDPServer::handle_receive(
            const boost::system::error_code& error,
            std::size_t /*bytes_transferred*/){
    if(! error || error == boost::asio::error::message_size){
        const std::shared_ptr<std::string> message_ptr =
                    std::make_shared<std::string>(make_daytime_string());
        socket_.async_send_to(
                    boost::asio::buffer(*message_ptr),
                    remote_endpoint_,
                    [this, message_ptr](
                                const boost::system::error_code& error,
                                std::size_t bytes_transferred){
                        this->handle_send(
                                    message_ptr,
                                    error,
                                    bytes_transferred);
                    });
        std::cout << "Send to      : " << make_daytime_string();
        start_service();
    }
}
// Handle Send
void UDPServer::handle_send(
            const std::shared_ptr<std::string>& message_ptr,
            const boost::system::error_code& /*error*/,
            std::size_t bytes_transferred){
    std::cout << "Message      : " << *message_ptr;
    std::cout << bytes_transferred
              << " bytes transferred" << std::endl;
}

int main(){
    std::cout << "UDP Asynchronous Server" << std::endl;
    try{
        boost::asio::io_service io_service;
        UDPServer server(io_service);
        io_service.run();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
