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