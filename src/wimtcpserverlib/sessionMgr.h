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

#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>

#include "../wimsessionlib/session.h"

namespace basio = boost::asio;

namespace WIM
{
    // singleton based session manager implementation - exactly this should be problably called user manager or user session manager
    class SessionMgr
    {
    public:
        const SessionMgr const * Instance();

        boost::shared_ptr<Session> CreateNewSession(boost::shared_ptr<basio::io_service> service);
    private:
        SessionMgr() {}
        SessionMgr(const SessionMgr&) {}

        SessionMgr * m_sessionMgr;
    };

    #define sSessionMgr SessionMgr::Instance()
    #define sSessionManager SessionMgr::Instance()
}

#endif // SESSIONMGR_H_INCLUDED
