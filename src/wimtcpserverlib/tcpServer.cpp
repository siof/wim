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

#include "tcpServer.h"

namespace WIM
{
    namespace TcpServer
    {
        Server::Server(const std::string & address, const std::string & port)
        {
            m_service = boost::shared_ptr<basio::io_service>(new basio::io_service());
            m_acceptor = boost::shared_ptr<basio::ip::tcp::acceptor>(new basio::ip::tcp::acceptor(*m_service));

            basio::ip::tcp::resolver resolver(*m_service);
            basio::ip::tcp::resolver::query query(address, port);
            basio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

            m_acceptor->open(endpoint.protocol());
            m_acceptor->set_option(basio::ip::tcp::acceptor::reuse_address(true));
            m_acceptor->bind(endpoint);
            m_acceptor->listen(basio::socket_base::max_connections);
        }

        Server::~Server()
        {
            if (!m_service->stopped())
                Stop();
        }

        void Server::Run(uint32_t threadCount)
        {
            for (uint32_t i = 0; i < threadCount; ++i)
                m_runThreads.add_thread(new boost::thread(boost::bind(&basio::io_service::run, m_service.get())));
        }

        void Server::Stop()
        {
            // we don't want to have any 'error' exceptions
            boost::system::error_code ec;

            // should we cancel all started async operations ? i don't think so ;p
            //m_acceptor->cancel();

            m_acceptor->close(ec);
            m_service->stop();

            // wait for end of io_service::run() in threads
            m_runThreads.join_all();
        }

        uint32_t Server::GetListenPort()
        {
            return m_acceptor->local_endpoint().port();
        }

        std::string Server::GetAddress()
        {
            return m_acceptor->local_endpoint().address().to_string();
        }

        void Server::JoinAll()
        {
            m_runThreads.join_all();
        }

        void Server::WaitForAll()
        {
            JoinAll();
        }

        void Server::AcceptNewConnection()
        {
            // create session
            boost::shared_ptr<Session> newSession = sSessionManager.CreateNewSession(m_service);
            // and wait for new session
            m_acceptor->async_accept(newSession->GetSocket(), boost::bind(&Server::HandleNewConnection, this, newSession, basio::placeholders::error));
        }

        void Server::HandleNewConnection(boost::shared_ptr<Session> newSession, const boost::system::error_code& error)
        {
            if (error)
            {
                std::cout << "Error ( "<< error.value() << " ) '" << error.message() << "' occured when accepting new session ... server will be stopped ..." << std::endl;
                Stop();
                return;
            }

            sSessionManager.StartSession(newSession);
            AcceptNewConnection();
        }
    }
}
