/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef WORLDOBJECTMETHODS_H
#define WORLDOBJECTMETHODS_H

namespace LuaWorldObject
{
    /* GETTERS */
    int GetName(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetName());
        return 1;
    }

    int GetMap(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetMap());
        return 1;
    }

#if (!defined(TBC) && !defined(CLASSIC))
    int GetPhaseMask(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetPhaseMask());
        return 1;
    }
#endif

    int GetInstanceId(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetInstanceId());
        return 1;
    }

    int GetAreaId(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetAreaId());
        return 1;
    }

    int GetZoneId(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetZoneId());
        return 1;
    }

    int GetMapId(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetMapId());
        return 1;
    }

    int GetX(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetPositionX());
        return 1;
    }

    int GetY(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetPositionY());
        return 1;
    }

    int GetZ(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetPositionZ());
        return 1;
    }

    int GetO(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetOrientation());
        return 1;
    }

    int GetLocation(lua_State* L, WorldObject* obj)
    {
        Eluna::Push(L, obj->GetPositionX());
        Eluna::Push(L, obj->GetPositionY());
        Eluna::Push(L, obj->GetPositionZ());
        Eluna::Push(L, obj->GetOrientation());
        return 4;
    }

    int GetNearestPlayer(lua_State* L, WorldObject* obj)
    {
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);

        Unit* target = NULL;
        ElunaUtil::WorldObjectInRangeCheck checker(true, obj, range, TYPEMASK_PLAYER);
#ifndef TRINITY
        MaNGOS::UnitLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(target, checker);
        Cell::VisitWorldObjects(obj, searcher, range);
#else
        Trinity::UnitLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, target, checker);
        obj->VisitNearbyObject(range, searcher);
#endif

        Eluna::Push(L, target);
        return 1;
    }

    int GetNearestGameObject(lua_State* L, WorldObject* obj)
    {
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 3, 0);

        GameObject* target = NULL;
        ElunaUtil::WorldObjectInRangeCheck checker(true, obj, range, TYPEMASK_GAMEOBJECT, entry);
#ifndef TRINITY
        MaNGOS::GameObjectLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(target, checker);
        Cell::VisitGridObjects(obj, searcher, range);
#else
        Trinity::GameObjectLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, target, checker);
        obj->VisitNearbyObject(range, searcher);
#endif

        Eluna::Push(L, target);
        return 1;
    }

    int GetNearestCreature(lua_State* L, WorldObject* obj)
    {
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 3, 0);

        Creature* target = NULL;
        ElunaUtil::WorldObjectInRangeCheck checker(true, obj, range, TYPEMASK_UNIT, entry);
#ifndef TRINITY
        MaNGOS::CreatureLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(target, checker);
        Cell::VisitGridObjects(obj, searcher, range);
#else
        Trinity::CreatureLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, target, checker);
        obj->VisitNearbyObject(range, searcher);
#endif

        Eluna::Push(L, target);
        return 1;
    }

    int GetPlayersInRange(lua_State* L, WorldObject* obj)
    {
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);

        std::list<Player*> list;
        ElunaUtil::WorldObjectInRangeCheck checker(false, obj, range, TYPEMASK_PLAYER);
#ifndef TRINITY
        MaNGOS::PlayerListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(list, checker);
        Cell::VisitWorldObjects(obj, searcher, range);
#else
        Trinity::PlayerListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, list, checker);
        obj->VisitNearbyObject(range, searcher);
#endif

        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (std::list<Player*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            Eluna::Push(L, ++i);
            Eluna::Push(L, *it);
            lua_settable(L, tbl);
        }

        lua_settop(L, tbl);
        return 1;
    }

    int GetCreaturesInRange(lua_State* L, WorldObject* obj)
    {
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 3, 0);

        std::list<Creature*> list;
        ElunaUtil::WorldObjectInRangeCheck checker(false, obj, range, TYPEMASK_UNIT, entry);
#ifndef TRINITY
        MaNGOS::CreatureListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(list, checker);
        Cell::VisitGridObjects(obj, searcher, range);
#else
        Trinity::CreatureListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, list, checker);
        obj->VisitNearbyObject(range, searcher);
#endif

        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (std::list<Creature*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            Eluna::Push(L, ++i);
            Eluna::Push(L, *it);
            lua_settable(L, tbl);
        }

        lua_settop(L, tbl);
        return 1;
    }

    int GetGameObjectsInRange(lua_State* L, WorldObject* obj)
    {
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 3, 0);

        std::list<GameObject*> list;
        ElunaUtil::WorldObjectInRangeCheck checker(false, obj, range, TYPEMASK_GAMEOBJECT, entry);
#ifndef TRINITY
        MaNGOS::GameObjectListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(list, checker);
        Cell::VisitGridObjects(obj, searcher, range);
#else
        Trinity::GameObjectListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, list, checker);
        obj->VisitNearbyObject(range, searcher);
#endif

        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (std::list<GameObject*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            Eluna::Push(L, ++i);
            Eluna::Push(L, *it);
            lua_settable(L, tbl);
        }

        lua_settop(L, tbl);
        return 1;
    }

    int GetNearObject(lua_State* L, WorldObject* obj)
    {
        bool nearest = Eluna::CHECKVAL<bool>(L, 2, true);
        float range = Eluna::CHECKVAL<float>(L, 3, SIZE_OF_GRIDS);
        uint16 type = Eluna::CHECKVAL<uint16>(L, 4, 0); // TypeMask
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 5, 0);
        uint32 hostile = Eluna::CHECKVAL<uint32>(L, 6, 0); // 0 none, 1 hostile, 2 friendly

        float x, y, z;
        obj->GetPosition(x, y, z);
        ElunaUtil::WorldObjectInRangeCheck checker(nearest, obj, range, type, entry, hostile);
        if (nearest)
        {
            WorldObject* target = NULL;
#ifndef TRINITY
            MaNGOS::WorldObjectLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(target, checker);
            Cell::VisitAllObjects(obj, searcher, range);
#else
            Trinity::WorldObjectLastSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, target, checker);
            obj->VisitNearbyObject(range, searcher);
#endif

            Eluna::Push(L, target);
            return 1;
        }
        else
        {
            std::list<WorldObject*> list;
#ifndef TRINITY
            MaNGOS::WorldObjectListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(list, checker);
            Cell::VisitAllObjects(obj, searcher, range);
#else
            Trinity::WorldObjectListSearcher<ElunaUtil::WorldObjectInRangeCheck> searcher(obj, list, checker);
            obj->VisitNearbyObject(range, searcher);
#endif

            lua_newtable(L);
            int tbl = lua_gettop(L);
            uint32 i = 0;

            for (std::list<WorldObject*>::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                Eluna::Push(L, ++i);
                Eluna::Push(L, *it);
                lua_settable(L, tbl);
            }

            lua_settop(L, tbl);
            return 1;
        }

        return 1;
    }

    int GetWorldObject(lua_State* L, WorldObject* obj)
    {
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);

#ifndef TRINITY
        switch (GUID_HIPART(guid))
        {
        case HIGHGUID_PLAYER:        Eluna::Push(L, obj->GetMap()->GetPlayer(ObjectGuid(guid))); break;
        case HIGHGUID_TRANSPORT:
        case HIGHGUID_MO_TRANSPORT:
        case HIGHGUID_GAMEOBJECT:    Eluna::Push(L, obj->GetMap()->GetGameObject(ObjectGuid(guid))); break;
#if (!defined(TBC) && !defined(CLASSIC))
        case HIGHGUID_VEHICLE:
#endif
        case HIGHGUID_UNIT:
        case HIGHGUID_PET:           Eluna::Push(L, obj->GetMap()->GetAnyTypeCreature(ObjectGuid(guid))); break;
        }
#else
        switch (GUID_HIPART(guid))
        {
        case HIGHGUID_PLAYER:        Eluna::Push(L, eObjectAccessor->GetPlayer(*obj, ObjectGuid(guid))); break;
        case HIGHGUID_TRANSPORT:
        case HIGHGUID_MO_TRANSPORT:
        case HIGHGUID_GAMEOBJECT:    Eluna::Push(L, eObjectAccessor->GetGameObject(*obj, ObjectGuid(guid))); break;
        case HIGHGUID_VEHICLE:
        case HIGHGUID_UNIT:          Eluna::Push(L, eObjectAccessor->GetCreature(*obj, ObjectGuid(guid))); break;
        case HIGHGUID_PET:           Eluna::Push(L, eObjectAccessor->GetPet(*obj, ObjectGuid(guid))); break;
        }
#endif
        return 1;
    }

    int GetDistance(lua_State* L, WorldObject* obj)
    {
        WorldObject* target = Eluna::CHECKOBJ<WorldObject>(L, 2, false);
        if (target && target->IsInWorld())
            Eluna::Push(L, obj->GetDistance(target));
        else
        {
            float X = Eluna::CHECKVAL<float>(L, 2);
            float Y = Eluna::CHECKVAL<float>(L, 3);
            float Z = Eluna::CHECKVAL<float>(L, 4);
            Eluna::Push(L, obj->GetDistance(X, Y, Z));
        }
        return 1;
    }

    int GetRelativePoint(lua_State* L, WorldObject* obj)
    {
        float dist = Eluna::CHECKVAL<float>(L, 2);
        float rad = Eluna::CHECKVAL<float>(L, 3);

        float x, y, z;
        obj->GetClosePoint(x, y, z, 0.0f, dist, rad);

        Eluna::Push(L, x);
        Eluna::Push(L, y);
        Eluna::Push(L, z);
        return 3;
    }

    int GetAngle(lua_State* L, WorldObject* obj)
    {
        WorldObject* target = Eluna::CHECKOBJ<WorldObject>(L, 2, false);

        if (target && target->IsInWorld())
            Eluna::Push(L, obj->GetAngle(target));
        else
        {
            float x = Eluna::CHECKVAL<float>(L, 2);
            float y = Eluna::CHECKVAL<float>(L, 3);
            Eluna::Push(L, obj->GetAngle(x, y));
        }
        return 1;
    }

    /* OTHER */
    int SendPacket(lua_State* L, WorldObject* obj)
    {
        WorldPacket* data = Eluna::CHECKOBJ<WorldPacket>(L, 2);
        obj->SendMessageToSet(data, true);
        return 0;
    }

    int SummonGameObject(lua_State* L, WorldObject* obj)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        float x = Eluna::CHECKVAL<float>(L, 3);
        float y = Eluna::CHECKVAL<float>(L, 4);
        float z = Eluna::CHECKVAL<float>(L, 5);
        float o = Eluna::CHECKVAL<float>(L, 6);
        uint32 respawnDelay = Eluna::CHECKVAL<uint32>(L, 7, 30);
#ifndef TRINITY
        Eluna::Push(L, obj->SummonGameObject(entry, x, y, z, o, respawnDelay));
#else
        Eluna::Push(L, obj->SummonGameObject(entry, x, y, z, o, 0, 0, 0, 0, respawnDelay));
#endif
        return 1;
    }

    int SpawnCreature(lua_State* L, WorldObject* obj)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        float x = Eluna::CHECKVAL<float>(L, 3);
        float y = Eluna::CHECKVAL<float>(L, 4);
        float z = Eluna::CHECKVAL<float>(L, 5);
        float o = Eluna::CHECKVAL<float>(L, 6);
        uint32 spawnType = Eluna::CHECKVAL<uint32>(L, 7, 8);
        uint32 despawnTimer = Eluna::CHECKVAL<uint32>(L, 8, 0);

        TempSummonType type;
        switch (spawnType)
        {
        case 1:
            type = TEMPSUMMON_TIMED_OR_DEAD_DESPAWN;
            break;
        case 2:
            type = TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN;
            break;
        case 3:
            type = TEMPSUMMON_TIMED_DESPAWN;
            break;
        case 5:
            type = TEMPSUMMON_CORPSE_DESPAWN;
            break;
        case 6:
            type = TEMPSUMMON_CORPSE_TIMED_DESPAWN;
            break;
        case 7:
            type = TEMPSUMMON_DEAD_DESPAWN;
            break;
        case 8:
            type = TEMPSUMMON_MANUAL_DESPAWN;
            break;
        default:
            return luaL_argerror(L, 7, "valid SpawnType expected");
        }
        Eluna::Push(L, obj->SummonCreature(entry, x, y, z, o, type, despawnTimer));
        return 1;
    }
};
#endif
