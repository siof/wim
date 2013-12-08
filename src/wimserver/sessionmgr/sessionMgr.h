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

#ifndef SESSIONMGR_H_INCLUDED
#define SESSIONMGR_H_INCLUDED

#include <memory>
#include <unordered_map>

#include <boost/asio/io_service.hpp>

#include "userSession.h"

namespace basio = boost::asio;

namespace WIM
{
    // session manager
    class SessionMgr
    {
    public:
        SessionMgr() {}
        SessionMgr(const SessionMgr&) {}
        ~SessionMgr() {}

        std::shared_ptr<UserSession> CreateNewSession(std::shared_ptr<basio::io_service> service);

        void StartSession(std::shared_ptr<UserSession> session);
    private:
        std::unordered_map<uint64_t, std::shared_ptr<UserSession> > sessions_;

    };
}

#endif // SESSIONMGR_H_INCLUDED
