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

#include <string>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

namespace basio = boost::asio;

namespace WIM
{
    class Session;

namespace TcpServer
{

    class Server
    {
    public:
        Server(const std::string & address, const std::string & port);
        ~Server();

        void Run(uint32_t threadCount);
        void Stop();

        uint32_t GetListenPort();
        std::string GetAddress();

        void JoinAll();
        void WaitForAll();

    private:
        void AcceptNewConnection();
        void HandleNewConnection(boost::shared_ptr<Session> newSession, const boost::system::error_code& error);

        boost::shared_ptr<basio::io_service> m_service;
        boost::shared_ptr<basio::ip::tcp::acceptor> m_acceptor;

        boost::thread_group m_runThreads;
    };
}
}

#endif // TCPSERVER_H_INCLUDED
