/*
 * Можете ругаться, но я буду тут комментить на русском
 * */

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// эта функция просто будет создавать строку со временем
std::string make_daytime_string() {
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

int main () {
    try {
        // все проги с асио должны иметь ио контекст, подробнее о нем есть в доке
        boost::asio::io_context io;

        // приниматель соединений на 13 порте
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));


        // так как acceptor принимает соединения циклически, имеет смысл загнать все это дело в цикл
        for (;;) {
            // сокет клиента
            tcp::socket sock(io);
            // ловим соединение
            acceptor.accept(sock);

            // когда поймали, пишем время в строку сообщения
            std::string message = make_daytime_string();
            std::cout << "Catch connection at: " << message;

            boost::system::error_code err; // вообще можно чекать этот ерр, но мы его игнорим

            // записываем сообщение ответ клиенту в сокет,
            // Функция boost::asio :: buffer ()
            // автоматически определяет размер массива, чтобы предотвратить переполнение буфера.
            // через нее и передадим сообщ
            boost::asio::write(sock, boost::asio::buffer(message), err);
        }
    } catch (std::exception &e) {
        std::cerr << "Error found: ";
        std::cerr << e.what() << std::endl;
    }

    return 0;
}