Dialog:Util_TeleportOption(playerid, response, listitem, inputtext[])
{
	if(response)
    {
        SetPlayerInterior(playerid,TeleportList[listitem][Interior]);
        SetPlayerPos(playerid,TeleportList[listitem][Pos_X],TeleportList[listitem][Pos_Y],TeleportList[listitem][Pos_Z]);
    }
	return 1;
}