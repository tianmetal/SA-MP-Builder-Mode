#include <YSI\y_hooks>

hook OnGameModeExit()
{
	foreach(new slot : DynamicObjects)
	{
	    DestroyDynamicObject(DynamicObject[slot]);
	}
	Iter_Clear(DynamicObjects);
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(GetPVarType(playerid,"EditingObject") > 0)
	{
	    new slot = GetPVarInt(playerid,"EditingObject");
		DeletePVar(playerid,"EditingObject");
	    ObjectEditor[slot] = INVALID_PLAYER_ID;
	}
}

public OnPlayerSelectDynamicObject(playerid, objectid, modelid, Float:x, Float:y, Float:z)
{
    new string[144];
	foreach(new slot : DynamicObjects)
	{
	    if(DynamicObject[slot] == objectid)
	    {
	        if((ObjectEditor[slot] != INVALID_PLAYER_ID) && (GetPVarInt(ObjectEditor[slot],"EditingObject") == slot))
	        {
	            new playername[MAX_PLAYER_NAME];
	            GetPlayerName(ObjectEditor[slot],playername,sizeof(playername));
			    format(string,sizeof(string),"<ERROR> : "RED"%s "GRAY"is currently editing this object!",playername);
				return SEM(playerid,string);
	        }
	        format(string,sizeof(string),"<OBJECT> : "WHITE"Selected object with "YELLOW"id %d",slot);
			ObjectEditor[slot] = playerid;
	        SetPVarInt(playerid,"EditingObject",slot);
	    	EditDynamicObject(playerid,objectid);
	    	SendClientMessage(playerid,X11_LIGHTBLUE,string);
	        break;
	    }
	}
	return 1;
}

public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(response == EDIT_RESPONSE_FINAL)
	{
		if(GetPVarType(playerid,"EditingObject") > 0)
		{
		    new slot = GetPVarInt(playerid,"EditingObject");
		    DeletePVar(playerid,"EditingObject");
		    ObjectEditor[slot] = INVALID_PLAYER_ID;
		    SetDynamicObjectPos(objectid,x,y,z);
            SetDynamicObjectRot(objectid,rx,ry,rz);
		}
	}
	else if(response == EDIT_RESPONSE_CANCEL)
	{
	    if(GetPVarType(playerid,"EditingObject") > 0)
		{
		    new slot = GetPVarInt(playerid,"EditingObject");
		    DeletePVar(playerid,"EditingObject");
		    ObjectEditor[slot] = INVALID_PLAYER_ID;
		    new Float:cPos[3],Float:cRot[3];
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,cPos[0]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,cPos[1]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,cPos[2]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_X,cRot[0]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Y,cRot[1]);
            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Z,cRot[2]);
            SetDynamicObjectPos(objectid,cPos[0],cPos[1],cPos[2]);
            SetDynamicObjectRot(objectid,cRot[0],cRot[1],cRot[2]);
		}
	}
	return 1;
}