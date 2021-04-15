#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <memory>

#define SERVICE_PORT 9000
#define WORKERS_AMOUNT 12
#define BUFFER_SIZE 4096

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(boost::asio::io_context &_io_context) : m_client_socket(_io_context) {}

    boost::asio::ip::tcp::socket &GetSocket() { return m_client_socket; }

private:
    boost::asio::ip::tcp::socket m_client_socket;
    std::array<int, BUFFER_SIZE> recv_buffer;
    std::array<int, BUFFER_SIZE> send_buffer;

};

class AsyncServer {
public:
    explicit AsyncServer(boost::asio::io_context &_io_context)
            : m_io_context(_io_context), m_tcp_acceptor(m_io_context) {}

    void StartServer();

private:
    void startAccept();

    void acceptHandler(std::shared_ptr<Connection> accepted_connection, const boost::system::error_code &ec) {
        if (ec) {
            std::cerr << "ERRROR" << std::endl;
            return;
        }
        try {
            std::cout << "Accepted client: "
                      << accepted_connection->GetSocket().remote_endpoint().address().to_string().c_str()
                      << " on thread [" << std::this_thread::get_id() << "]" << std::endl;
            accepted_connection->GetSocket().close();
            accepted_connection.reset();
            startAccept();
        } catch (std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        }
    }

    void run() { m_io_context.run(); }

    boost::asio::io_context &m_io_context;
    boost::asio::ip::tcp::acceptor m_tcp_acceptor;
};

void AsyncServer::startAccept() {
    std::shared_ptr<Connection> new_connection(new Connection(m_io_context));
    m_tcp_acceptor.async_accept(new_connection->GetSocket(),
                                [this, new_connection](const boost::system::error_code &ec) {
                                    this->acceptHandler(new_connection, ec);
                                });
}

void AsyncServer::StartServer() {
    boost::asio::ip::tcp::endpoint tcp_endpoint(boost::asio::ip::tcp::v4(), SERVICE_PORT);
    m_tcp_acceptor.open(tcp_endpoint.protocol());
    m_tcp_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_tcp_acceptor.bind(tcp_endpoint);
    m_tcp_acceptor.listen();

    startAccept();

    std::vector<std::thread> workers;
    for (int i = 0; i < WORKERS_AMOUNT; ++i) {
        workers.emplace_back(std::thread(std::bind(&AsyncServer::run, this)));
    }

    for (auto &worker : workers) {
        worker.join();
    }
}

int main() {
    boost::asio::io_context io;
    AsyncServer server(io);
    server.StartServer();

    return 0;
}