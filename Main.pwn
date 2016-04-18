/*==============================================================================
=================================[Builder Mode]=================================
=================================[by tianmetal]=================================
==============================================================================*/

/*==================================[Credits]===================================

Special Thanks to	:
	• SA:MP Team past & present & future

Thanks to			:
	• Y_Less    			for     sscanf, foreach, and YSI
	• Incognito  			for     streamer
	• Zeex       			for     zcmd
    • JaTochNietDan  		for 	FileManager
    • All contributors		for 	all specified projects above
==============================================================================*/

/*=================================[Changelog]==================================

Version 0.1:
	- Initial release

Version 0.2:
	- Rewritten part of mode, gamemode now requires YSI 3.x or 4.x
	- Implemented YSI's y_colour for all color purposes
==============================================================================*/

//=============================[Include & Defines]==============================

#define GAMEMODE
#define GAMEMODE_NAME		"BuilderMode"
#define GAMEMODE_VERSION    "0.2"
#define MAPNAME             "San Andreas"

#define MAX_EDITING_OBJECT			500
#define MAX_OBJECT_MATERIAL_SLOT	10
#define MAX_EDITING_VEHICLE			200
#define MAX_EDITING_PICKUP 			300
#define MAX_EDITING_ICON        	100

#include <a_samp>

#undef MAX_PLAYERS
#define MAX_PLAYERS (20)

#include <sscanf2>      				// sscanf plugin by Y_Less
#include <streamer>     				// streamer plugin by Incognito
#include <filemanager>      			// FileManager plugin by JaTochNietDan

#include <YSI\y_iterate>    			// YSI by Y_Less
#include <YSI\y_commands>   			// YSI by Y_Less
#include <YSI\y_color>					// YSI by Y_Less

#include "./Builder/Extras/Defines"
#include "./Builder/Extras/Variables"
#include "./Builder/Extras/Functions"
#include "./Builder/Extras/Commands"

#define MAIN_DATABASE "internal/main.db"

#define DIALOG_TELEPORT         (1)

#include "./Builder/Object/Defines"
#include "./Builder/Object/Variables"
#include "./Builder/Object/Callbacks"
#include "./Builder/Object/Commands"

//=================================[Variables]==================================

new bool:NPCLoading;
new bool:Recording[MAX_PLAYERS],
	RecordingState[MAX_PLAYERS],
	RecordingFile[MAX_PLAYERS][32];

new Iterator:DynamicVehicles<(MAX_EDITING_VEHICLE+1)>;  // +1 since vehicles starts with ID 1
new DynamicVehicleColor[(MAX_EDITING_VEHICLE+1)][3];    // +1 since vehicles starts with ID 1

new Iterator:DynamicPickups<MAX_EDITING_PICKUP>;
new DynamicPickup[MAX_EDITING_PICKUP];
new Text3D:DynamicPickupLabel[MAX_EDITING_PICKUP];

new Iterator:MapIcons<MAX_EDITING_ICON>;
new MapIcon[MAX_EDITING_ICON];
new Text3D:MapIconLabel[MAX_EDITING_PICKUP];

enum npcinfo
{
	ID,
	Skin,
	State,
	Record[32],
	VehID,
}
new NPCTest[npcinfo];

//==========================[Default SA:MP Callbacks]===========================

main()
{
	print("==/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====");
	print("=/=====/=====/=====/=====/=====/==[Builder Mode]=/=====/=====/=====/=====/=====/");
	print("/=====/=====/=====/=====/=====/===[by tianmetal]/=====/=====/=====/=====/=====/=");
	print("=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/==");
}

public OnGameModeInit()
{
	new password[32];
	UsePlayerPedAnims();
	SetGameModeText(""GAMEMODE_NAME" v"GAMEMODE_VERSION"");
	SendRconCommand("mapname "MAPNAME"");
	AddPlayerClass(0,0.0,0.0,3.0,0.0,0,0,0,0,0,0);
	GetServerVarAsString("password",password,32);
	if(IsNull(password))
	{
	    print("[warning] The server is not passworded, it is recommended to enable password!");
	}
	if(GetServerVarAsInt("announce") == 1)
	{
	    print("[warning] It is recommended to set variable announce to 0!");
	}
	
    new DB:database = db_open(MAIN_DATABASE);
    db_query(database,"CREATE TABLE IF NOT EXISTS `users`(`name`,`skin`,`pos_x`,`pos_y`,`pos_z`,`rot_z`)");
    db_query(database,"CREATE TABLE IF NOT EXISTS `npc_recordings`(`name`,`skin`,`vehmodel`)");
    db_close(database);
    
	return 1;
}
public OnGameModeExit()
{
	foreach(new vehid : DynamicVehicles)
	{
	    DestroyVehicle(vehid);
	}
	Iter_Clear(DynamicVehicles);
	return 1;
}
public OnPlayerConnect(playerid)
{
	if(IsPlayerNPC(playerid))
	{
        NPCLoading = false;
	    NPCTest[ID] = playerid;
	}
	new playername[MAX_PLAYER_NAME],string[144];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(string,sizeof(string),"[JOIN] "YELLOW"%s "WHITE"has connected to the server",playername);
	SendClientMessageToAll(X11_GREEN,string);
	return 1;
}
public OnPlayerDisconnect(playerid,reason)
{
	if(Recording[playerid])
	{
	    StopRecordingPlayerData(playerid);
	    Recording[playerid] = false;
	}
	GodMode[playerid] = false;
	
    new playername[MAX_PLAYER_NAME],string[256];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(string,144,"[LEAVE] "RED"%s "WHITE"has diconnected from the server",playername);
	SendClientMessageToAll(X11_RED,string);
	
	if(GMX) return 1;
	new Float:cPos[4];
	GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	GetPlayerFacingAngle(playerid,cPos[3]);
	strToLower(playername);
	format(string,sizeof(string),"UPDATE `users` SET `skin`='%d',`pos_x`='%f',`pos_y`='%f',`pos_z`='%f',`rot_z`='%f' WHERE `name`='%s'",
	GetPlayerSkin(playerid),cPos[0],cPos[1],cPos[2],cPos[3],playername);
	new DB:database = db_open(MAIN_DATABASE);
	db_query(database,string);
	db_close(database);
	return 1;
}
public OnPlayerRequestClass(playerid,classid)
{
	if(IsPlayerNPC(playerid))
	{
	    SetSpawnInfo(playerid,-1,NPCTest[Skin],0.0,0.0,3.0,0.0,-1,-1,-1,-1,-1,-1);
	    return 1;
	}
	if(GetPVarType(playerid,"Spawned") == 0)
	{
	    new string[256],playername[MAX_PLAYER_NAME];
	    GetPlayerName(playerid,playername,sizeof(playername));
		strToLower(playername);
	    format(string,sizeof(string),"SELECT * FROM `users` WHERE `name`='%s'",playername);
		new DB:database = db_open(MAIN_DATABASE);
		new DBResult:result = db_query(database,string);
		if(db_num_rows(result) > 0)
		{
		    new data[32],skin,Float:spawnPos[4];
			db_get_field(result,1,data,sizeof(data)); skin = strval(data);
			db_get_field(result,2,data,sizeof(data)); spawnPos[0] = floatstr(data);
			db_get_field(result,3,data,sizeof(data)); spawnPos[1] = floatstr(data);
			db_get_field(result,4,data,sizeof(data)); spawnPos[2] = floatstr(data);
			db_get_field(result,5,data,sizeof(data)); spawnPos[3] = floatstr(data);
			db_free_result(result);
			SetSpawnInfo(playerid,-1,skin,spawnPos[0],spawnPos[1],spawnPos[2],spawnPos[3],0,0,0,0,0,0);
			SpawnPlayer(playerid);
		}
		else
		{
		    db_free_result(result);
			format(string,sizeof(string),"INSERT INTO `users` VALUES('%s','0','0.0','0.0','3.0','0.0')",playername);
			db_query(database,string);
		}
		db_close(database);
		SetPVarInt(playerid,"Spawned",1);
	}
	return 1;
}
public OnPlayerSpawn(playerid)
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
public OnPlayerDeath(playerid,killerid,reason)
{
	new Float:cPos[4];
	GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	GetPlayerFacingAngle(playerid,cPos[3]);
	SetSpawnInfo(playerid,-1,GetPlayerSkin(playerid),cPos[0],cPos[1],cPos[2],cPos[3],0,0,0,0,0,0);
	return 1;
}
public OnPlayerUpdate(playerid)
{
	if(GodMode[playerid]) SetPlayerHealth(playerid,400.0);
	return 1;
}
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
	    case DIALOG_TELEPORT:
    	{
	        if(response)
	        {
	            SetPlayerInterior(playerid,TeleportList[listitem][Interior]);
	            SetPlayerPos(playerid,TeleportList[listitem][Pos_X],TeleportList[listitem][Pos_Y],TeleportList[listitem][Pos_Z]);
	        }
		}
	}
	return 1;
}

//=============================[sscanf's kustom]================================


SSCANF:vehmenu(string[])
{
	if(!strcmp(string,"create",true)) 			return 1;
	else if(!strcmp(string,"add",true)) 		return 1;
	else if(!strcmp(string,"destroy",true)) 	return 2;
	else if(!strcmp(string,"delete",true)) 		return 2;
	else if(!strcmp(string,"remove",true)) 		return 2;
	else if(!strcmp(string,"model",true)) 		return 3;
	else if(!strcmp(string,"park",true)) 		return 4;
	else if(!strcmp(string,"color",true)) 		return 5;
	else if(!strcmp(string,"colour",true))		return 5;
	else if(!strcmp(string,"paintjob",true)) 	return 6;
	else if(!strcmp(string,"clear",true)) 		return 7;
	else if(!strcmp(string,"reset",true)) 		return 7;
	else if(!strcmp(string,"goto",true)) 		return 8;
	else if(!strcmp(string,"tele",true)) 		return 8;
	else if(!strcmp(string,"enter",true)) 		return 9;
	else if(!strcmp(string,"gethere",true)) 	return 10;
	else if(!strcmp(string,"import",true)) 		return 11;
	else if(!strcmp(string,"export",true)) 		return 12;
	else if(!strcmp(string,"respawn",true))     return 13;
	else if(!strcmp(string,"respawnall",true))  return 14;
	return 0;
}
SSCANF:pickupmenu(string[])
{
    if(!strcmp(string,"create",true)) 			return 1;
	else if(!strcmp(string,"add",true)) 		return 1;
	else if(!strcmp(string,"destroy",true)) 	return 2;
	else if(!strcmp(string,"delete",true)) 		return 2;
	else if(!strcmp(string,"remove",true)) 		return 2;
	else if(!strcmp(string,"model",true)) 		return 3;
	else if(!strcmp(string,"move",true)) 		return 4;
	else if(!strcmp(string,"gethere",true)) 	return 4;
	else if(!strcmp(string,"goto",true))		return 5;
	else if(!strcmp(string,"tele",true)) 		return 5;
	else if(!strcmp(string,"clear",true)) 		return 6;
	else if(!strcmp(string,"reset",true)) 		return 6;
	else if(!strcmp(string,"import",true)) 		return 7;
	else if(!strcmp(string,"export",true)) 		return 8;
	return 0;
}
SSCANF:mapiconmenu(string[])
{
    if(!strcmp(string,"create",true)) 			return 1;
	else if(!strcmp(string,"add",true)) 		return 1;
	else if(!strcmp(string,"destroy",true)) 	return 2;
	else if(!strcmp(string,"delete",true)) 		return 2;
	else if(!strcmp(string,"remove",true)) 		return 2;
	else if(!strcmp(string,"clear",true)) 		return 3;
	else if(!strcmp(string,"reset",true)) 		return 3;
	else if(!strcmp(string,"move",true)) 		return 4;
	else if(!strcmp(string,"gethere",true))		return 4;
	else if(!strcmp(string,"type",true)) 		return 5;
	else if(!strcmp(string,"model",true)) 		return 5;
	else if(!strcmp(string,"color",true)) 		return 6;
	else if(!strcmp(string,"colour",true)) 		return 6;
	else if(!strcmp(string,"tele",true)) 		return 7;
	else if(!strcmp(string,"goto",true)) 		return 7;
	else if(!strcmp(string,"import",true)) 		return 8;
	else if(!strcmp(string,"export",true)) 		return 9;
	return 0;
}
SSCANF:npcmenu(string[])
{
	if(!strcmp(string,"start",true)) 			return 1;
	else if(!strcmp(string,"stop",true)) 		return 2;
	else if(!strcmp(string,"test",true)) 		return 3;
	else if(!strcmp(string,"stoptest",true)) 	return 4;
	return 0;
}
SSCANF:importmenu(string[])
{
	if(!strcmp(string,"samp",true))				return 1;
	else if(!strcmp(string,"mta",true))			return 2;
	return 0;
}
SSCANF:exportmenu(string[])
{
	if(!strcmp(string,"samp",true))				return 1;
	else if(!strcmp(string,"streamer",true))	return 2;
	return 0;
}

//==================================[Commands]==================================

CMD:veh(playerid,params[])
{
    new action,subparam[128],string[256];
	unformat(params,"k<vehmenu>S()[128]",action,subparam);
	switch(action)
	{
	    case 1:
	    {
	        new model,color1,color2;
            if(sscanf(subparam,"k<vehicle>D(-1)D(-1)",model,color1,color2)) return SEM(playerid,"<SYNTAX> : /veh create [model] [opt:color1] [opt:color2]");
            if(model < 400 || model > 611) return SEM(playerid,"<ERROR> : Invalid vehicle model!");
            new Float:cPos[4];
            GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
            GetPlayerFacingAngle(playerid,cPos[3]);
            new vehid = CreateVehicle(model,cPos[0],cPos[1],cPos[2],cPos[3],color1,color2,600000);
            if(vehid > 0)
            {
				if(Iter_Add(DynamicVehicles,vehid))
				{
					DynamicVehicleColor[vehid][0] = color1;
					DynamicVehicleColor[vehid][1] = color2;
					DynamicVehicleColor[vehid][2] = 3;
					PutPlayerInVehicle(playerid,vehid,0);
					format(string,144,"<VEHICLE> : "WHITE"Vehicle id '"YELLOW"%d"WHITE"' with modelid '"YELLOW"%d"WHITE"' has been created, total vehicles: "GREEN"%d",vehid,model,Iter_Count(DynamicVehicles));
					SendClientMessage(playerid,X11_LIGHTBLUE,string);
				}
				else
				{
				    DestroyVehicle(vehid);
				    SEM(playerid,"<ERROR> : No vehicle free slot!");
				}
            }
            else SEM(playerid,"<ERROR> : No vehicle free slot!");
	    }
	    case 2:
	    {
	        new vehid;
			if(IsNull(subparam))
			{
			    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			    {
					vehid = GetPlayerVehicleID(playerid);
			    }
			    else return SEM(playerid,"<SYNTAX> : /veh destroy [slot]");
			}
			else
			{
				vehid = strval(subparam);
			}
			if(Iter_Contains(DynamicVehicles,vehid))
			{
			    DestroyVehicle(vehid);
			    Iter_Remove(DynamicVehicles,vehid);
			    format(string,144,"<VEHICLE> : "WHITE"Vehicle id '"YELLOW"%d"WHITE"' has been deleted, total vehicles: "GREEN"%d",vehid,Iter_Count(DynamicVehicles));
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid vehicle slot id!");
	    }
	    case 3:
	    {
	        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				new model,color1,color2,
					vehid = GetPlayerVehicleID(playerid);
				if(sscanf(subparam,"k<vehicle>D(-1)D(-1)",model,color1,color2)) return SEM(playerid,"<SYNTAX> : /veh model [newmodel] [opt:color1] [opt:color2]");
				if(model < 400 || model > 611) return SEM(playerid,"<ERROR> : Invalid vehicle model!");
				if(Iter_Contains(DynamicVehicles,vehid))
				{
	                new Float:cPos[4];
	                GetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
	                GetVehicleZAngle(vehid,cPos[3]);
	                DestroyVehicle(vehid);
	                new newvid = CreateVehicle(model,cPos[0],cPos[1],cPos[2],cPos[3],color1,color2,600000);
	                if(newvid != vehid)
	                {
	                    Iter_Remove(DynamicVehicles,vehid);
	                    Iter_Add(DynamicVehicles,newvid);
	                }
	                DynamicVehicleColor[newvid][0] = color1;
					DynamicVehicleColor[newvid][1] = color2;
					DynamicVehicleColor[newvid][2] = 3;
	                PutPlayerInVehicle(playerid,newvid,0);
	                format(string,144,"<VEHICLE> : "WHITE"Vehicle "YELLOW"slot %d "WHITE"model was changed to '"YELLOW"%d"WHITE"'",model);
					SendClientMessage(playerid,-1,string);
				}
				else SEM(playerid,"<ERROR> : Invalid vehicle!");
		    }
		    else SEM(playerid,"<ERROR> : You need to be in a vehicle to use this command!");
	    }
	    case 4:
	    {
	        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				new vehid = GetPlayerVehicleID(playerid);
                if(Iter_Contains(DynamicVehicles,vehid))
				{
	                new model = GetVehicleModel(vehid),
						Float:cPos[4],mods[14];
	                GetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
	                GetVehicleZAngle(vehid,cPos[3]);
	                Loop(i,14)
	                {
	                    mods[i] = GetVehicleComponentInSlot(vehid,i);
	                }
	                DestroyVehicle(vehid);
	                new newvid = CreateVehicle(model,cPos[0],cPos[1],cPos[2],cPos[3],0,0,600000);
	                if(newvid != vehid)
	                {
	                    Iter_Remove(DynamicVehicles,vehid);
	                    Iter_Add(DynamicVehicles,newvid);
	                    Loop(i,3)
	                    {
	                        DynamicVehicleColor[newvid][i] = DynamicVehicleColor[vehid][i];
	                    }
	                }
	                ChangeVehiclePaintjob(vehid,DynamicVehicleColor[vehid][2]);
	                ChangeVehicleColor(vehid,DynamicVehicleColor[vehid][0],DynamicVehicleColor[vehid][1]);
	                Loop(i,14)
	                {
	                    AddVehicleComponent(vehid,mods[i]);
	                }
	                PutPlayerInVehicle(playerid,vehid,0);
					SendClientMessage(playerid,X11_LIGHTBLUE,"<VEHICLE> : "WHITE"Vehicle parked!");
				}
				else SEM(playerid,"<ERROR> : Invalid vehicle!");
		    }
		    else SEM(playerid,"<ERROR> : You need to be in a vehicle to use this command!");
	    }
	    case 5:
	    {
	        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				new color1,color2,
					vehid = GetPlayerVehicleID(playerid);
				if(sscanf(subparam,"dD(-1)",color1,color2)) return SEM(playerid,"<SYNTAX> : /veh color [color1] [opt:color2]");
				if(Iter_Contains(DynamicVehicles,vehid))
				{
				    if(color2 == -1)
	                {
	                    color2 = color1;
	                }
	                DynamicVehicleColor[vehid][0] = color1;
	                DynamicVehicleColor[vehid][1] = color2;
	                ChangeVehiclePaintjob(vehid,DynamicVehicleColor[vehid][2]);
	                ChangeVehicleColor(vehid,DynamicVehicleColor[vehid][0],DynamicVehicleColor[vehid][1]);
					SendClientMessage(playerid,X11_LIGHTBLUE,"<VEHICLE> : "WHITE"Vehicle color changed!");
				}
                else SEM(playerid,"<ERROR> : Invalid vehicle!");
		    }
		    else SEM(playerid,"<ERROR> : You need to be in a vehicle to use this command!");
	    }
	    case 6:
	    {
	        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				new color3,
					vehid = GetPlayerVehicleID(playerid);
				if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /veh paintjob [paintjobid (0-3)]");
				color3 = strval(subparam);
				if(color3 < 0 || color3 > 3) return SEM(playerid,"<ERROR> : Invalid paintjobid, valid: 0-2 & 3 = delete paintjob");
				if(Iter_Contains(DynamicVehicles,vehid))
				{
				    DynamicVehicleColor[vehid][0] = 1;
				    DynamicVehicleColor[vehid][1] = 1;
	                DynamicVehicleColor[vehid][2] = color3;
	                ChangeVehiclePaintjob(vehid,DynamicVehicleColor[vehid][2]);
	                ChangeVehicleColor(vehid,DynamicVehicleColor[vehid][0],DynamicVehicleColor[vehid][1]);
					SendClientMessage(playerid,X11_LIGHTBLUE,"<VEHICLE> : "WHITE"Vehicle paintjob changed!");
				}
				else SEM(playerid,"<ERROR> : Invalid vehicle!");
		    }
		    else SEM(playerid,"<ERROR> : You need to be in a vehicle to use this command!");
	    }
	    case 7:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /veh clear [confirm]");
			if(!strcmp(subparam,"confirm",true))
			{
			    new count = Iter_Count(DynamicVehicles);
				if(count > 0)
				{
				    foreach(new vehid : DynamicVehicles)
				    {
				        DestroyVehicle(vehid);
				    }
				    Iter_Clear(DynamicVehicles);
				    format(string,144,"<VEHICLE> : "YELLOW"%d vehicles "WHITE"have been deleted",count);
					SendClientMessage(playerid,X11_LIGHTBLUE,string);
				}
				else SEM(playerid,"<ERROR> : There are no vehicles!");
			}
			else SEM(playerid,"<SYNTAX> : /veh clear [confirm]");
	    }
	    case 8:
	    {
			if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /veh goto [veh id]");
			new vehid = strval(subparam);
			if(Iter_Contains(DynamicVehicles,vehid))
			{
			    new Float:cPos[3];
				GetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
				SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+1.0));
				format(string,144,"<VEHICLE> : "WHITE"You have been telepored to vehicle "YELLOW"slot %d",vehid);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERRRO> : Invalid vehicle id!");
	    }
	    case 9:
	    {
			if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /veh enter [veh id]");
			new vehid = strval(subparam);
			if(Iter_Contains(DynamicVehicles,vehid))
			{
				PutPlayerInVehicle(playerid,vehid,0);
				format(string,144,"<VEHICLE> : "WHITE"You have been telepored to vehicle "YELLOW"slot %d",vehid);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERRRO> : Invalid vehicle id!");
		}
		case 10:
	    {
			if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /veh gethere [veh id]");
			new vehid = strval(subparam);
			if(Iter_Contains(DynamicVehicles,vehid))
			{
			    new Float:cPos[3];
			    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
				SetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
				SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+1.0));
				format(string,144,"<VEHICLE> : "WHITE"You have telepored vehicle "YELLOW"slot %d "WHITE"to your location",vehid);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERRRO> : Invalid vehicle id!");
	    }
	    case 11:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /veh import [filename]");
	        new filename[32],dir[64];
			strmid(filename,subparam,0,strlen(subparam),sizeof(filename));
			format(dir,sizeof(dir),"input/vehicles/%s",filename);
			if(fexist(dir))
			{
			    new File:file = fopen(dir,io_read),
					data[256],model,Float:cPos[4],color[2],loaded;
				while(fread(file,data,sizeof(data)) > 0)
				{
					strmid(string,data,(strfind(data,"(")+1),strfind(data,")"),sizeof(string));
				    if(strfind(data,"CreateVehicle") != -1 || strfind(data,"AddStaticVehicleEx") != -1)
				    {
				        unformat(string,"p<,>dffffdd{d}",model,cPos[0],cPos[1],cPos[2],cPos[3],color[0],color[1]);
				    }
				    else if(strfind(data,"AddStaticVehicle") != -1)
				    {
				        unformat(string,"p<,>dffffdd",model,cPos[0],cPos[1],cPos[2],cPos[3],color[0],color[1]);
				    }
				    else continue;
					new vehid = CreateVehicle(model,cPos[0],cPos[1],cPos[2],cPos[3],color[0],color[1],600000);
					Iter_Add(DynamicVehicles,vehid);
					DynamicVehicleColor[vehid][0] = color[0];
				    DynamicVehicleColor[vehid][1] = color[1];
	                DynamicVehicleColor[vehid][2] = 3;
					loaded++;
				}
				fclose(file);
				format(string,144,"<VEHICLE> : "YELLOW"%d vehicles "WHITE"have been loaded from file '"GREEN"%s"WHITE"'",loaded,filename);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
            else SEM(playerid,"<ERROR> : File does not exist!");
	    }
	    case 12:
	    {
			new count = Iter_Count(DynamicVehicles);
	        if(count == 0) return SEM(playerid,"<ERROR> : There are no vehicles!");
	        new filename[32],mode[9];
	        if(sscanf(subparam,"s[32]s[9]",filename,mode)) return SEM(playerid,"<SYNTAX> : /veh export [filename] [mode (simple/extended)]");
	        new bool:extended,dir[64];
	        if(!strcmp(mode,"extended",true))
	        {
	            extended = true;
	        }
			format(dir,sizeof(dir),"output/vehicles/%s",filename);
		    new File:file = fopen(dir,io_write),
				model,Float:cPos[4];
			if(extended)
			{
				format(string,sizeof(string),"new vehicles[%d];\r\n\r\n",count);
				fwrite(file,string);
			}
			foreach(new vehid : DynamicVehicles)
			{
				model = GetVehicleModel(vehid);
				GetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
				GetVehicleZAngle(vehid,cPos[3]);
				if(extended)
				{
					format(string,sizeof(string),"vehicles[%d] = CreateVehicle(%d, %.4f, %.4f, %.4f, %.4f, %d, %d, 600000);\r\n",(vehid-1),model,cPos[0],cPos[1],cPos[2],cPos[3],DynamicVehicleColor[vehid][0],DynamicVehicleColor[vehid][1]);
	                fwrite(file,string);
	                if(DynamicVehicleColor[vehid][2] != 3)
	                {
	                    format(string,sizeof(string),"ChangeVehiclePaintjob(vehicles[%d], %d);\r\nChangeVehicleColor(vehicles[%d], %d, %d);\r\n",(vehid-1),DynamicVehicleColor[vehid][2],(vehid-1),DynamicVehicleColor[vehid][0],DynamicVehicleColor[vehid][1]);
	                    fwrite(file,string);
	                }
	                Loop(i,14)
	                {
						new mod = GetVehicleComponentInSlot(vehid,i);
						if(mod > 0)
						{
						    format(string,sizeof(string),"AddVehicleComponent(vehicles[%d], %d);\r\n",(vehid-1),mod);
						    fwrite(file,string);
						}
	                }
				}
				else
				{
				    format(string,sizeof(string),"CreateVehicle(%d, %.4f, %.4f, %.4f, %.4f, %d, %d, 600000);\r\n",model,cPos[0],cPos[1],cPos[2],cPos[3],DynamicVehicleColor[vehid][0],DynamicVehicleColor[vehid][1]);
	                fwrite(file,string);
				}
			}
			fclose(file);
			format(string,144,"<VEHICLE> : "YELLOW"%d vehicles "WHITE"have been exported to '"GREEN"%s"WHITE"'",count,dir);
			SendClientMessage(playerid,X11_LIGHTBLUE,string);
	    }
	    case 13:
	    {
	        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				new vehid = (GetPlayerVehicleID(playerid));
				SetVehicleToRespawn(vehid);
				SendClientMessage(playerid,X11_LIGHTBLUE,"<VEHICLE> : "WHITE"Vehicle respawned!");
		    }
		    else SEM(playerid,"<ERROR> : You need to be in a vehicle to use this command!");
	    }
	    case 14:
	    {
	        foreach(new vehid : DynamicVehicles)
	        {
	            SetVehicleToRespawn(vehid);
	        }
	        format(string,144,"<VEHICLE> : "YELLOW"%d vehicles "WHITE"have been respawned!",Iter_Count(DynamicVehicles));
	        SendClientMessage(playerid,X11_LIGHTBLUE,string);
	    }
	    default:
	    {
	        SEM(playerid,"<SYNTAX> : /veh [action]");
	        SEM(playerid,"<ACTIONS> : create,destroy,model,park,color,paintjob,clear,goto,enter,gethere,import,export,respawn,respawnall");
	    }
	}
	return 1;
}
CMD:pickup(playerid,params[])
{
    new action,subparam[128],string[256];
	unformat(params,"k<pickupmenu>S()[128]",action,subparam);
	switch(action)
	{
	    case 1:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup create [model]");
			new model = strval(subparam);
			new slot = Iter_Free(DynamicPickups);
			if(slot != -1)
			{
			    new Float:cPos[3];
			    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
				DynamicPickup[slot] = CreateDynamicPickup(model,24,cPos[0],cPos[1],cPos[2]);
				Iter_Add(DynamicPickups,slot);
				format(string,32,"[pickup slot: %d model: %d]",slot,model);
				DynamicPickupLabel[slot] =	CreateDynamic3DTextLabel(string,-1,cPos[0],cPos[1],cPos[2],100.0);
				Streamer_Update(playerid);
				format(string,144,"<PICKUP> : "WHITE"Pickup model '"YELLOW"%d"WHITE"' with slot id '"YELLOW"%d"WHITE"' created, total pickups: "GREEN"%d",model,slot,Iter_Count(DynamicPickups));
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : No pickup free slot!");
	    }
	    case 2:
	    {
            if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup destroy [slot]");
            new slot = strval(subparam);
            if(Iter_Contains(DynamicPickups,slot))
            {
                DestroyDynamicPickup(DynamicPickup[slot]);
                Iter_Remove(DynamicPickups,slot);
				DestroyDynamic3DTextLabel(DynamicPickupLabel[slot]);
                format(string,144,"<PICKUP> : "WHITE"Pickup with slot id '"YELLOW"%d"WHITE"' has been deleted, total pickups: "GREEN"%d",slot,Iter_Count(DynamicPickups));
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
            }
            else SEM(playerid,"<ERROR> : Invalid pickup slot id!");
	    }
	    case 3:
	    {
			new slot,model;
			if(sscanf(subparam,"dd",slot,model)) return SEM(playerid,"<SYNTAX> : /pickup model [slot] [newmodel]");
			if(Iter_Contains(DynamicPickups,slot))
			{
				Streamer_SetIntData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_MODEL_ID,model);
				format(string,32,"[pickup slot: %d model: %d]",slot,model);
				UpdateDynamic3DTextLabelText(DynamicPickupLabel[slot],-1,string);
			    format(string,144,"<PICKUP> : "WHITE"Pickup slot id '"YELLOW"%d"WHITE"' model was changed to '"YELLOW"%d"WHITE"'",slot,model);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid pickup slot id!");
	    }
	    case 4:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup move [slot]");
            new slot = strval(subparam);
            if(Iter_Contains(DynamicPickups,slot))
            {
                new Float:cPos[3];
			    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
			    Streamer_SetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_X,cPos[0]);
			    Streamer_SetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_SetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_Z,cPos[2]);
			    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL,DynamicPickupLabel[slot],E_STREAMER_X,cPos[0]);
			    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL,DynamicPickupLabel[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL,DynamicPickupLabel[slot],E_STREAMER_Z,cPos[2]);
                format(string,144,"<PICKUP> : "WHITE"You have teleported pickup slot id '"YELLOW"%d"WHITE"' to your location!",slot);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
            }
            else SEM(playerid,"<ERROR> : Invalid pickup slot id!");
	    }
	    case 5:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup goto [slot]");
            new slot = strval(subparam);
            if(Iter_Contains(DynamicPickups,slot))
            {
                new Float:cPos[3];
			    Streamer_GetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_X,cPos[0]);
			    Streamer_GetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_GetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_Z,cPos[2]);
			    Streamer_UpdateEx(playerid,cPos[0],cPos[1],cPos[2]);
			    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
                format(string,144,"<PICKUP> : "WHITE"You have been teleported to pickup slot id '"YELLOW"%d"WHITE"'",slot);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
            }
            else SEM(playerid,"<ERROR> : Invalid pickup slot id!");
	    }
		case 6:
		{
		    if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup clear [confirm]");
			if(!strcmp(subparam,"confirm",true))
			{
			    new count = Iter_Count(DynamicPickups);
				if(count > 0)
				{
				    foreach(new slot : DynamicPickups)
				    {
				        DestroyDynamicPickup(DynamicPickup[slot]);
				        DestroyDynamic3DTextLabel(DynamicPickupLabel[slot]);
				    }
				    Iter_Clear(DynamicPickups);
				    format(string,144,"<PICKUP> : "YELLOW"%d pickups "WHITE"have been cleared",count);
					SendClientMessage(playerid,-1,string);
				}
				else SEM(playerid,"<ERROR> : There are no vehicles!");
			}
			else SEM(playerid,"<SYNTAX> : /veh clear [confirm]");
		}
		case 7:
		{
		    if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup import [filename]");
	        new filename[32],dir[64];
			strmid(filename,subparam,0,strlen(subparam),sizeof(filename));
			format(dir,sizeof(dir),"input/pickups/%s",filename);
			if(fexist(dir))
			{
			    new File:file = fopen(dir,io_read),
					data[256],model,Float:cPos[3],loaded;
				while(fread(file,data,sizeof(data)) > 0)
				{
				    new slot = Iter_Free(DynamicPickups);
				    if(slot == -1) break;
					strmid(string,data,(strfind(data,"(")+1),strfind(data,")"),sizeof(string));
				    if(strfind(data,"AddStaticPickup") != -1 || strfind(data,"CreatePickup") != -1)
				    {
				        unformat(string,"p<,>d{d}fff{d}",model,cPos[0],cPos[1],cPos[2]);
				    }
				    else continue;
					DynamicPickup[slot] = CreateDynamicPickup(model,24,cPos[0],cPos[1],cPos[2]);
					Iter_Add(DynamicPickups,slot);
					format(string,32,"[pickup slot: %d model: %d]",slot,model);
					DynamicPickupLabel[slot] =	CreateDynamic3DTextLabel(string,-1,cPos[0],cPos[1],cPos[2],100.0);
					loaded++;
				}
				fclose(file);
				Streamer_Update(playerid);
				format(string,144,"<PICKUP> : "YELLOW"%d pickups "WHITE"have been loaded from file '"GREEN"%s"WHITE"'",loaded,filename);
				SendClientMessage(playerid,-1,string);
			}
			else SEM(playerid,"<ERROR> : File does not exist!");
		}
		case 8:
		{
		    if(Iter_Count(DynamicPickups) == 0) return SEM(playerid,"<ERROR> : There are no pickup!");
		    if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /pickup export [filename]");
	        new filename[32],dir[64];
			strmid(filename,subparam,0,strlen(subparam),sizeof(filename));
			format(dir,sizeof(dir),"output/pickups/%s",filename);
		    new File:file = fopen(dir,io_write),
				model,Float:cPos[3];
			foreach(new slot : DynamicPickups)
			{
				model = Streamer_GetIntData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_MODEL_ID);
				Streamer_GetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_X,cPos[0]);
			    Streamer_GetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_GetFloatData(STREAMER_TYPE_PICKUP,DynamicPickup[slot],E_STREAMER_Z,cPos[2]);
				format(string,sizeof(string),"CreatePickup(%d, 23, %.4f, %.4f, %.4f);\r\n",model,cPos[0],cPos[1],cPos[2]);
                fwrite(file,string);
			}
			fclose(file);
			format(string,144,"<PICKUP> : "YELLOW"%d pickups "WHITE"have been exported to '"GREEN"%s"WHITE"'",Iter_Count(DynamicPickups),dir);
			SendClientMessage(playerid,-1,string);
		}
		default: SEM(playerid,"<SYNTAX> : /pickup [create/destroy/model/gethere/goto/clear/import/export]");
	}
	return 1;
}
CMD:mapicon(playerid,params[])
{
    new action,subparam[128],string[256];
	unformat(params,"k<mapiconmenu>S()[128]",action,subparam);
	switch(action)
	{
	    case 1:
	    {
			new type,red,green,blue,alpha;
			if(sscanf(subparam,"dD(255)D(255)D(255)D(255)",type,red,green,blue,alpha)) return SEM(playerid,"<SYNTAX> : /mapicon create [type (0-63)] [opt:red] [opt:green] [opt:blue] [opt:alpha]");
			new slot = Iter_Free(MapIcons);
			if(slot == -1) return SEM(playerid,"<ERROR> : No free map icon slot!");
			if(63 >= type >= 0)
			{
			    new Float:cPos[3];
			    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
			    Iter_Add(MapIcons,slot);
			    new color = RGBAToInt(red,green,blue,alpha);
			    MapIcon[slot] = CreateDynamicMapIcon(cPos[0],cPos[1],cPos[2],type,color);
			    format(string,64,"[mapicon slot: %d type: %d]",slot,type);
			    MapIconLabel[slot] = CreateDynamic3DTextLabel(string,-1,cPos[0],cPos[1],cPos[2],100.0);
			    format(string,144,"<MAPICON> : "WHITE"Created mapicon type '"YELLOW"%d"WHITE"' with slot '"YELLOW"%d"WHITE"', total mapicons: "GREEN"%d",type,slot,Iter_Count(MapIcons));
			    SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid mapicon type!");
	    }
	    case 2:
	    {
			if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /mapicon destroy [mapicon slot]");
			new slot = strval(subparam);
			if(Iter_Contains(MapIcons,slot))
			{
			    Iter_Remove(MapIcons,slot);
			    DestroyDynamicMapIcon(MapIcon[slot]);
			    DestroyDynamic3DTextLabel(MapIconLabel[slot]);
			    format(string,144,"<MAPICON> : "WHITE"Created mapicon slot '"YELLOW"%d"WHITE"' deleted, total mapicons: "GREEN"%d",slot,Iter_Count(MapIcons));
			    SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid mapicon slot!");
	    }
	    case 3:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /mapicon clear [confirm]");
	        new count = Iter_Count(MapIcons);
	        if(count == 0) return SEM(playerid,"<ERROR> : There are no mapicons!");
	        if(!strcmp(subparam,"confirm",true))
	        {
	            foreach(new slot : MapIcons)
	            {
	                DestroyDynamicMapIcon(MapIcon[slot]);
			    	DestroyDynamic3DTextLabel(MapIconLabel[slot]);
	            }
	            Iter_Clear(MapIcons);
				format(string,144,"<MAPICON> : "YELLOW"%d mapicons "WHITE"have been cleared!",count);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
	        }
	        else SEM(playerid,"<SYNTAX> : /mapicon clear [confirm]");
	    }
	    case 4:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /mapicon move [mapicon slot]");
			new slot = strval(subparam);
			if(Iter_Contains(MapIcons,slot))
			{
			    new Float:cPos[3];
			    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
			    Streamer_SetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_X,cPos[0]);
			    Streamer_SetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_SetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_Z,cPos[2]);
			    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL,MapIconLabel[slot],E_STREAMER_X,cPos[0]);
			    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL,MapIconLabel[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL,MapIconLabel[slot],E_STREAMER_Z,cPos[2]);
			    format(string,144,"<MAPICON> : "WHITE"Teleported mapicon slot '"YELLOW"%d"WHITE"' to your location",slot);
			    SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid mapicon slot!");
	    }
	    case 5:
	    {
	        new slot,type;
	        if(sscanf(subparam,"dd",slot,type)) return SEM(playerid,"<SYNTAX> : /mapicon type [mapicon slot] [newtype]");
			if(Iter_Contains(MapIcons,slot))
			{
			    if(63 >= type >= 0)
				{
				    Streamer_SetIntData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_TYPE,type);
				    format(string,64,"[mapicon slot: %d type: %d]",slot,type);
			    	UpdateDynamic3DTextLabelText(MapIconLabel[slot],-1,string);
				    format(string,144,"<MAPICON> : "WHITE"Mapicon slot '"YELLOW"%d"WHITE"' type was changed to '"YELLOW"%d"WHITE"'",slot,type);
				    SendClientMessage(playerid,X11_LIGHTBLUE,string);
				}
				else SEM(playerid,"<ERROR> : Invalid mapicon type!");
			}
			else SEM(playerid,"<ERROR> : Invalid mapicon slot!");
	    }
	    case 6:
	    {
	        new slot,red,green,blue,alpha;
	        if(sscanf(subparam,"ddddD(255)",slot,red,green,blue,alpha)) return SEM(playerid,"<SYNTAX> : /mapicon color [mapicon slot] [red] [green] [blue] [opt:alpha]");
			if(Iter_Contains(MapIcons,slot))
			{
			    new color = RGBAToInt(red,green,blue,alpha);
			    Streamer_SetIntData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_COLOR,color);
			    format(string,144,"<MAPICON> : "WHITE"Mapicon slot '"YELLOW"%d"WHITE"' color was changed",slot);
			    SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid mapicon slot!");
	    }
	    case 7:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /mapicon move [mapicon slot]");
			new slot = strval(subparam);
			if(Iter_Contains(MapIcons,slot))
			{
			    new Float:cPos[3];
			    Streamer_GetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_X,cPos[0]);
			    Streamer_GetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_GetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_Z,cPos[2]);
				Streamer_UpdateEx(playerid,cPos[0],cPos[1],cPos[2]);
				SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
			    format(string,144,"<MAPICON> : "WHITE"Teleported to mapicon slot '"YELLOW"%d"WHITE"'",slot);
			    SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid mapicon slot!");
	    }
	    case 8:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /mapicon import [filename]");
			new filename[32],dir[64];
			strmid(filename,subparam,0,strlen(subparam),sizeof(filename));
			format(dir,sizeof(dir),"input/mapicons/%s",filename);
			if(fexist(dir))
			{
			    new File:file = fopen(dir,io_read),
					data[256],type,color,Float:cPos[3],loaded;
				while(fread(file,data,sizeof(string)) > 0)
				{
				    new slot = Iter_Free(MapIcons);
					if(slot == -1) break;
					strmid(string,data,(strfind(data,"(")+1),strfind(data,")"),sizeof(string));
					if(strfind(data,"CreateDynamicMapIcon") != -1)
					{
						unformat(string,"p<,>fffdd",cPos[0],cPos[1],cPos[2],type,color);
					}
					else if(strfind(data,"SetPlayerMapIcon") != -1)
					{
					    unformat(string,"p<,>{sd}fffdd",cPos[0],cPos[1],cPos[2],type,color);
					}
					else continue;
					Iter_Add(MapIcons,slot);
				    MapIcon[slot] = CreateDynamicMapIcon(cPos[0],cPos[1],cPos[2],type,color);
				    format(string,64,"[mapicon slot: %d type: %d]",slot,type);
				    MapIconLabel[slot] = CreateDynamic3DTextLabel(string,-1,cPos[0],cPos[1],cPos[2],100.0);
				    loaded++;
				}
				fclose(file);
				Streamer_Update(playerid);
				format(string,144,"<MAPICON> : "YELLOW"%d mapicons "WHITE"have been loaded from file '"GREEN"%s"WHITE"'",loaded,filename);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : File does not exist!");
	    }
	    case 9:
		{
		    new count = Iter_Count(MapIcons);
		    if(count == 0) return SEM(playerid,"<ERROR> : There are no mapicon!");
		    new filename[32],mode[9];
		    if(sscanf(subparam,"s[32]S(samp)[9]",filename,mode)) return SEM(playerid,"<SYNTAX> : /mapicon export [filename] [opt:mode (samp/streamer)]");
	        new dir[64],bool:normal;
	        if(!strcmp(mode,"samp",true))
	        {
	            normal = true;
	        }
			format(dir,sizeof(dir),"output/mapicons/%s",filename);
		    new File:file = fopen(dir,io_write),
				type,color,Float:cPos[3];
			foreach(new slot : MapIcons)
			{
				type = Streamer_GetIntData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_TYPE);
				color = Streamer_GetIntData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_COLOR);
				Streamer_GetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_X,cPos[0]);
			    Streamer_GetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_Y,cPos[1]);
			    Streamer_GetFloatData(STREAMER_TYPE_MAP_ICON,MapIcon[slot],E_STREAMER_Z,cPos[2]);
			    if(normal)
			    {
			        format(string,sizeof(string),"SetPlayerMapIcon(playerid, %d, %.4f, %.4f, %.4f, %d, %d);\r\n",slot,cPos[0],cPos[1],cPos[2],type,color);
			    }
			    else
			    {
			        format(string,sizeof(string),"CreateDynamicMapIcon(%.4f, %.4f, %.4f, %d, %d);\r\n",cPos[0],cPos[1],cPos[2],type,color);
			    }
                fwrite(file,string);
			}
			fclose(file);
			format(string,144,"<PICKUP> : "YELLOW"%d mapicons "WHITE"have been exported to '"GREEN"%s"WHITE"'",count,dir);
			SendClientMessage(playerid,X11_LIGHTBLUE,string);
		}
	    default: SEM(playerid,"<SYNTAX> : /mapicon [create/destroy/clear/move/type/color/goto/import/export]");
	}
	return 1;
}
CMD:npcrecord(playerid,params[])
{
    new action,subparam[128],string[256];
	unformat(params,"k<npcmenu>S()[128]",action,subparam);
	switch(action)
	{
	    case 1:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /npcrecord start [name]");
	        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		    {
				strmid(RecordingFile[playerid],subparam,0,strlen(subparam),32);
				format(string,64,"%s.rec",RecordingFile[playerid]);
				if(fexist(string)) return SEM(playerid,"<ERROR> : Someone else is currently recording with that name!");
				RecordingState[playerid] = PLAYER_STATE_DRIVER;
		        Recording[playerid] = true;
		        StartRecordingPlayerData(playerid,PLAYER_RECORDING_TYPE_DRIVER,RecordingFile[playerid]);
				SendClientMessage(playerid,X11_LIGHTBLUE,"<NPCRECORD> : "WHITE"In vehicle recording started.");
		    }
		    else if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		    {
		        strmid(RecordingFile[playerid],subparam,0,strlen(subparam),32);
		        format(string,64,"%s.rec",RecordingFile[playerid]);
				if(fexist(string)) return SEM(playerid,"<ERROR> : Someone else is currently recording with that name!");
		        RecordingState[playerid] = PLAYER_STATE_ONFOOT;
		        Recording[playerid] = true;
		        StartRecordingPlayerData(playerid,PLAYER_RECORDING_TYPE_ONFOOT,RecordingFile[playerid]);
		        SendClientMessage(playerid,X11_LIGHTBLUE,"<NPCRECORD> : "WHITE"On foot recording started.");
		    }
		    else SEM(playerid,"<ERROR> : Unknown state");
	    }
	    case 2:
	    {
	        if(Recording[playerid])
	        {
	            Recording[playerid] = false;
	            StopRecordingPlayerData(playerid);
	            if(RecordingState[playerid] == PLAYER_STATE_ONFOOT)
				{
					new filename[64];
					format(filename,64,"scriptfiles/%s.rec",RecordingFile[playerid]);
					format(string,128,"scriptfiles/output/npc_recordings/onfoot/%s.rec",RecordingFile[playerid]);
					if(file_exists(string)) file_delete(string);
					file_move(filename,string);
					format(string,sizeof(string),"DELETE FROM `npc_recordings` WHERE `name`='%s'",RecordingFile[playerid]);
					new DB:database = db_open(MAIN_DATABASE);
					db_query(database,string);
					format(string,sizeof(string),"INSERT INTO `npc_recordings` VALUES('%s','%d','0')",RecordingFile[playerid],GetPlayerSkin(playerid));
					db_query(database,string);
					db_close(database);
					format(string,144,"<NPCRECORD> : "WHITE"Recording stopped, file saved to '"GREEN"output/npc_recordings/onfoot/%s.rec"WHITE"'",RecordingFile[playerid]);
				}
	            else
				{
				    new filename[64];
					format(filename,64,"scriptfiles/%s.rec",RecordingFile[playerid]);
					format(string,128,"scriptfiles/output/npc_recordings/invehicle/%s.rec",RecordingFile[playerid]);
					if(file_exists(string)) file_delete(string);
					file_move(filename,string);
					format(string,sizeof(string),"DELETE FROM `npc_recordings` WHERE `name`='%s'",RecordingFile[playerid]);
					new DB:database = db_open(MAIN_DATABASE);
					db_query(database,string);
					format(string,sizeof(string),"INSERT INTO `npc_recordings` VALUES('%s','%d','%d')",RecordingFile[playerid],GetPlayerSkin(playerid),GetVehicleModel(GetPlayerVehicleID(playerid)));
					db_query(database,string);
					db_close(database);
					format(string,144,"<NPCRECORD> : "WHITE"Recording stopped, file saved to '"GREEN"output/npc_recordings/invehicle/%s.rec"WHITE"'",RecordingFile[playerid]);
				}
	            SendClientMessage(playerid,X11_LIGHTBLUE,string);
	        }
	        else SEM(playerid,"<ERROR> : You are not recording!");
	    }
	    case 3:
	    {
	        if(NPCLoading) return SEM(playerid,"<ERROR> : Someone else is currently loading the NPC!");
            if(IsPlayerConnected(NPCTest[ID]) && IsPlayerNPC(NPCTest[ID])) return SEM(playerid,"<ERROR> : Stop the current testing in order to test another!");
	        new mode[8],filename[32],dir1[128],dir2[128];
			if(sscanf(subparam,"s[8]s[32]",mode,filename)) return SEM(playerid,"<SYNTAX> : /npcrecord test [onfoot/vehicle] [file]");
			if(!strcmp(mode,"onfoot",true))
			{
				format(dir1,128,"scriptfiles/output/npc_recordings/onfoot/%s.rec",filename);
				if(file_exists(dir1))
				{
				    format(dir2,128,"npcmodes/recordings/%s.rec",filename);
				    if(file_exists(dir2)) file_delete(dir2);
				    file_move(dir1,dir2);
				    format(string,256,"SELECT * FROM `npc_recordings` WHERE `name`='%s'",filename);
					new DB:database = db_open(MAIN_DATABASE);
					new DBResult:result = db_query(database,string);
					if(db_num_rows(result) > 0)
					{
						db_get_field(result,1,string,32);
						NPCTest[Skin] = strval(string);
						NPCTest[State] = PLAYER_STATE_ONFOOT;
						strmid(NPCTest[Record],filename,0,strlen(filename),32);
						NPCTest[VehID] = 0;
						ConnectNPC("Test_NPC","testnpc");
					}
					else
					{
					    NPCTest[Skin] = 299;
						NPCTest[State] = PLAYER_STATE_ONFOOT;
						strmid(NPCTest[Record],filename,0,strlen(filename),32);
						NPCTest[VehID] = 0;
						ConnectNPC("Test_NPC","testnpc");
					}
					db_free_result(result);
					db_close(database);
					format(string,144,"<NPCRECORD> : "WHITE"Testing "YELLOW"%s "WHITE"recording from file "GREEN"%s.rec",mode,filename);
					SendClientMessage(playerid,-1,string);
				}
				else SEM(playerid,"<ERROR> : File does not exist!");
			}
			else if(!strcmp(mode,"vehicle",true))
			{
			    format(dir1,128,"scriptfiles/output/npc_recordings/invehicle/%s.rec",filename);
				if(file_exists(dir1))
				{
				    format(dir2,128,"npcmodes/recordings/%s.rec",filename);
				    if(file_exists(dir2)) file_delete(dir2);
				    file_move(dir1,dir2);
				    format(string,256,"SELECT * FROM `npc_recordings` WHERE `name`='%s'",filename);
					new DB:database = db_open(MAIN_DATABASE);
					new DBResult:result = db_query(database,string);
					if(db_num_rows(result) > 0)
					{
						db_get_field(result,1,string,32);
						NPCTest[Skin] = strval(string);
						NPCTest[State] = PLAYER_STATE_DRIVER;
						strmid(NPCTest[Record],filename,0,strlen(filename),32);
						db_get_field(result,2,string,32);
						NPCTest[VehID] = CreateVehicle(strval(string),0.0,0.0,3.0,0.0,0,0,600000);
						NPCLoading = true;
						ConnectNPC("Test_NPC","testnpc");
					}
					else
					{
					    NPCTest[Skin] = 299;
						NPCTest[State] = PLAYER_STATE_DRIVER;
						strmid(NPCTest[Record],filename,0,strlen(filename),32);
						NPCTest[VehID] = CreateVehicle(400,0.0,0.0,3.0,0.0,0,0,600000);
						NPCLoading = true;
						ConnectNPC("Test_NPC","testnpc");
					}
					db_free_result(result);
					db_close(database);
					format(string,144,"<NPCRECORD> : "WHITE"Testing "YELLOW"%s "WHITE"recording from file "GREEN"%s.rec",mode,filename);
					SendClientMessage(playerid,-1,string);
				}
				else SEM(playerid,"<ERROR> : File does not exist!");
			}
			else SEM(playerid,"<ERROR> : Invalid mode, valid modes: onfoot,vehicle");
	    }
	    case 4:
	    {
	        if(IsPlayerConnected(NPCTest[ID]) && IsPlayerNPC(NPCTest[ID]))
	        {
	            Kick(NPCTest[ID]);
	            if(GetVehicleModel(NPCTest[VehID]) > 0)
	            {
	                DestroyVehicle(NPCTest[VehID]);
	            }
	            new dir1[128],dir2[128];
	            format(dir2,128,"npcmodes/recordings/%s.rec",NPCTest[Record]);
	            if(NPCTest[State] == PLAYER_STATE_ONFOOT)
	            {
                    format(dir1,128,"scriptfiles/output/npc_recordings/onfoot/%s.rec",NPCTest[Record]);
	            }
	            else
	            {
	                format(dir1,128,"scriptfiles/output/npc_recordings/invehicle/%s.rec",NPCTest[Record]);
	            }
	            file_move(dir2,dir1);
	        }
	    }
	    default: SEM(playerid,"<SYNTAX> : /npcrecord [start/stop/test/stoptest]");
	}
	return 1;
}