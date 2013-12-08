#ifndef USERSESSION_H_INCLUDED
#define USERSESSION_H_INCLUDED

#include "../defines.h"

#include <boost/asio.hpp>

namespace WIM
{
    class UserSession
    {
    public:
        UserSession(asio::io_service & service, const uint64_t & sessionId):
            m_socket(service), sessionId_(sessionId)
        {

        }

        virtual ~UserSession() {}

        virtual void StartSession() { ReadPacketHeader(); }

        virtual void ReadPacketHeader() = 0;
        virtual void ReadPacketData() = 0;

        virtual void ProcessPacket() {}
        virtual void SendPacket() {}

        asio::ip::tcp::socket & GetSocket()
        {
            return m_socket;
        }

    private:

        virtual void HandlePacketWritten() {}
        virtual void HandlePacketHeaderReaded() = 0;
        virtual void handlePacketDataReaded() = 0;

        asio::ip::tcp::socket m_socket;

        uint64_t sessionId_;
    };
}

#endif // USERSESSION_H_INCLUDED
