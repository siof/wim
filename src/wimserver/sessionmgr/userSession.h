#ifndef USERSESSION_H_INCLUDED
#define USERSESSION_H_INCLUDED

#include <memory>

#include <asio.hpp>

#include "../defines.h"

namespace WIM
{
    class UserSession : public std::enable_shared_from_this<UserSession>
    {
    public:
        UserSession(std::shared_ptr<asio::io_service> service);
        ~UserSession();

        void StartSession(const uint64_t & sessionId);

        void ReadPacketHeader();
        void ReadPacketData();

        void ProcessPacket();
        void SendPacket();

        std::shared_ptr<asio::ip::tcp::socket> GetSocket();

    private:
        UserSession() {}
        UserSession(const UserSession &) {}

        void HandlePacketWritten();
        void HandlePacketHeaderReaded(const asio::error_code & error);
        void HandlePacketDataReaded(const asio::error_code & error);

        std::shared_ptr<asio::ip::tcp::socket> socket_;

        uint32_t readSize_;

        uint64_t sessionId_;
    };
}

#endif // USERSESSION_H_INCLUDED
