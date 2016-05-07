#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main(){
    // io_serviceはProgram中に少なくとも１つ必要
    boost::asio::io_service io;
    // I/O機能にaccessするasio classの1つ
    // そのようなclassはconstructorの第1引数にio_serviceを取る
    boost::asio::deadline_timer timer(
                io,
                boost::posix_time::seconds(5));
    // blocking wait on the timer
    // timer生成から指定した時間が経過した(expired)ならば,wait()は終了する
    // deadline timerはexpiredかnot expiredの2つの状態を持つ
    timer.wait();
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
