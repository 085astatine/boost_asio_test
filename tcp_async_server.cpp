#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string make_daytime_string(){
    std::ostringstream sout;
    sout << boost::posix_time::microsec_clock::local_time() << std::endl;
    return sout.str();
}

class TCPConnection;

class TCPServer{
public:
    // constructor
    explicit TCPServer(
                boost::asio::io_service& io_service);
private:
    // Start Accept
    void start_accept();
    // Handle Accept
    void handle_accept(
                const std::shared_ptr<TCPConnection>& new_connection,
                const boost::system::error_code& error);
    // Accepter
    boost::asio::ip::tcp::acceptor acceptor_;
};

class TCPConnection: public std::enable_shared_from_this<TCPConnection>{
public:
    // pointer type
    using pointer = std::shared_ptr<TCPConnection>;
    // Create
    static pointer create(
                boost::asio::io_service& io_service);
    // Socket
    boost::asio::ip::tcp::socket& socket();
    // Start
    void start();
private:
    // constructor
    explicit TCPConnection(
                boost::asio::io_service& io_service);
    // void
    void handle_write(
                const boost::system::error_code& error,
                std::size_t bytes_transferrd);
    // Socket
    boost::asio::ip::tcp::socket socket_;
    // Message
    std::string message_;
};
// TCPServer
// constructor
TCPServer::TCPServer(
            boost::asio::io_service& io_service)
      : acceptor_(io_service,
                  boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(),
                            13)){
    start_accept();
}
// Start Accept
void TCPServer::start_accept(){
    auto new_connection = TCPConnection::create(
                acceptor_.get_io_service());
    acceptor_.async_accept(
                new_connection->socket(),
                [this, new_connection]
                (const boost::system::error_code& error){
                    this->handle_accept(new_connection, error);
                });
}
// Handle Accept
void TCPServer::handle_accept(
            const std::shared_ptr<TCPConnection>& new_connection,
            const boost::system::error_code& error){
    if(! error){
        new_connection->start();
    }
    start_accept();
}
// TCPConnection
// Create
TCPConnection::pointer TCPConnection::create(
            boost::asio::io_service& io_service){
    class Dummy: public TCPConnection{
    public:
        explicit Dummy(boost::asio::io_service& io_service)
                    : TCPConnection(io_service){}
    };
    return std::make_shared<Dummy>(io_service);
}
// constructor
TCPConnection::TCPConnection(
            boost::asio::io_service& io_service)
      : socket_(io_service){}
// Socket
boost::asio::ip::tcp::socket& TCPConnection::socket(){
    return socket_;
}
// Start
void TCPConnection::start(){
    const auto self = shared_from_this();
    message_ = make_daytime_string();
    boost::asio::async_write(
                socket_,
                boost::asio::buffer(message_),
                [self](const boost::system::error_code& error,
                       std::size_t bytes_transferred){
                    self->handle_write(error, bytes_transferred);
                });
}
// Handle Write
void TCPConnection::handle_write(
            const boost::system::error_code&,
            std::size_t){
}


int main(){
    std::cout << "TCP Asynchronous Server" << std::endl;
    try{
        boost::asio::io_service io_service;
        TCPServer server(io_service);
        io_service.run();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
