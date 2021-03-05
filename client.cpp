#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        // В доке написано, что он разрешает запросы к списку конечных точке
        // но я так понял, что эта штука помогает законектиться к серверу посредством resolve
        tcp::resolver resolver(io_context);

        // про это лучше почитать
        // https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio/reference/ip__basic_resolver/resolve/overload3.html
        // Там кста можно указывать либо службу для конекта, либо порт, я вот указал порт
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "13" /* daytime */);
        std::cout << "Coonecting" << std::endl;

        // создаем сокет для подключения
        tcp::socket socket(io_context);

        // Будет пробовать каждую точку подключения из переданных, пока не подключится
        // к какой-нибудь, вернет итертор на успешно подключенный ендпоинт
        // может кинуть ошибку в boost::system::system_error
        auto it = boost::asio::connect(socket, endpoints);

        for (;;) {
            // Тут создадим буффер для чтения из сокета
            // он ктса может быть и стд строкой и char[]
            std::cout << "Trying read" << std::endl;
            boost::array<char, 128> buf;
            boost::system::error_code error;

            // читаем
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            // Эту ошибку получим, когда сервер разорвет соединение, это и будет для нас
            // идентификатором выхода из цикла
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            // здеь просто выводим полученные данные в поток
            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}