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

int main(){
    std::cout << "UDP Synchronous Server" << std::endl;
    try{
        boost::asio::io_service io_service;
        // Create Socket: to receive requests on UDP port 13
        boost::asio::ip::udp::socket socket(
                    io_service,
                    boost::asio::ip::udp::endpoint(
                                boost::asio::ip::udp::v4(),
                                13));
        std::cout << "Start : " << make_daytime_string();
        while(true){
            std::array<char, 1> receive_buffer;
            boost::asio::ip::udp::endpoint remote_endpoint;
            boost::system::error_code error;
            // Wait for Client & Initialize Constact with us
            socket.receive_from(
                        boost::asio::buffer(receive_buffer),
                        remote_endpoint,
                        0,/* socket_base::message_flags flag*/
                        error);
            std::cout << "Receive from: " << make_daytime_string();
            if(error && error != boost::asio::error::message_size){
                throw boost::system::system_error(error);
            }
            // determine what wa are going to sent back to client
            const std::string message = make_daytime_string();
            std::cout << "Message     : " << message;
            boost::system::error_code ignored_error;
            // Send the response to the remote endpoint
            socket.send_to(
                        boost::asio::buffer(message),
                        remote_endpoint,
                        0,/* socket_base::message_flags flag*/
                        ignored_error);
            std::cout << "Send to     : " << make_daytime_string();
        }
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
