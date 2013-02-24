/*
*   WIM - Web Instant Messanger project
*   Copyright (C) 2012 Siof
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU Affero General Public License version 3 as
*   published by the Free Software Foundation.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Affero General Public License for more details.
*
*   You should have received a copy of the GNU Affero General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include <memory>

#include <boost/asio.hpp>

namespace basio = boost::asio;

namespace WIM
{
    class Session
    {
    public:
        Session(basio::io_service & service);
        virtual ~Session() {}

        virtual void StartSession() { ReadPacketHeader(); }

        virtual void ReadPacketHeader() = 0;
        virtual void ReadPacketData() = 0;

        virtual void ProcessPacket() {}
        virtual void SendPacket() {}
    private:
        // we don't want default and copy ctors to be available
        Session() {}
        Session(const Session&) {}

        virtual void HandlePacketWritten() {}
        virtual void HandlePacketHeaderReaded() = 0;
        virtual void handlePacketDataReaded() = 0;

        std::shared_ptr<basio::ip::tcp::socket> m_socket;
    };
}

#endif // SESSION_H_INCLUDED
