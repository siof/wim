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

#include <functional>
#include <iostream>

#include "../sessionMgr/userSession.h"

namespace WIM
{
    Server::Server(const std::string & address, const std::string & port)
    {
        // create service and acceptor to accept new connections
        service_ = std::shared_ptr<asio::io_service>(new asio::io_service());
        acceptor_ = std::shared_ptr<asio::ip::tcp::acceptor>(new asio::ip::tcp::acceptor(*service_));

        // resolve bind address
        asio::ip::tcp::resolver resolver(*service_);
        asio::ip::tcp::resolver::query query(address, port);
        asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        // bind acceptor to specified address and listen to new connections
        acceptor_->open(endpoint.protocol());
        acceptor_->set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_->set_option(asio::ip::tcp::acceptor::keep_alive(true));
        acceptor_->bind(endpoint);
        acceptor_->listen(asio::socket_base::max_connections);
    }

    Server::~Server()
    {
        // stop service if needed
        if (!service_->stopped())
            Stop();
    }

    void Server::Run(uint32_t threadCount)
    {
        // run server on specified amount of threads
        for (uint32_t i = 0; i < threadCount; ++i)
            threads_.push_back(new std::thread([this]() -> void {service_->run();}));
    }

    void Server::Kill()
    {
        // cancel async connections
        acceptor_->cancel();
        // and stop server
        Stop();
    }

    void Server::Stop()
    {
        // we don't want to have any 'error' exceptions here
        asio::error_code ec;

        // close acceptor for new connections and stop service
        acceptor_->close(ec);
        service_->stop();

        // wait for end of io_service::run() in threads
        WaitForAll();
    }

    uint32_t Server::GetListenPort()
    {
        return acceptor_->local_endpoint().port();
    }

    std::string Server::GetAddress()
    {
        return acceptor_->local_endpoint().address().to_string();
    }

    void Server::JoinAll()
    {
        std::for_each(threads_.begin(), threads_.end(), std::bind(&std::thread::join, std::placeholders::_1));
    }

    void Server::WaitForAll()
    {
        JoinAll();
    }

    void Server::AcceptNewConnection()
    {
        // create session element
        std::shared_ptr<UserSession> newSession = sessionMgr_.CreateNewSession(service_);
        // and wait for new session
        acceptor_->async_accept(*newSession->GetSocket(), std::bind(&Server::HandleNewConnection, this, newSession, std::placeholders::_1));
    }

    void Server::HandleNewConnection(std::shared_ptr<UserSession> newSession, const asio::error_code& error)
    {
        if (error)
        {
            std::cout << "Error ( "<< error.value() << " ) '" << error.message() << "' occured when accepting new session ... server will be stopped ..." << std::endl;
            Stop();
            return;
        }

        sessionMgr_.StartSession(newSession);
        AcceptNewConnection();
    }
}
