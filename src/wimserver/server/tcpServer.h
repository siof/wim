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

#ifndef TCPSERVER_H_INCLUDED
#define TCPSERVER_H_INCLUDED

#include <memory>
#include <string>
#include <thread>

#include <asio.hpp>

#include "../sessionMgr/sessionMgr.h"

namespace WIM
{
    class Server
    {
    public:
        Server(const std::string & address, const std::string & port);
        ~Server();

        void Run(uint32_t threadCount);
        void Stop();
        void Kill();

        uint32_t GetListenPort();
        std::string GetAddress();

        void JoinAll();
        void WaitForAll();

    private:
        void AcceptNewConnection();
        void HandleNewConnection(std::shared_ptr<UserSession> newSession, const asio::error_code & error);

        std::shared_ptr<asio::io_service> service_;
        std::shared_ptr<asio::ip::tcp::acceptor> acceptor_;

        WIM::SessionMgr sessionMgr_;

        std::vector<std::thread *> threads_;
    };
}

#endif // TCPSERVER_H_INCLUDED
