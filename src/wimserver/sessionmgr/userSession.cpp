#include "userSession.h"

#include <functional>

namespace WIM
{
    UserSession::UserSession(std::shared_ptr<asio::io_service> service)
    {
        socket_.reset(new asio::ip::tcp::socket(*service));
    }

    UserSession::~UserSession()
    {
    }

    void UserSession::StartSession(const uint64_t & sessionId)
    {
        sessionId_ = sessionId;
        ReadPacketHeader();
    }

    void UserSession::ReadPacketHeader()
    {
        asio::async_read(*socket_, asio::buffer(&readSize_, 4),
            std::bind(&UserSession::HandlePacketHeaderReaded, shared_from_this(), std::placeholders::_1));
    }

    void UserSession::ReadPacketData()
    {
    }

    void UserSession::ProcessPacket()
    {
    }

    void UserSession::SendPacket()
    {
    }

    std::shared_ptr<asio::ip::tcp::socket> UserSession::GetSocket()
    {
        return socket_;
    }

    void UserSession::HandlePacketWritten()
    {
    }

    void UserSession::HandlePacketHeaderReaded(const asio::error_code & error)
    {
    }

    void UserSession::HandlePacketDataReaded(const asio::error_code & error)
    {
    }
}
