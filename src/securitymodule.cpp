#include "Channel.h"
#include "ScriptMgr.h"
#include "Player.h"

enum ForbiddenAreas
{
	AREA_GM_ISLAND           = 876,   // GM Island
};

class map_security : public PlayerScript
{
public:
	map_security() : PlayerScript("map_security") {}

	void OnUpdateZone(Player* pPlayer, uint32 __attribute__ ((unused))newZone, uint32 __attribute__ ((unused))newArea)
	{
		// Forbidden areas:
		switch (pPlayer->GetAreaId())
		{
		case AREA_GM_ISLAND:
			{
				if (pPlayer->GetSession()->GetSecurity() >= 1)
					return;

				pPlayer->TeleportTo(1, 16218.700195, 16403.599609, -64.378288, 6.263502); // Prison
				pPlayer->GetSession()->SendAreaTriggerMessage("Sorry but you are not a GameMaster this area is off limits.");
			}
			break;
		}
	}
};

class gamemasters_security : public PlayerScript
{
public:
	gamemasters_security() : PlayerScript("gamemasters_security") {}

	void OnLogin(Player* player)
	{

        // Prevent GMs at all ranks to play as a normal player
        if (player->GetSession()->GetSecurity() == 1 || player->GetSession()->GetSecurity() == 2 || player->GetSession()->GetSecurity() == 3)
        {
            for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            for (uint8 slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            for (uint8 slot = BANK_SLOT_ITEM_START; slot < BANK_SLOT_ITEM_END; slot++)
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            for (uint8 slot = BANK_SLOT_BAG_START; slot < BANK_SLOT_BAG_END; slot++)
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            for (uint8 slot = BUYBACK_SLOT_START; slot < BUYBACK_SLOT_END; slot++)
                player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
 
            player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 2586, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FEET, 11508, true);
            player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 12064, true);
        }

        // Prevent players to log in with the same IP
		/*SessionMap sessions = sWorld->GetAllSessions();
		for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
		{
			if (Player* plr = itr->second->GetPlayer())
			{
				// GMs can log with more that one character
				if (player->GetSession()->GetSecurity() >= 3)
					return;

				// but players are not able to
				if (player != plr) // Just in case to not face the same IP as player's IP
				{
					if (player->GetSession()->GetRemoteAddress() == plr->GetSession()->GetRemoteAddress())
						player->GetSession()->KickPlayer();
				}
			}
		}*/
	}
};

void AddsecuritymoduleScripts()
{
    new map_security();
    new gamemasters_security();
}
