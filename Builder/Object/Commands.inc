SSCANF:objectmenu(string[])
{
	if(!strcmp(string,"create",true)) 				return 1;
	else if(!strcmp(string,"add",true)) 			return 1;
	else if(!strcmp(string,"destroy",true)) 		return 2;
	else if(!strcmp(string,"delete",true)) 			return 2;
	else if(!strcmp(string,"remove",true)) 			return 2;
	else if(!strcmp(string,"model",true)) 			return 3;
	else if(!strcmp(string,"clear",true)) 			return 4;
	else if(!strcmp(string,"reset",true)) 			return 4;
	else if(!strcmp(string,"move",true)) 			return 5;
	else if(!strcmp(string,"rotate",true)) 			return 6;
	else if(!strcmp(string,"rot",true)) 			return 6;
	else if(!strcmp(string,"copy",true)) 			return 7;
	else if(!strcmp(string,"duplicate",true)) 		return 7;
	else if(!strcmp(string,"select",true)) 			return 8;
	else if(!strcmp(string,"control",true)) 		return 8;
	else if(!strcmp(string,"goto",true)) 			return 9;
	else if(!strcmp(string,"tele",true)) 			return 9;
	else if(!strcmp(string,"gethere",true)) 		return 10;
	else if(!strcmp(string,"import",true)) 			return 11;
	else if(!strcmp(string,"export",true)) 			return 12;

	else if(!strcmp(string,"material",true))		return 13;
	else if(!strcmp(string,"settext",true))			return 14;
	else if(!strcmp(string,"textprop",true))		return 14;
	else if(!strcmp(string,"text",true))			return 14;
	else if(!strcmp(string,"clearmaterial",true))	return 15;
	return 0;
}
SSCANF:colour(string[])
{
	new color = 0;
	new red,green,blue,alpha;
	if(!sscanf(string,"dddD(255)",red,green,blue,alpha))
	{
		color = RGBAToInt(red,green,blue,alpha);
	}
	else
	{
		color = GetColour(string,0xFF);
	}
	return color;
}

CMD:object(playerid,params[])
{
	new action,subparam[128],string[256];
	unformat(params,"k<objectmenu>S()[128]",action,subparam);
	switch(action)
	{
	    case 1:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /object create [model]");
			new model = strval(subparam);
			new slot = Iter_Free(DynamicObjects);
			if(slot != -1)
			{
			    new Float:cPos[3];
			    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
				DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],0.0,0.0,0.0);
				Streamer_Update(playerid);
				ObjectEditor[slot] = INVALID_PLAYER_ID;
				SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+2.0));
				Iter_Add(DynamicObjects,slot);
				format(string,144,"<OBJECT> : "WHITE"Created object model '"YELLOW"%d"WHITE"' assigned to slot id '"YELLOW"%d"WHITE"', total objects: "GREEN"%d",model,slot,Iter_Count(DynamicObjects));
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : No object free slot!");
	    }
	    case 2:
	    {
            if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /object destroy [slot]");
            new slot = strval(subparam);
            if(Iter_Contains(DynamicObjects,slot))
            {
                DestroyDynamicObject(DynamicObject[slot]);
                Iter_Remove(DynamicObjects,slot);
                format(string,144,"<OBJECT> : "WHITE"Deleted object with slot id '"YELLOW"%d"WHITE"' has been deleted, total objects: "GREEN"%d",slot,Iter_Count(DynamicObjects));
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
            }
            else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 3:
	    {
			new slot,model;
			if(sscanf(subparam,"dd",slot,model)) return SEM(playerid,"<SYNTAX> : /object model [slot] [newmodel]");
			if(Iter_Contains(DynamicObjects,slot))
			{
				Streamer_SetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID,model);
			    format(string,144,"<OBJECT> : "WHITE"Object slot id '"YELLOW"%d"WHITE"' model was changed to '"YELLOW"%d"WHITE"'",slot,model);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 4:
	    {
			if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /object clear [confirm]");
			if(!strcmp(subparam,"confirm",true))
			{
			    new count = Iter_Count(DynamicObjects);
				if(count > 0)
				{
				    foreach(new i : DynamicObjects)
				    {
				        DestroyDynamicObject(DynamicObject[i]);
				    }
				    Iter_Clear(DynamicObjects);
				    format(string,144,"<OBJECT> : "YELLOW"%d objects "WHITE"have been deleted",count);
					SendClientMessage(playerid,X11_LIGHTBLUE,string);
				}
				else SEM(playerid,"<ERROR> : There are no objects!");
			}
			else SEM(playerid,"<SYNTAX> : /object clear [confirm]");
	    }
	    case 5:
	    {
			new slot,direction,Float:amount,Float:speed;
			if(sscanf(subparam,"dcfF(5.0)",slot,direction,amount,speed)) return SEM(playerid,"<SYNTAX> : /object move [slot] [direction (N,S,W,E,U,D)] [amount] [opt:speed]");
			if(Iter_Contains(DynamicObjects,slot))
			{
			    new Float:cPos[3],
			        objectid = DynamicObject[slot];
			    GetDynamicObjectPos(objectid,cPos[0],cPos[1],cPos[2]);
			    switch(direction)
			    {
			        case 'N','n': cPos[0] += amount;
			        case 'S','s': cPos[0] -= amount;
			        case 'W','w': cPos[1] += amount;
			        case 'E','e': cPos[1] -= amount;
			        case 'U','u': cPos[2] += amount;
			        case 'D','d': cPos[2] -= amount;
			    }
			    MoveDynamicObjectEx(objectid,cPos[0],cPos[1],cPos[2],speed);
			}
			else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 6:
	    {
	        new slot,Float:Rotation[3];
	        if(sscanf(subparam,"dfff",slot,Rotation[0],Rotation[1],Rotation[2])) return SEM(playerid,"<SYNTAX> : /object rotate [slot] [rotX] [rotY] [rotZ]");
	        if(Iter_Contains(DynamicObjects,slot))
	        {
				SetDynamicObjectRot(DynamicObject[slot],Rotation[0],Rotation[1],Rotation[2]);
	        }
	        else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 7:
	    {
			if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /object copy [slot]");
			new slot = strval(subparam);
			if(Iter_Contains(DynamicObjects,slot))
			{
			    new slot2 = Iter_Free(DynamicObjects);
			    if(slot2 == -1) return SEM(playerid,"<ERROR> : No object free slot!");
			    new model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID),
			        Float:cPos[3],Float:cRot[3];
				GetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
				GetDynamicObjectRot(DynamicObject[slot],cRot[0],cRot[1],cRot[2]);
				DynamicObject[slot2] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
				Iter_Add(DynamicObjects,slot2);
				format(string,144,"<OBJECT> : "WHITE"Object slot '"YELLOW"%d"WHITE"' duplicated to object slot '"YELLOW"%d"WHITE"', total objects: "GREEN"%d",slot,slot2,Iter_Count(DynamicObjects));
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 8:
	    {
	        if(GetPVarType(playerid,"EditingObject") > 0) return SEM(playerid,"<ERROR> : You must first release your current object!");
			if(IsNull(subparam)) SelectObject(playerid);
			else
			{
			    new slot = strval(subparam);
			    if(Iter_Contains(DynamicObjects,slot))
			    {
			        if((ObjectEditor[slot] != INVALID_PLAYER_ID) && (GetPVarType(ObjectEditor[slot],"EditingObject") == slot))
			        {
					    new playername[24];
					    GetPlayerName(ObjectEditor[slot],playername,24);
					    format(string,144,"<ERROR> : "RED"%s "GRAY"is currently editing this object!",playername);
						return SEM(playerid,string);
			        }
			        if(Streamer_IsItemVisible(playerid,STREAMER_TYPE_OBJECT,DynamicObject[slot]))
					{
						format(string,144,"<OBJECT> : "WHITE"Selected object "YELLOW"slot %d",slot);
	                    ObjectEditor[slot] = playerid;
	                    SetPVarInt(playerid,"EditingObject",slot);
	                    EditDynamicObject(playerid,DynamicObject[slot]);
	                    SendClientMessage(playerid,X11_LIGHTBLUE,string);
					}
					else SEM(playerid,"<ERROR> : That object is not streamed in!");
			    }
			    else SEM(playerid,"<ERROR> : Invalid object slot id!");
			}
	    }
	    case 9:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /object goto [slot]");
	        new slot = strval(subparam);
			if(Iter_Contains(DynamicObjects,slot))
			{
				new Float:cPos[3];
				GetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
				Streamer_UpdateEx(playerid,cPos[0],cPos[1],cPos[2]);
				SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+1.0));
				format(string,144,"<OBJECT> : "WHITE"Teleported to object "YELLOW"slot %d",slot);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 10:
	    {
	        if(IsNull(subparam)) return SEM(playerid,"<SYNTAX> : /object gethere [slot]");
	        new slot = strval(subparam);
			if(Iter_Contains(DynamicObjects,slot))
			{
				new Float:cPos[3];
				GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
				SetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
				Streamer_Update(playerid);
				SetPlayerPos(playerid,cPos[0],cPos[1],(cPos[2]+1.0));
				format(string,144,"<OBJECT> : "WHITE"Teleported object "YELLOW"id %d "WHITE"to your location!",slot);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid object slot id!");
	    }
	    case 11:
	    {
	        new mode,filename[64];
	        if(sscanf(subparam,"k<importmenu>s[32]",mode,filename)) return SEM(playerid,"<SYNTAX> : /object import [mode (samp,mta)] [name]");
			if(mode == 1)
			{
			    format(filename,sizeof(filename),"input/objects/samp/%s",filename);
			    if(fexist(filename))
			    {
			        new File:file = fopen(filename,io_read),
						data[256],loaded;
			        while(fread(file,data,sizeof(data)) > 0)
			        {
			            if(strfind(data,"CreateObject") == -1 && strfind(data,"CreateDynamicObject") == -1) continue;
						new slot = Iter_Free(DynamicObjects);
						if(slot == -1) break;
			            new model,Float:cPos[3],Float:cRot[3];
			            strmid(string,data,(strfind(data,"(")+1),strfind(data,")"),sizeof(string));
			            unformat(string,"p<,>dffffff",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
			            DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
			            ObjectEditor[slot] = INVALID_PLAYER_ID;
			            Iter_Add(DynamicObjects,slot);
			            loaded++;
			        }
			        fclose(file);
			        Streamer_Update(playerid);
			        format(string,144,"<OBJECT> : "YELLOW"%d objects "WHITE"has been imported from '"GREEN"%s"WHITE"'",loaded,filename);
					SendClientMessage(playerid,X11_LIGHTBLUE,string);
			    }
			    else SEM(playerid,"<ERROR> : File does not exist!");
			}
			else if(mode == 2)
			{
			    format(filename,sizeof(filename),"input/objects/mta/%s",filename);
			    if(fexist(filename))
			    {
			        new File:file = fopen(filename,io_read),
						data[512],loaded;
			        while(fread(file,data,sizeof(data)) > 0)
			        {
			            new model,Float:cPos[3],Float:cRot[3];
			            if(sscanf(data,"p<\">'object''model='d'posX='f'posY='f'posZ='f'rotX='f'rotY='f'rotZ='f",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2])) continue;
                        new slot = Iter_Free(DynamicObjects);
						if(slot == -1) break;
                        DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
                        ObjectEditor[slot] = INVALID_PLAYER_ID;
                        Iter_Add(DynamicObjects,slot);
                        loaded++;
			        }
			        fclose(file);
			        Streamer_Update(playerid);
			        format(string,144,"<OBJECT> : "YELLOW"%d objects "WHITE"has been imported from '"GREEN"%s"WHITE"'",loaded,filename);
					SendClientMessage(playerid,X11_LIGHTBLUE,string);
			    }
			    else SEM(playerid,"<ERROR> : File does not exist!");
			}
			else SEM(playerid,"<ERROR> : Invalid mode, valid modes: samp,mta");
	    }
	    case 12:
	    {
	        if(Iter_Count(DynamicObjects) == 0) return SEM(playerid,"<ERROR> : There are no objects!");
	        new mode,filename[32],world,interior,Float:distance;
	        if(sscanf(subparam,"k<exportmenu>s[32]D(-1)D(-1)F(300.0)",mode,filename,world,interior,distance)) return SEM(playerid,"<SYNTAX> : /object export [mode (samp,streamer)] [map name] [opt:world] [opt:interior] [opt:distance]");
			if(!strcmp(mode,"samp",true))
			{
			    new dir[64];
			    format(dir,sizeof(dir),"output/objects/%s.pwn",filename);
				new File:file = fopen(dir,io_write);
				foreach(new slot : DynamicObjects)
				{
					new model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID),
			        	Float:cPos[3],Float:cRot[3];
					GetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
					GetDynamicObjectRot(DynamicObject[slot],cRot[0],cRot[1],cRot[2]);
				    format(string,sizeof(string),"CreateObject(%d, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.1f);\r\n",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2],distance);
				    fwrite(file,string);
				}
				fclose(file);
				format(string,144,"<OBJECT> : "YELLOW"%d objects "WHITE"have been exported to '"GREEN"%s"WHITE"'",Iter_Count(DynamicObjects),dir);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else if(!strcmp(mode,"streamer",true))
			{
			    new dir[64];
			    format(dir,sizeof(dir),"output/objects/%s.pwn",filename);
				new File:file = fopen(dir,io_write);
				foreach(new slot : DynamicObjects)
				{
					new model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID),
			        	Float:cPos[3],Float:cRot[3];
					GetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
					GetDynamicObjectRot(DynamicObject[slot],cRot[0],cRot[1],cRot[2]);
				    format(string,sizeof(string),"CreateDynamicObject(%d, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %d, %d, -1, %.1f);\r\n",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2],world,interior,distance);
				    fwrite(file,string);
				}
				fclose(file);
				format(string,144,"<OBJECT> : "YELLOW"%d objects have been exported to '"GREEN"%s"WHITE"'",Iter_Count(DynamicObjects),dir);
				SendClientMessage(playerid,X11_LIGHTBLUE,string);
			}
			else SEM(playerid,"<ERROR> : Invalid mode, valid modes: samp,streamer");
	    }
	    case 13:
	    {
	    	new slot,index,modelid,txdname[32],texturename[32],color;
	    	if(sscanf(subparam,"ddds[32]s[32]D(0)",slot,index,modelid,txdname,texturename,color)) return SEM(playerid,"<SYNTAX> : /object material [object slot] [index] [modelid] [txd name] [texture name] [color]")
	    	if(index < 0 || index > MAX_OBJECT_MATERIAL_SLOT) return SEM(playerid,"<ERROR> : Material index cannot go below 0 or above "#MAX_OBJECT_MATERIAL_SLOT"!");
	    	if(Iter_Contains(DynamicObjects,slot))
			{
	    		ObjectMaterial[slot][index] = MATERIAL_TYPE_TEXTURE;
	    		SetDynamicObjectMaterial(DynamicObject[slot],index,modelid,txdname,texturename,color);
	    	}
	    	else SEM(playerid,"ERROR: Invalid object slot id");
	    }
	    case 14:
	    {
	    	new slot,index;
	    }
	    default:
	    {
	        SEM(playerid,"<SYNTAX> : /object [action]");
	        SEM(playerid,"<ACTIONS> : create,destroy,model,clear,move,rotate,copy,select,goto,gethere,import,export");
	    }
	}
	return 1;
}