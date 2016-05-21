SSCANF:npcmenu(string[])
{
	if(!strcmp(string,"start",true)) 			return 1;
	else if(!strcmp(string,"stop",true)) 		return 2;
	else if(!strcmp(string,"test",true)) 		return 3;
	else if(!strcmp(string,"stoptest",true)) 	return 4;
	return 0;
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