#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]){
    try{
        if(argc != 2){
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }
        boost::asio::io_service io_service;
        // Server名をparameterで指定しTCP endpointとする
        // tcp::resolver: queryを受け取り Endpoint Listとする
        tcp::resolver resolver(io_service);
        // Query作成
        //   name of server : argv[1]
        //   name of service: daytime
        tcp::resolver::query query(argv[1], "daytime");
        // EndPoint List: io::tcp::resolver::iterator として返される
        //   iteratorのdefault constructorはend iteratorとなる
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        // create &connect Socket
        tcp::socket socket(io_service);
        // EndPoint ListにはIPv4とIPv6が混在している可能性
        //   boost::asio::connectionはそれを自動的に解決する
        boost::asio::connect(socket, endpoint_iterator);
        while(true){
            // hold Received Data
            //   char[], std::vector等で代替可能
            // boost::asio::buffer
            //   Buffer Overrunが起こらないよう自動的にsizeを決定する
            std::array<char, 128> buf;
            boost::system::error_code error;
            // Close Connection
            // socket::read_some boost::asio::error::eof_errorで終了
            const std::size_t len = socket.read_some(
                        boost::asio::buffer(buf), error);
            if (error == boost::asio::error::eof){
                break; // Connection closed cleanly by peer.
            }else if(error){
                throw boost::system::system_error(error); // Some other error.
            }
            std::cout.write(buf.data(), len);
        }
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
