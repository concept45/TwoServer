/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef GROUPMETHODS_H
#define GROUPMETHODS_H

namespace LuaGroup
{
    /* BOOLEAN */
    int IsLeader(lua_State* L, Group* group)
    {
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);
        Eluna::Push(L, group->IsLeader(ObjectGuid(guid)));
        return 1;
    }

    int IsFull(lua_State* L, Group* group)
    {
        Eluna::Push(L, group->IsFull());
        return 1;
    }

    int IsRaidGroup(lua_State* L, Group* group)
    {
        Eluna::Push(L, group->isRaidGroup());
        return 1;
    }

    int IsBGGroup(lua_State* L, Group* group)
    {
        Eluna::Push(L, group->isBGGroup());
        return 1;
    }

    int IsMember(lua_State* L, Group* group)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 2);
        Eluna::Push(L, group->IsMember(player->GET_GUID()));
        return 1;
    }

    int IsAssistant(lua_State* L, Group* group)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 2);

        Eluna::Push(L, group->IsAssistant(player->GET_GUID()));
        return 1;
    }

    int SameSubGroup(lua_State* L, Group* group)
    {
        Player* player1 = Eluna::CHECKOBJ<Player>(L, 2);
        Player* player2 = Eluna::CHECKOBJ<Player>(L, 3);
        Eluna::Push(L, group->SameSubGroup(player1, player2));
        return 1;
    }

    int HasFreeSlotSubGroup(lua_State* L, Group* group)
    {
        uint8 subGroup = Eluna::CHECKVAL<uint8>(L, 2);
        Eluna::Push(L, group->HasFreeSlotSubGroup(subGroup));
        return 1;
    }

    int AddInvite(lua_State* L, Group* group)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 2);

        Eluna::Push(L, group->AddInvite(player));
        return 1;
    }

    /*int IsLFGGroup(lua_State* L, Group* group) // TODO: Implementation
    {
    Eluna::Push(L, group->isLFGGroup());
    return 1;
    }*/

    /*int IsBFGroup(lua_State* L, Group* group) // TODO: Implementation
    {
    Eluna::Push(L, group->isBFGroup());
    return 1;
    }*/

    /* GETTERS */
    int GetMembers(lua_State* L, Group* group)
    {
        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (GroupReference* itr = group->GetFirstMember(); itr; itr = itr->next())
        {
#ifndef TRINITY
            Player* member = itr->getSource();
#else
            Player* member = itr->GetSource();
#endif

            if (!member || !member->GetSession())
                continue;

            ++i;
            Eluna::Push(L, i);
            Eluna::Push(L, member);
            lua_settable(L, tbl);
        }

        lua_settop(L, tbl); // push table to top of stack
        return 1;
    }

    int GetLeaderGUID(lua_State* L, Group* group)
    {
#ifndef TRINITY
        Eluna::Push(L, group->GetLeaderGuid());
#else
        Eluna::Push(L, group->GetLeaderGUID());
#endif
        return 1;
    }

    int GetLeader(lua_State* L, Group* group)
    {
#ifndef TRINITY
        Eluna::Push(L, eObjectAccessor->FindPlayer(group->GetLeaderGuid()));
#else
        Eluna::Push(L, eObjectAccessor->FindPlayer(group->GetLeaderGUID()));
#endif
        return 1;
    }

    int GetGUID(lua_State* L, Group* group)
    {
#ifdef CLASSIC
        Eluna::Push(L, group->GetId());
#else
        Eluna::Push(L, group->GET_GUID());
#endif
        return 1;
    }

    int GetMemberGUID(lua_State* L, Group* group)
    {
        const char* name = Eluna::CHECKVAL<const char*>(L, 2);
#ifndef TRINITY
        Eluna::Push(L, group->GetMemberGuid(name));
#else
        Eluna::Push(L, group->GetMemberGUID(name));
#endif
        return 1;
    }

    int GetMembersCount(lua_State* L, Group* group)
    {
        Eluna::Push(L, group->GetMembersCount());
        return 1;
    }

    int GetMemberGroup(lua_State* L, Group* group)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 2);

        Eluna::Push(L, group->GetMemberGroup(player->GET_GUID()));
        return 1;
    }

    /* OTHER */
    int SetLeader(lua_State* L, Group* group)
    {
        Player* leader = Eluna::CHECKOBJ<Player>(L, 2);

        group->ChangeLeader(leader->GET_GUID());
        return 0;
    }

    // SendPacket(packet, sendToPlayersInBattleground[, ignoreguid])
    int SendPacket(lua_State* L, Group* group)
    {
        WorldPacket* data = Eluna::CHECKOBJ<WorldPacket>(L, 2);
        bool ignorePlayersInBg = Eluna::CHECKVAL<bool>(L, 3);
        uint64 ignore = Eluna::CHECKVAL<uint64>(L, 4);

        group->BroadcastPacket(data, ignorePlayersInBg, -1, ObjectGuid(ignore));
        return 0;
    }

    int RemoveMember(lua_State* L, Group* group)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 2);
        uint32 method = Eluna::CHECKVAL<uint32>(L, 3, 0);

#ifndef TRINITY
        Eluna::Push(L, group->RemoveMember(player->GET_GUID(), method));
#else
        Eluna::Push(L, group->RemoveMember(player->GET_GUID(), (RemoveMethod)method));
#endif
        return 1;
    }

    int Disband(lua_State* /*L*/, Group* group)
    {
        group->Disband();
        return 0;
    }

    int ConvertToRaid(lua_State* /*L*/, Group* group)
    {
        group->ConvertToRaid();
        return 0;
    }

    int SetMembersGroup(lua_State* L, Group* group)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 2);
        uint8 groupID = Eluna::CHECKVAL<uint8>(L, 3);

        group->ChangeMembersGroup(player->GET_GUID(), groupID);
        return 0;
    }

    int SetTargetIcon(lua_State* L, Group* group)
    {
        uint8 icon = Eluna::CHECKVAL<uint8>(L, 2);
        uint64 target = Eluna::CHECKVAL<uint64>(L, 3);
        uint64 setter = Eluna::CHECKVAL<uint64>(L, 4, 0);

        if (icon >= TARGETICONCOUNT)
            return luaL_argerror(L, 2, "valid target icon expected");

#if (defined(CLASSIC) || defined(TBC))
        group->SetTargetIcon(icon, ObjectGuid(target));
#else
        group->SetTargetIcon(icon, ObjectGuid(setter), ObjectGuid(target));
#endif
        return 0;
    }

    /*int ConvertToLFG(lua_State* L, Group* group) // TODO: Implementation
    {
    group->ConvertToLFG();
    return 0;
    }*/
};
#endif
