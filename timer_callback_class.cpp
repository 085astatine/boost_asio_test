#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class Printer{
public:
    // constructor
    Printer(boost::asio::io_service& io);
    // destructor
    ~Printer();
    // callback function
    void print();
private:
    boost::asio::deadline_timer timer_;
    int count_;
};
// constructor
Printer::Printer(
            boost::asio::io_service& io)
      : timer_(io,
               boost::posix_time::seconds(1)),
        count_(0){
    timer_.async_wait(boost::bind(&Printer::print, this));
}
// destructor
Printer::~Printer(){
    std::cout << "Final count is " << count_ << std::endl;
}
// callback function
void Printer::print(){
    if(count_ < 5){
        std::cout << count_ << std::endl;
        ++count_;
        timer_.expires_at(
                    timer_.expires_at() + boost::posix_time::seconds(1));
        timer_.async_wait(boost::bind(&Printer::print, this));
    }
}

int main_base(){
    boost::asio::io_service io;
    Printer printer(io);
    io.run();

    return 0;
}

class PrinterCpp11{
public:
    // constructor
    PrinterCpp11(
                boost::asio::io_service& io);
    // destructor
    ~PrinterCpp11();
    // callback functtion
    void print();
private:
    boost::asio::deadline_timer timer_;
    int count_;
};
// constructor
PrinterCpp11::PrinterCpp11(
            boost::asio::io_service& io)
      : timer_(io,
               boost::posix_time::seconds(1)),
        count_(0){
    timer_.async_wait([this](const boost::system::error_code&){
                    this->print();
                });
}
// destructor
PrinterCpp11::~PrinterCpp11(){
    std::cout << "Final count is " << count_ << std::endl;
}
// callback function
void PrinterCpp11::print(){
    if(count_ < 5){
        std::cout << count_ << std::endl;
        ++count_;
        timer_.expires_at(
                    timer_.expires_at() + boost::posix_time::seconds(1));
        timer_.async_wait([this](const boost::system::error_code&){
                        this->print();
                    });
    }
}
int main_cpp11(){
    boost::asio::io_service io;
    PrinterCpp11 printer(io);
    io.run();

    return 0;
}

int main(){
    std::cout << "main base" << std::endl;
    main_base();
    std::cout << "main c++11" << std::endl;
    main_cpp11();
}

