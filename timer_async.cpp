#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// callback function: asynchroinous operation終了時に呼ばれる
void print(const boost::system::error_code& /*ec*/){
    std::cout << "Hello, world!" << std::endl;
}

int main(){
    boost::asio::io_service io;
    boost::asio::deadline_timer timer(
                io,
                boost::posix_time::seconds(5));
    // asynchronoous wait
    // callback handlerを渡す
    timer.async_wait(&print);
    timer.async_wait([](const boost::system::error_code&){
                std::cout << "lambda callback" << std::endl;});
    // io_service::run()を実行する
    // この関数を呼ばなければ,asynchronous waitのcallbackは起動しない
    std::cout << "Call io_service::run" << std::endl;
    io.run();
    std::cout << "Call io_service::run: end" << std::endl;
    return 0;
}
