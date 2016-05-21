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