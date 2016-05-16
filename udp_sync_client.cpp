#include <array>
#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char* argv[]){
    std::cout << "UDP Synchronous Client" << std::endl;
    try{
        if(argc != 2){
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }
        boost::asio::io_service io_service;
        // udp::resolver
        //   to find the correct remote endpoint
        boost::asio::ip::udp::resolver resolver(io_service);
        boost::asio::ip::udp::resolver::query query(
                    boost::asio::ip::udp::v4(),
                    argv[1],
                    "daytime");
        // udp::resolver::resolve()
        //   if it does not fail
        //      return at least one end point
        //   -> it is safe to dereference the return value directly
        boost::asio::ip::udp::endpoint receiver_endpoint =
                    *resolver.resolve(query);
        // UDP: datagram-oriented
        //   not be using a stream socket
        // Initialize ip::udp::socket & contact with remote endpoint
        boost::asio::ip::udp::socket socket(io_service);
        socket.open(boost::asio::ip::udp::v4());
        std::array<char, 1> send_buffer{{0}};
        socket.send_to(
                    boost::asio::buffer(send_buffer),
                    receiver_endpoint);
        // ready to accept whatever the server sends back to us
        //   endpoint of our side: Initialized by socket::receive_from()
        std::array<char, 128> receive_buffer;
        boost::asio::ip::udp::endpoint sender_endpoint;
        const std::size_t length = socket.receive_from(
                    boost::asio::buffer(receive_buffer),
                    sender_endpoint);
        std::cout.write(receive_buffer.data(), length);
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
