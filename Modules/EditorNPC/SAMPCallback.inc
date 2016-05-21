#include <YSI\y_hooks>

hook OnPlayerConnect(playerid)
{
	if(IsPlayerNPC(playerid))
	{
        NPCLoading = false;
	    NPCTest[ID] = playerid;
	}
	return 1;
}
hook OnPlayerDisconnect(playerid,reason)
{
	if(Recording[playerid])
	{
	    StopRecordingPlayerData(playerid);
	    Recording[playerid] = false;
	}
	return 1;
}
hook OnPlayerRequestClass(playerid,classid)
{
	if(IsPlayerNPC(playerid)) SetSpawnInfo(playerid,-1,NPCTest[Skin],0.0,0.0,3.0,0.0,-1,-1,-1,-1,-1,-1);
	return 1;
}
hook OnPlayerSpawn(playerid)
{
	if(IsPlayerNPC(playerid))
	{
		new string[64];
		if(NPCTest[State] == PLAYER_STATE_ONFOOT)
		{
		    format(string,sizeof(string),"ONFOOT|%s",NPCTest[Record]);
		    SendClientMessage(playerid,-1,string);
		}
		else
		{
		    PutPlayerInVehicle(playerid,NPCTest[VehID],0);
		    format(string,sizeof(string),"VEHICLE|%s",NPCTest[Record]);
		    SendClientMessage(playerid,-1,string);
		}
	}
	return 1;
}