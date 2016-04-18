CMD:help(playerid,params[])
{
	SendClientMessage(playerid,X11_LIGHTBLUE,"COMMAND LIST:");
	SendClientMessage(playerid,X11_WHITE,"/mark /gotomark /setloc /skin /godmode /object /veh /pickup /mapicon /npcrecord /gmx");
	SendClientMessage(playerid,X11_WHITE,"/lt /rt /ft /bt /up /dn /fly /kill /fix /flip /teleport /weapon");
	return 1;
}
CMD:mark(playerid,params[])
{
	new Float:cPos[4],string[128];
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    new vehid = GetPlayerVehicleID(playerid);
	    GetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
		GetVehicleZAngle(vehid,cPos[3]);
	}
	else
	{
		GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
		GetPlayerFacingAngle(playerid,cPos[3]);
	}
	format(string,sizeof(string),"%.2f,%.2f,%.2f,%.2f",cPos[0],cPos[1],cPos[2],cPos[3]);
	if(IsNull(params))
	{
	    SetPVarString(playerid,"Mark",string);
	    SendClientMessage(playerid,X11_LIGHTBLUE,"<MARKER> : "WHITE"Location marked!");
	}
	else
	{
	    new markerid = strval(string);
	    if(10 >= markerid > 0)
	    {
	        new var[16];
	        format(var,16,"Mark_%d",markerid);
	        SetPVarString(playerid,"MarkedPos",string);
	        format(string,sizeof(string),"<MARKER> : "WHITE"Location "YELLOW"slot %d "WHITE"marked!",markerid);
	    	SendClientMessage(playerid,X11_LIGHTBLUE,string);
	    }
	    else SEM(playerid,"<ERROR> : Markerid cannot go under 1 or over 10!");
	}
	return 1;
}
CMD:gotomark(playerid,params[])
{
    new Float:cPos[4],string[128];
	if(IsNull(params))
	{
	    if(GetPVarType(playerid,"Mark") > 0)
	    {
		    GetPVarString(playerid,"Mark",string,sizeof(string));
		    unformat(string,"e<p<,>ffff>",cPos);
		    Streamer_UpdateEx(playerid,cPos[0],cPos[1],cPos[2]);
		    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		    {
		        new vehid = GetPlayerVehicleID(playerid);
		        SetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
		        SetVehicleZAngle(vehid,cPos[3]);
		    }
		    else
		    {
		    	SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
		    	SetPlayerFacingAngle(playerid,cPos[3]);
		    	SetCameraBehindPlayer(playerid);
			}
		    SendClientMessage(playerid,X11_LIGHTBLUE,"<MARKER> : "WHITE"Teleported to marker!");
		}
		else SEM(playerid,"<ERROR> : You have no marked location!");
	}
	else
	{
	    new markerid = strval(string);
	    if(10 >= markerid > 0)
	    {
	        new var[16];
	        format(var,16,"Mark_%d",markerid);
	        if(GetPVarType(playerid,var) > 0)
	        {
		        GetPVarString(playerid,var,string,64);
			    unformat(string,"e<p<,>ffff>",cPos);
			    Streamer_UpdateEx(playerid,cPos[0],cPos[1],cPos[2]);
			    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			    {
			        new vehid = GetPlayerVehicleID(playerid);
			        SetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
			        SetVehicleZAngle(vehid,cPos[3]);
			    }
			    else
			    {
			    	SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
			    	SetPlayerFacingAngle(playerid,cPos[3]);
			    	SetCameraBehindPlayer(playerid);
				}
			    format(string,sizeof(string),"<MARKER> : "WHITE"Teleported to markerid "YELLOW"slot %d",markerid);
			    SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : You have no marked location!");
	    }
	    else SEM(playerid,"<ERROR> : Markerid cannot go under 1 or over 10!");
	}
	return 1;
}
CMD:setloc(playerid,params[])
{
	new Float:cPos[3],interior;
	if(sscanf(params,"fffD(-1)",cPos[0],cPos[1],cPos[2],interior)) return SEM(playerid,"<SYNTAX> : /setloc [X] [Y] [Z] [opt:interior]");
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    new vehid = GetPlayerVehicleID(playerid);
	    if(interior >= 0)
		{
			LinkVehicleToInterior(vehid,interior);
			SetPlayerInterior(playerid,interior);
		}
		SetVehiclePos(vehid,cPos[0],cPos[1],cPos[2]);
	}
	else
	{
		if(interior >= 0) SetPlayerInterior(playerid,interior);
		SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	}
	SendClientMessage(playerid,X11_LIGHTBLUE,"<SETLOC> : "WHITE"You have been teleported!");
	return 1;
}
CMD:skin(playerid,params[])
{
	if(IsNull(params)) return SEM(playerid,"<SYNTAX> : /skin [skinid (0-299)]");
	new skinid = strval(params);
	if(300 > skinid >= 0)
	{
	    new string[128];
	    SetPlayerSkin(playerid,skinid);
	    format(string,sizeof(string),"<SKIN> : "WHITE"Changed skin to "YELLOW"skinid %d",skinid);
		SendClientMessage(playerid,X11_LIGHTBLUE,string);
	}
	else SEM(playerid,"<ERROR> : Invalid skinid!");
	return 1;
}
CMD:godmode(playerid,params[])
{
	GodMode[playerid] = !(GodMode[playerid]);
	if(GodMode[playerid])
	{
	    SetPlayerHealth(playerid,100.0);
	    SendClientMessage(playerid,X11_LIGHTBLUE,"<GODMODE> : "RED"OFF");
	}
	else SendClientMessage(playerid,X11_LIGHTBLUE,"<GODMODE> : "GREEN"ON");
	return 1;
}
CMD:lt(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYLeftOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}
CMD:rt(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYRightOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}
CMD:ft(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYInFrontOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}
CMD:bt(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYBehindPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
	return 1;
}
CMD:up(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[3];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+cDistance));
	return 1;
}
CMD:dn(playerid,params[])
{
    new Float:cDistance = 2.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[3];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]-cDistance));
	return 1;
}
CMD:fly(playerid,params[])
{
    new Float:cDistance = 15.0;
    if(!IsNull(params))
    {
        cDistance = floatstr(params);
    }
    new Float:cPos[4];
    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
    GetPlayerFacingAngle(playerid,cPos[3]);
    GetXYInFrontOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],cDistance);
    SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+(cDistance/3.0)));
	return 1;
}
CMD:kill(playerid,params[])
{
	SetPlayerHealth(playerid,0.0);
	return 1;
}
CMD:fix(playerid,params[])
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    RepairVehicle(GetPlayerVehicleID(playerid));
	}
	return 1;
}
CMD:flip(playerid,params[])
{
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    new vehid = GetPlayerVehicleID(playerid),
			Float:zAngle;
	    GetVehicleZAngle(vehid,zAngle);
	    SetVehicleZAngle(vehid,zAngle);
	}
	return 1;
}
CMD:teleport(playerid,params[])
{
    new string[2744];
    format(string,sizeof(string),"Four Dragons' Managerial Suite\nGanton Gym\nBrothel\nBrothel2\nInside Track Betting\nBlastin' Fools Records\nThe Big Spread Ranch\nWarehouse 1\nWarehouse 2\nB Dup's Apartment\nB Dup's ");
    format(string,sizeof(string),"%sCrack Palace\nWheel Arch Angels\nOG Loc's House\nBarber Shop\nPlanning Department\nLas Venturas Police Department\nPro-Laps\nSex Shop\nLas Venturas Tattoo parlor\nLost San Fierro Tattoo ",string);
    format(string,sizeof(string),"%sparlor\n24/7 (version 1)\nDiner 1\nPizza Stack\nRusty Brown's Donuts\nAmmu-nation\nVictim\nLoco Low Co\nSan Fierro Police Department\n24/7 (version 2 - large)\nBelow The Belt Gym (Las ",string);
    format(string,sizeof(string),"%sVenturas)\nTransfenders\nWorld of Coq\nAmmu-nation (version 2)\nSubUrban\nDenise's Bedroom\nHelena's Barn\nBarbara's Love nest\nSan Fierro Garage\nOval Stadium\n8-Track Stadium\nThe Pig ",string);
    format(string,sizeof(string),"%sPen (strip club 2)\nFour Dragons\nLiberty City\nRyder's house\nSweet's House\nRC Battlefield\nThe Johnson House\nBurger shot\nCaligula's Casino\nKatie's Lovenest\nBarber Shop 2 ",string);
    format(string,sizeof(string),"%s(Reece's)\nAngel Pine Trailer\n24/7 (version 3)\nZip\nThe Pleasure Domes\nMadd Dogg's Mansion\nBig Smoke's Crack Palace\nBurning Desire Building\nWu-Zi Mu's\nAbandoned AC tower",string);
    format(string,sizeof(string),"%s\nWardrobe/Changing room\nDidier Sachs\nCasino (Redsands West)\nKickstart Stadium\nClub\nAtrium\nLos Santos Tattoo Parlor\nSafe House group 1\nSafe House group 2\nSafe House group 3\nSafe ",string);
    format(string,sizeof(string),"%sHouse group 4\nSherman Dam\n24/7 (version 4)\nJefferson Motel\nJet Interior\nThe Welcome Pump\nBurglary House X1\nBurglary House X2\nBurglary House X3\nBurglary House X4\nBinco",string);
    format(string,sizeof(string),"%s\n4 Burglary houses\nBlood Bowl Stadium\nBudget Inn Motel Room\nPalamino Bank\nPalamino Diner\nDillimore Gas Station\nLil' Probe Inn\nTorreno's Ranch\nZombotech - lobby area\nCrypt in LS ",string);
    format(string,sizeof(string),"%scemetery (temple)\nBlueberry Liquor Store\nPair of Burglary Houses\nCrack Den\nBurglary House X11\nBurglary House X12\nAmmu-nation (version 3)\nJay's Diner\n24/7 (version 5)\nWarehouse ",string);
    format(string,sizeof(string),"%s3\nMichelle's Love Nest*\nBurglary House X14\nSindacco Abatoir\nK.A.C.C. Military Fuels Depot\nArea 69\nBurglary House X13\nUnused Safe House\nMillie's Bedroom\nBarber Shop",string);
    format(string,sizeof(string),"%s\nDirtbike Stadium\nCobra Gym\nLos Santos Police Department\nLos Santos Airport\nBurglary House X15\nBurglary House X16\nBurglary House X17\nBike School\nFrancis International Airport",string);
    format(string,sizeof(string),"%s\nVice Stadium\nBurglary House X18\nBurglary House X19\nZero's RC Shop\nAmmu-nation (version 4)\nAmmu-nation (version 5)\nBurglary House X20\n24/7 (version 6)\nSecret Valley Diner\nRosenberg's Office in Caligulas\nFanny Batter's ",string);
    format(string,sizeof(string),"%sWhore House\nColonel Furhberger's\nCluckin' Bell\nThe Camel's Toe Safehouse\nCaligula's Roof\nOld Venturas Strip Casino\nDriving School\nVerdant Bluffs Safehouse\nBike School\nAndromada\nFour ",string);
    format(string,sizeof(string),"%sDragons' Janitor's Office\nBar\nBurglary House X21\nWillowfield Safehouse\nBurglary House X22\nBurglary House X23\nOtto's Autos\nWang Cars",string);
    ShowPlayerDialog(playerid,DIALOG_TELEPORT,DIALOG_STYLE_LIST,"Interiors",string,"Teleport","Close");
    return 1;
}
CMD:weapon(playerid,params[])
{
	new weaponid,ammo;
	if(sscanf(params,"dD(500)",weaponid,ammo)) return SEM(playerid,"<SYNTAX> : /weapon [weaponid] [opt:ammo]");
    if (0 <= weaponid <= 18 || 22 <= weaponid <= 46)
    {
        GivePlayerWeapon(playerid,weaponid,ammo);
    }
	return 1;
}
CMD:gmx(playerid,params[])
{
	new playername[24],string[256];
	GetPlayerName(playerid,playername,24);
	format(string,144,"<GMX> : "YELLOW"%s "WHITE"is restarting the server",playername);
	SendClientMessageToAll(X11_LIGHTBLUE,string);
	new DB:database = db_open(MAIN_DATABASE);
	foreach(new player : Player)
	{
		GetPlayerName(player,playername,24);
		new Float:cPos[4];
		GetPlayerPos(player,cPos[0],cPos[1],cPos[2]);
		GetPlayerFacingAngle(player,cPos[3]);
		strToLower(playername);
		format(string,sizeof(string),"UPDATE `users` SET `skin`='%d',`pos_x`='%f',`pos_y`='%f',`pos_z`='%f',`rot_z`='%f' WHERE `name`='%s'",
		GetPlayerSkin(player),cPos[0],cPos[1],cPos[2],cPos[3],playername);
		db_query(database,string);
	}
	db_close(database);
	GMX = true;
	GameModeExit();
	return 1;
}