#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

class Printer{
public:
    // constructor
    Printer(boost::asio::io_service& io);
    // destructor
    ~Printer();
    // callback function
    void print1();
    void print2();
private:
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;
    int count_;
};
// constructor
Printer::Printer(
            boost::asio::io_service& io)
      : strand_(io),
        timer1_(io, boost::posix_time::seconds(1)),
        timer2_(io, boost::posix_time::seconds(1)),
        count_(0){
    timer1_.async_wait(strand_.wrap(
                [this](const boost::system::error_code&){
                    this->print1();
                }));
    timer2_.async_wait(strand_.wrap(
                [this](const boost::system::error_code&){
                    this->print2();
                }));
}
// destructor
Printer::~Printer(){
    std::cout << "Final count is " << count_ << std::endl;
}
// callback function: 1
void Printer::print1(){
    std::cout << "Call " << __func__ << " at: "
              << boost::posix_time::microsec_clock::local_time() << std::endl;
    if(count_ < 10){
        std::cout << "Timer 1: " << count_ << std::endl;
        ++count_;
        timer1_.expires_at(
                    timer1_.expires_at() + boost::posix_time::seconds(1));
        timer1_.async_wait(strand_.wrap(
                    [this](const boost::system::error_code&){
                        this->print1();
                    }));
    }
    std::cout << "End " << __func__ << " at: "
              << boost::posix_time::microsec_clock::local_time() << std::endl;
}
// callback function: 2
void Printer::print2(){
    std::cout << "Call " << __func__ << " at: "
              << boost::posix_time::microsec_clock::local_time() << std::endl;
    if(count_ < 10){
        std::cout << "Timer 2: " << count_ << std::endl;
        ++count_;
        timer2_.expires_at(
                    timer2_.expires_at() + boost::posix_time::seconds(1));
        timer2_.async_wait(strand_.wrap(
                    [this](const boost::system::error_code&){
                        this->print2();
                    }));
    }
    std::cout << "End " << __func__ << " at: "
              << boost::posix_time::microsec_clock::local_time() << std::endl;
}

int main(){
    boost::asio::io_service io;
    Printer printer(io);
    boost::thread thread([&io](){io.run();});
    thread.join();

    return 0;
}
