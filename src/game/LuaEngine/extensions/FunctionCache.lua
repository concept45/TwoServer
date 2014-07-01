--
-- Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
-- This program is free software licensed under GPL version 3
-- Please see the included DOCS/LICENSE.md for more information
--

-- Functions with parameters or multiple return values are not supported
-- These functions should return static data
local T = {
}

-- Global
table.insert(T, "GetLuaEngine")
table.insert(T, "GetCoreName")
table.insert(T, "GetCoreVersion")
table.insert(T, "GetCoreExpansion")

T.Object = {}
table.insert(T.Object, "GetEntry")
table.insert(T.Object, "GetGUID")
table.insert(T.Object, "GetGUIDLow")
table.insert(T.Object, "GetTypeId")

T.WorldObject = {}
table.insert(T.WorldObject, "GetName")

-- T.Unit = {}

T.Player = {}
table.insert(T.Player, "GetGMRank")
table.insert(T.Player, "GetAccountId")
table.insert(T.Player, "GetAccountName")
table.insert(T.Player, "GetPlayerIP")
table.insert(T.Player, "GetDbLocaleIndex")
table.insert(T.Player, "GetDbcLocale")

T.Creature = {}
table.insert(T.Creature, "GetCorpseDelay")
table.insert(T.Creature, "GetScriptId")
table.insert(T.Creature, "GetAIName")
table.insert(T.Creature, "GetScriptName")
table.insert(T.Creature, "IsWorldBoss")
table.insert(T.Creature, "IsRacialLeader")
table.insert(T.Creature, "IsCivilian")
table.insert(T.Creature, "IsGuard")
table.insert(T.Creature, "IsElite")

T.Item = {}
table.insert(T.Item, "GetClass")
table.insert(T.Item, "GetSubClass")
table.insert(T.Item, "GetName")
table.insert(T.Item, "GetDisplayId")
table.insert(T.Item, "GetQuality")
table.insert(T.Item, "GetBuyCount")
table.insert(T.Item, "GetBuyPrice")
table.insert(T.Item, "GetSellPrice")
table.insert(T.Item, "GetInventoryType")
table.insert(T.Item, "GetAllowableClass")
table.insert(T.Item, "GetAllowableRace")
table.insert(T.Item, "GetItemLevel")
table.insert(T.Item, "GetRequiredLevel")
if (GetCoreExpansion() == 2) then
    table.insert(T.Item, "GetStatsCount")
end
table.insert(T.Item, "GetRandomProperty")
if (GetCoreExpansion() ~= 0) then
    table.insert(T.Item, "GetRandomSuffix")
end
table.insert(T.Item, "GetItemSet")
table.insert(T.Item, "GetBagSize")
table.insert(T.Item, "IsBag")
if (GetCoreExpansion() ~= 0) then
    table.insert(T.Item, "IsCurrencyToken")
end
table.insert(T.Item, "IsPotion")
if (GetCoreExpansion() ~= 3) then
    table.insert(T.Item, "IsWeaponVellum")
    table.insert(T.Item, "IsArmorVellum")
end
table.insert(T.Item, "IsConjuredConsumable")

T.Aura = {}
table.insert(T.Aura, "GetCaster")
table.insert(T.Aura, "GetCasterGUID")
table.insert(T.Aura, "GetAuraId")
table.insert(T.Aura, "GetOwner")

T.Spell = {}
table.insert(T.Spell, "GetCaster")
table.insert(T.Spell, "GetCastTime")
table.insert(T.Spell, "GetEntry")
table.insert(T.Spell, "GetDuration")
table.insert(T.Spell, "GetPowerCost")
table.insert(T.Spell, "GetTargetDest")
table.insert(T.Spell, "GetTarget")

T.Quest = {}
table.insert(T.Quest, "GetId")
table.insert(T.Quest, "GetLevel")
table.insert(T.Quest, "GetMinLevel")
table.insert(T.Quest, "GetNextQuestId")
table.insert(T.Quest, "GetPrevQuestId")
table.insert(T.Quest, "GetNextQuestInChain")
table.insert(T.Quest, "GetFlags")
table.insert(T.Quest, "GetType")
table.insert(T.Quest, "HasFlag")
if (GetCoreExpansion() ~= 0) then
    table.insert(T.Quest, "IsDaily")
end
table.insert(T.Quest, "IsRepeatable")

T.Group = {}
table.insert(T.Group, "GetGUID")

T.Guild = {}
table.insert(T.Guild, "GetId")
table.insert(T.Guild, "GetName")

if (GetCoreExpansion() >= 2) then
    T.Vehicle = {}
    table.insert(T.Vehicle, "GetOwner")
    table.insert(T.Vehicle, "GetEntry")
end

T.QueryResult = {}
table.insert(T.QueryResult, "GetColumnCount")
table.insert(T.QueryResult, "GetRowCount")

T.WorldPacket = {}
table.insert(T.WorldPacket, "GetSize")

T.Map = {}
table.insert(T.Map, "GetName")
table.insert(T.Map, "GetDifficulty")
table.insert(T.Map, "GetInstanceId")
table.insert(T.Map, "GetMapId")
if (GetCoreExpansion() ~= 0) then
    table.insert(T.Map, "IsArena")
end
table.insert(T.Map, "IsBattleground")
table.insert(T.Map, "IsDungeon")
if (GetCoreExpansion() ~= 0) then
    table.insert(T.Map, "IsHeroic")
end
table.insert(T.Map, "IsRaid")

T.Corpse = {}
table.insert(T.Corpse, "GetOwnerGUID")

T.Weather = {}
table.insert(T.Weather, "GetZoneId")

-- T.AuctionHouseObject


-- This is used for global functions
-- weak keys for table so data available for GC when cant be used anymore
local GFT = {}
do
    local GFTM = {
        __mode = "k",
    }
    setmetatable(GFT, GFTM)
end

for typeName, tbl in pairs(T) do
    if (type(typeName) == "number" and type(tbl) == "string") then
        -- Global function
        assert(_G[tbl], "_G["..tostring(tbl).."] doesnt exist")
        local temp = _G[tbl]
        
        _G[tbl] = function()
            if (GFT[tbl] ~= nil) then
                return GFT[tbl]
            end
            GFT[tbl] = temp()
            return GFT[tbl]
        end
        
    elseif (type(typeName) == "string" and type(tbl) == "table") then
        -- A method
        assert(_G[typeName], "_G["..tostring(typeName).."] doesnt exist")
    
        for _, funcName in ipairs(tbl) do
            assert(_G[typeName][funcName], "_G["..tostring(typeName).."]["..tostring(funcName).."] doesnt exist")
            
            -- Temporarily save the actual Get function with different name
            local temp = _G[typeName][funcName]

            -- weak keys for table so data available for GC when cant be used anymore
            local FT = {}
            do
                local FTM = {
                    __mode = "k",
                }
                setmetatable(FT, FTM)
            end
            
            -- Check cache table for reference, return it if exists else fetch, save and return
            _G[typeName][funcName] = function(self)
                if (FT[self] ~= nil) then
                    return FT[self]
                end
                FT[self] = temp(self)
                return FT[self]
            end
        end
    else
        error("_G["..tostring(typeName).."] = "..tostring(tbl).." is not valid k,v pair for Get cache system")
    end
end
