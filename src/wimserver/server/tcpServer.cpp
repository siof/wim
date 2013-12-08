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

#include "../sessionmgr/userSession.h"

namespace WIM
{
    Server::Server(const std::string & address, const std::string & port)
    {
        // create service and acceptor to accept new connections
        m_service = std::shared_ptr<basio::io_service>(new basio::io_service());
        m_acceptor = std::shared_ptr<basio::ip::tcp::acceptor>(new basio::ip::tcp::acceptor(*m_service));

        // resolve bind address
        basio::ip::tcp::resolver resolver(*m_service);
        basio::ip::tcp::resolver::query query(address, port);
        basio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        // bind acceptor to specified address and listen to new connections
        m_acceptor->open(endpoint.protocol());
        m_acceptor->set_option(basio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor->bind(endpoint);
        m_acceptor->listen(basio::socket_base::max_connections);
    }

    Server::~Server()
    {
        // stop service if needed
        if (!m_service->stopped())
            Stop();
    }

    void Server::Run(uint32_t threadCount)
    {
        // run server on specified amount of threads
        for (uint32_t i = 0; i < threadCount; ++i)
            m_runThreads.add_thread(new boost::thread(boost::bind(&basio::io_service::run, m_service.get())));
    }

    void Server::Kill()
    {
        // cancel async connections
        m_acceptor->cancel();
        // and stop server
        Stop();
    }

    void Server::Stop()
    {
        // we don't want to have any 'error' exceptions here
        boost::system::error_code ec;

        // close acceptor for new connections and stop service
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
        // create session element
        std::shared_ptr<UserSession> newSession = sessionMgr.CreateNewSession(m_service);
        // and wait for new session
        m_acceptor->async_accept(newSession->GetSocket(), boost::bind(&Server::HandleNewConnection, this, newSession, basio::placeholders::error));
    }

    void Server::HandleNewConnection(std::shared_ptr<UserSession> newSession, const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Error ( "<< error.value() << " ) '" << error.message() << "' occured when accepting new session ... server will be stopped ..." << std::endl;
            Stop();
            return;
        }

        sessionMgr.StartSession(newSession);
        AcceptNewConnection();
    }
}
