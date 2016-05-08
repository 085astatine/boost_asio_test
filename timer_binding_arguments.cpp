#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// callback function: asynchronous wait
void print(const boost::system::error_code&,
           boost::asio::deadline_timer* timer_ptr,
           int* count_ptr){
    if(*count_ptr < 5){
        std::cout << *count_ptr << std::endl;
        ++(*count_ptr);
        // timerのexpiry timeを移動
        timer_ptr->expires_at(
                    timer_ptr->expires_at() + boost::posix_time::seconds(1));
        timer_ptr->async_wait(boost::bind(
                    print,
                    boost::asio::placeholders::error,
                    timer_ptr,
                    count_ptr));
    }
}

int main_base(){
    boost::asio::io_service io;
    int count = 0;
    boost::asio::deadline_timer timer(
                io,
                boost::posix_time::seconds(1));
    timer.async_wait(boost::bind(
                print,
                boost::asio::placeholders::error,
                &timer,
                &count));
    io.run();
    std::cout << "Final count is " << count << std::endl;
    return 0;
}

void print(const boost::system::error_code& ec,
           const std::shared_ptr<boost::asio::deadline_timer>& timer_ptr,
           const std::shared_ptr<int>& count_ptr){
    std::cout << "wait until: "
              << timer_ptr->expires_at() << std::endl;
    std::cout << "Timer pointer count: "
              << timer_ptr.use_count() << std::endl;
    if(*count_ptr < 5){
        std::cout << *count_ptr << std::endl;
        ++(*count_ptr);
        // timerのexpiry timeを移動
        timer_ptr->expires_at(
                    timer_ptr->expires_at() + boost::posix_time::seconds(1));
        timer_ptr->async_wait(
                    [&ec, &timer_ptr, &count_ptr]
                    (const boost::system::error_code&){
                        return print(
                                ec,
                                timer_ptr,
                                count_ptr);
                    });
    }
}

int main_cpp11(){
    boost::asio::io_service io;
    const auto count_ptr = std::make_shared<int>(0);
    const auto timer_ptr = std::make_shared<boost::asio::deadline_timer>(
                io,
                boost::posix_time::seconds(1));
    timer_ptr->async_wait(
                [&timer_ptr, &count_ptr]
                (const boost::system::error_code& ec){
                        return print(
                                ec,
                                timer_ptr,
                                count_ptr);
                });
    io.run();
    std::cout << "Final count is " << *count_ptr << std::endl;
    return 0;
}

int main(){
    std::cout << "main base" << std::endl;
    main_base();
    std::cout << "main cpp11" << std::endl;
    main_cpp11();
    return 0;
}

