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

// TCP Connection
//  use shared_ptr, enable_shared_from_this
//  to keep the Object alive as long as there is an Operation that refers to it
class TCPConnection: public std::enable_shared_from_this<TCPConnection>{
public:
    // pointer type
    using pointer = std::shared_ptr<TCPConnection>;
    // Create
    static pointer create(
                boost::asio::io_service& io_service);
    // destructor
    ~TCPConnection();
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
//   Initialize: Acceptor to listen on TCP port 13
TCPServer::TCPServer(
            boost::asio::io_service& io_service)
      : acceptor_(io_service,
                  boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(),
                            13)){
    start_accept();
}
// Start Accept
//   Create Socket & Initialize Asynchronous Accept Operation
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
//   Called when Asynchronous Accept Operation Initialized
void TCPServer::handle_accept(
            const std::shared_ptr<TCPConnection>& new_connection,
            const boost::system::error_code& error){
    if(! error){
        // Service: the Client Request
        std::cout << "Call: TCPConnection::start()" << std::endl;
        new_connection->start();
    }
    // Initialize the next Accept Operation
    std::cout << "Create New Connection" << std::endl;
    start_accept();
}
// TCPConnection
// Create
TCPConnection::pointer TCPConnection::create(
            boost::asio::io_service& io_service){
    // 関数内class
    //   std::make_sharedの関数内ではprivate constructorを呼び出せないため.
    //   newで生成するならば不要.
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
      : socket_(io_service){
    std::cout << "TCP Connection constructed: at "
              << make_daytime_string();
}
// destructor
TCPConnection::~TCPConnection(){
    std::cout << "TCP Connection destructed : at "
              << make_daytime_string();
}
// Socket
boost::asio::ip::tcp::socket& TCPConnection::socket(){
    return socket_;
}
// Start
//   Call boost::async_write() to serve the data to the client
//   boost::async_write_some() ではなく boost::async_writeを使う理由
//     to ensure that the entire block of data is sent
void TCPConnection::start(){
    const auto self = shared_from_this();
    // The Data to be sent:
    //   class member
    //   as we need keep it valid until the Asynchronous Operation is Completed
    message_ = make_daytime_string();
    // Capture shared_ptr
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
            std::size_t bytes_transferred){
    std::cout << bytes_transferred
              << " bytes Transferred" << std::endl;
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
