/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "IdleShutdown.h"
#include "Config.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "World.h"


IdleShutdown* IdleShutdown::instance()
{
    static IdleShutdown instance;
    return &instance;
}

bool IdleShutdown::Initialize()
{
    enabled = sConfigMgr->GetOption<bool>("IdleShutdown.Enable", true);
    if (!enabled)
    {
        return false;
    }

    shutdownTimeout = sConfigMgr->GetOption<uint32>("IdleShutdown.Timeout", 600);
    return true;
}

void IdleShutdown::OnLogin(Player* player)
{
    if (!enabled || player->GetSession()->GetRemoteAddress() == "bot")
        return;


    sWorld->ShutdownCancel();
}

void IdleShutdown::OnLogout(Player* player)
{
    if (!enabled || player->GetSession()->GetRemoteAddress() == "bot")
        return;

    if (sWorld->GetActiveAndQueuedSessionCount() == 0)
        sWorld->ShutdownServ(shutdownTimeout, SHUTDOWN_MASK_IDLE, SHUTDOWN_EXIT_CODE, "server idle");
}
