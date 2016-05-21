#include <YSI\y_hooks>

hook OnGameModeInit()
{
	new DB:database = db_open(MAIN_DATABASE);
    db_query(database,"CREATE TABLE IF NOT EXISTS `users`(`name`,`skin`,`pos_x`,`pos_y`,`pos_z`,`rot_z`)");
    db_query(database,"CREATE TABLE IF NOT EXISTS `npc_recordings`(`name`,`skin`,`vehmodel`)");
    db_close(database);
	return 1;
}
hook OnPlayerDisconnect(playerid,reason)
{
	if(GMX) return 1;
	new playername[MAX_PLAYER_NAME],Float:cPos[4],string[256];
	GetPlayerName(playerid,playername,sizeof(playername));
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
public OnPlayerRequestClass(playerid, classid)
{
	if(IsPlayerNPC(playerid)) return 1;
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