/*==============================================================================
=================================[Builder Mode]=================================
=================================[by tianmetal]=================================
==============================================================================*/

/*=================================[Credits]====================================
Special Thanks to	:
	• SA:MP Team past & present & future

Thanks to			:
	• Y_Less    			for     sscanf, foreach, and YSI
	• Incognito  			for     streamer
	• Zeex       			for     zcmd
    • JaTochNietDan  		for 	FileManager
    • Emmet_		  		for 	easyDialog
    • All contributors		for 	all specified projects above
==============================================================================*/

/*=================================[Changelog]==================================
Version 0.1:
	- Initial release

Version 0.2:
	- Rewritten part of mode, gamemode now requires YSI 3.x or 4.x
	- Implemented YSI's y_colour for all color purposes
==============================================================================*/

//==================================[Settings]==================================

#define GAMEMODE_NAME		"BuilderMode"
#define GAMEMODE_VERSION    "0.2"
#define MAPNAME             "San Andreas"

#define MAX_EDITING_OBJECT			500
#define MAX_OBJECT_MATERIAL_SLOT	10
#define MAX_EDITING_VEHICLE			200
#define MAX_EDITING_PICKUP 			300
#define MAX_EDITING_ICON        	100

#define MAIN_DATABASE "internal/main.db"

//=================================[Libraries]==================================

#define GAMEMODE

#include <a_samp>

#include <sscanf2>      				// sscanf plugin by Y_Less
#include <streamer>     				// streamer plugin by Incognito
#include <filemanager>      			// FileManager plugin by JaTochNietDan

#include <YSI\y_iterate>    			// YSI by Y_Less
#include <YSI\y_commands>   			// YSI by Y_Less
#include <YSI\y_colors>					// YSI by Y_Less
#include <easyDialog>					// easyDialog by Emmet_

#include "Modules/Util"
#include "Modules/Player"
#include "Modules/EditorObject"
#include "Modules/EditorVehicle"
#include "Modules/EditorPickup"
#include "Modules/EditorMapIcon"

//===================================[Start]====================================

main()
{
	print("===/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/=====/====");
	print("==/=====/=====/=====/=====/=====[Builder Mode v0.2]=====/=====/=====/=====/=====");
	print("=/=====/=====/=====/=====/=====/==[by tianmetal]=/=====/=====/=====/=====/=====/");
	print("/=====/=====/=====/=====/=====/[Thank you for using]==/=====/=====/=====/=====/=");
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
	if(IsNull(password)) print("[warning] The server is not passworded, it is recommended to enable password!");
	if(GetServerVarAsInt("announce") == 1) print("[warning] It is recommended to set variable announce to 0!");
	return 1;
}
public OnPlayerConnect(playerid)
{
	new playername[MAX_PLAYER_NAME],string[144];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(string,sizeof(string),"[JOIN] "YELLOW"%s "WHITE"has connected to the server",playername);
	SendClientMessageToAll(X11_GREEN,string);
	return 1;
}
public OnPlayerDisconnect(playerid,reason)
{
	new playername[MAX_PLAYER_NAME],string[144];
	GetPlayerName(playerid,playername,sizeof(playername));
	format(string,sizeof(string),"[LEAVE] "YELLOW"%s "WHITE"has diconnected from the server",playername);
	SendClientMessageToAll(X11_RED,string);
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

//====================================[End]=====================================