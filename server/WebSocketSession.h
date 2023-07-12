//
// Created by sungwon on 23. 7. 10.
//

#ifndef CHATSERVER_WEBSOCKETSESSION_H
#define CHATSERVER_WEBSOCKETSESSION_H

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;

typedef beast::websocket::stream<beast::tcp_stream> websocket;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    /**
     * @brief WebSocketSession 목록 초기화
     * @param socket
     * @param sessions
     */
    explicit WebSocketSession(tcp::socket socket, std::vector<std::shared_ptr<WebSocketSession>>& sessions)
            : socket_(std::move(socket)),
              sessions_(sessions)
    {
    }

    /**
     * @brief 세션 연결 후 Client 목록에 추가
     */
    void start() {
        websocket_.emplace(std::move(socket_));
        websocket_->async_accept([self = shared_from_this()](const boost::system::error_code& ec) {
            if (!ec) {
                self->sessions_.push_back(self);
                self->readWebSocketData();
            }
        });
    }

    /**
     * @brief Client 에게 에러 메시지 전송
     * @param message
     */
    void send(const std::string& message) {
        if (websocket_ && websocket_->is_open()) {
            websocket_->
                async_write(asio::buffer(message), [self = shared_from_this()](
                        const boost::system::error_code& ec, std::size_t bytesTransferred) {
                            if (ec) {
                                std::cerr << "Error: " << ec.message() << std::endl;
                            }
            });
        }
    }

private:

    /**
     * @brief 데이터를 비동기적으로 읽어옴. 오류가 발생하면 세션에서 제거
     */
    void readWebSocketData() {
        websocket_->async_read(buffer_, [self = shared_from_this(), this](const boost::system::error_code& ec, std::size_t bytesTransferred) {
            if (!ec) {
                self->broadcast(beast::buffers_to_string(self->buffer_.data()));
                std::cout << "Received data from client: " << beast::buffers_to_string(buffer_.data()) << std::endl;
                self->buffer_.consume(self->buffer_.size());
                self->readWebSocketData();
            } else {
                self->sessions_.erase(std::remove(self->sessions_.begin(), self->sessions_.end(), self), self->sessions_.end());
            }
        });
    }

    /**
     * @brief 자신을 제외한 연결된 모든 Client 에게 Broadcast.
     * @param message
     */
    void broadcast(const std::string& message) {
        for (const auto& session : sessions_) {
            if (session != shared_from_this()) {
                session->send(message);
            }
        }
    }

    boost::optional<websocket> websocket_;
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    std::vector<std::shared_ptr<WebSocketSession>>& sessions_;
};

#endif //CHATSERVER_WEBSOCKETSESSION_H
