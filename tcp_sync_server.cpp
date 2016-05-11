#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string make_daytime_string(){
    const boost::posix_time::ptime ptime =
                boost::posix_time::microsec_clock::local_time();
    std::ostringstream sout;
    sout << ptime << std::endl;
    return sout.str();
}

int main(){
    using boost::asio::ip::tcp;
    std::cout << "Server Start: " << make_daytime_string();
    try{
        boost::asio::io_service io_service;
        // io::tcp::adapter
        //  TCP IPv4 Port 13
        //  Iterative Server: 1 connection at once
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
        while(true){
            std::cout << "Iterate Start: " << make_daytime_string();
            // Socket: ClientとのConnectionを表す
            tcp::socket socket(io_service);
            // Wait Connectiont
            acceptor.accept(socket);
            std::cout << "Get Socket   : " << make_daytime_string();
            const std::string message = make_daytime_string();
            std::cout << "Message      : " << message;
            boost::system::error_code ignored_error;
            // Send Message to Client
            boost::asio::write(
                        socket,
                        boost::asio::buffer(message),
                        ignored_error);
            std::cout << "Iterate End  : " << make_daytime_string();
        }
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
