#include <YSI\y_hooks>

hook OnPlayerDisconnect(playerid,reason)
{
	GodMode[playerid] = false;
	return 1;
}

hook OnPlayerUpdate(playerid)
{
	if(GodMode[playerid]) SetPlayerHealth(playerid,400.0);
	return 1;
}