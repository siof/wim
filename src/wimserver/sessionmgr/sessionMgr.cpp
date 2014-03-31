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

#include "sessionMgr.h"

namespace WIM
{
    std::shared_ptr<UserSession> SessionMgr::CreateNewSession(std::shared_ptr<asio::io_service> service)
    {
        // placeholder - missing server session implementation

        return std::shared_ptr<UserSession>(new UserSession(service));
    }

    void SessionMgr::StartSession(std::shared_ptr<UserSession> session)
    {
        uint64_t tmpId = ++sessionCounter_;
        sessions_[tmpId] = session;
        session->StartSession(tmpId);
    }
}
