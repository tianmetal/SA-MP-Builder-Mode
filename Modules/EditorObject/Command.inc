SSCANF:objectmenu(string[])
{
    if(!strcmp(string,"create",true)) return 1;
    else if(!strcmp(string,"add",true)) return 1;
    else if(!strcmp(string,"delete",true)) return 2;
    else if(!strcmp(string,"destroy",true)) return 2;
    else if(!strcmp(string,"remove",true)) return 2;
    else if(!strcmp(string,"clear",true)) return 3;
    else if(!strcmp(string,"reset",true)) return 3;
    else if(!strcmp(string,"copy",true)) return 4;
    else if(!strcmp(string,"duplicate",true)) return 4;
    else if(!strcmp(string,"move",true)) return 5;
    else if(!strcmp(string,"rot",true)) return 6;
    else if(!strcmp(string,"rotate",true)) return 6;
    else if(!strcmp(string,"select",true)) return 7;
    else if(!strcmp(string,"control",true)) return 7;
    else if(!strcmp(string,"save",true)) return 8;
    else if(!strcmp(string,"load",true)) return 9;
    else if(!strcmp(string,"tele",true)) return 10;
    else if(!strcmp(string,"goto",true)) return 10;
    else if(!strcmp(string,"mgethere",true)) return 11;
    else if(!strcmp(string,"mmove",true)) return 12;
    else if(!strcmp(string,"export",true)) return 13;
    else if(!strcmp(string,"model",true)) return 14;
    else if(!strcmp(string,"attach",true)) return 15;
    else if(!strcmp(string,"gethere",true)) return 16;
    else if(!strcmp(string,"listmap",true)) return 17;
    else if(!strcmp(string,"maplist",true)) return 17;
    else if(!strcmp(string,"material",true)) return 18;
    else if(!strcmp(string,"import",true)) return 19;
	else if(!strcmp(string,"textprop",true)) return 20;
	else if(!strcmp(string,"resetmaterial",true)) return 21;
	else if(!strcmp(string,"clearmaterial",true))	return 21;
	else if(!strcmp(string,"deletemode",true)) return 22;
	else if(!strcmp(string,"rdelete",true)) return 23;
	else if(!strcmp(string,"rremove",true)) return 23;
	else if(!strcmp(string,"rdestroy",true)) return 23;
	else if(!strcmp(string,"paintbrush",true)) return 24;
	else if(!strcmp(string,"cleanbrush",true)) return 25;
    else if(!strcmp(string,"setpriority",true)) return 26;
    else if(!strcmp(string,"priority",true)) return 26;
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

/*CMD:object(playerid,params[])
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
			if(mode == 1)
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
			else if(mode == 2)
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
	    	if(sscanf(subparam,"ddds[32]s[32]D(0)",slot,index,modelid,txdname,texturename,color)) return SEM(playerid,"<SYNTAX> : /object material [object slot] [index] [modelid] [txd name] [texture name] [color]");
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
	    	if(sscanf(subparam,"dD(0)",slot,index)) return SEM(playerid,"<SYNTAX> : /object settext [object slot] [index]");
	    	if(index < 0 || index > MAX_OBJECT_MATERIAL_SLOT) return SEM(playerid,"<ERROR> : Material index cannot go below 0 or above "#MAX_OBJECT_MATERIAL_SLOT"!");
	    	if(Iter_Contains(DynamicObjects,slot))
			{
				if(ObjectMaterial[slot][index] != MATERIAL_TYPE_MESSAGE)
				{
					ObjectMaterial[slot][index] = MATERIAL_TYPE_MESSAGE;
	    			SetDynamicObjectMaterialText(DynamicObject[slot],index,"Text");
				}
	    		SetPVarInt(playerid,"EditingObject",slot);
	    		SetPVarInt(playerid,"EditingIndex",index);
	    		Dialog_Show(playerid,Object_TextMenu,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
	    	}
	    	else SEM(playerid,"ERROR: Invalid object slot id");
	    }
	    case 15:
	    {
	    	
	    }
	    default:
	    {
	        SEM(playerid,"<SYNTAX> : /object [action]");
	        SEM(playerid,"<ACTIONS> : create,destroy,model,clear,move,rotate,copy,select,goto,gethere,import,export");
	    }
	}
	return 1;
}*/

CMD:object(playerid,params[])
{
    new string[256],item,subparam[96];
    sscanf(params,"K<objectmenu>(0)S()[96]",item,subparam);
    switch(item)
    {
        case 1:
        {
            if(!IsNull(subparam))
            {
                new model = strval(subparam);
                new slot = Iter_Free(DynamicObjects);
                if(slot != cellmin)
                {
                    new Float:cPos[3];
                    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
                    DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],0.0,0.0,0.0);
                    DynamicObjectPriority[slot] = 0;
                    Iter_Add(DynamicObjects,slot);
                    Streamer_Update(playerid);
                    format(string,128,"OBJECT: Object created with ID %d, total object: %d",slot,Iter_Count(DynamicObjects));
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
                else SEM(playerid,"ERROR: Full slot!");
            }
            else SEM(playerid,"KEGUNAAN: /object create [object model id]");
        }
        case 2:
        {
            if(!IsNull(subparam))
            {
                new slot = strval(subparam);
                if(Iter_Contains(DynamicObjects,slot))
                {
                    Iter_Remove(DynamicObjects,slot);
                    DestroyDynamicObject(DynamicObject[slot]);
					Loop(i,MAX_OBJECT_MATERIAL_SLOT)
					{
						DynamicObjectMaterial[slot][i] = MATERIAL_TYPE_NONE;
					}
                    format(string,128,"OBJECT: Object with ID %d has been deleted, total object: %d",slot,Iter_Count(DynamicObjects));
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
                else SEM(playerid,"ERROR: Invalid objectid!");
            }
            else SEM(playerid,"KEGUNAAN: /object delete [object id]");
        }
        case 3:
        {
            if((!IsNull(subparam)) && (!strcmp(subparam,"confirm",true)))
            {
                foreach(new slot : DynamicObjects)
                {
                    DestroyDynamicObject(DynamicObject[slot]);
					Loop(i,MAX_OBJECT_MATERIAL_SLOT)
					{
						DynamicObjectMaterial[slot][i] = MATERIAL_TYPE_NONE;
					}
                }
                format(string,128,"OBJECT: %d objects have been cleared",Iter_Count(DynamicObjects));
                Iter_Clear(DynamicObjects);
                SendClientMessage(playerid,X11_YELLOW,string);
            }
            else SEM(playerid,"WARNING: Are you sure you want to clear all the objects ? (/object clear confirm)");
        }
        case 4:
        {
            if(!IsNull(subparam))
            {
                new slot = strval(subparam);
                if(Iter_Contains(DynamicObjects,slot))
                {
                    new slot2 = Iter_Free(DynamicObjects);
                    if(slot2 != cellmin)
                    {
                        new model,Float:cPos[3],Float:cRot[3];
                        model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,cPos[0]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,cPos[1]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,cPos[2]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_X,cRot[0]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Y,cRot[1]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Z,cRot[2]);
                        DynamicObject[slot2] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
                        DynamicObjectPriority[slot2] = 0;
                        Iter_Add(DynamicObjects,slot2);
						Loop(index,MAX_OBJECT_MATERIAL_SLOT)
						{
							if(DynamicObjectMaterial[slot][index] == MATERIAL_TYPE_TEXTURE)
							{
								new modelid,txdname[32],texturename[32],color;
								GetDynamicObjectMaterial(DynamicObject[slot],index,modelid,txdname,texturename,color);
								SetDynamicObjectMaterial(DynamicObject[slot2],index,modelid,txdname,texturename,color);
								DynamicObjectMaterial[slot2][index] = MATERIAL_TYPE_TEXTURE;
							}
							else if(DynamicObjectMaterial[slot][index] == MATERIAL_TYPE_MESSAGE)
							{
								new text[128],resolution,font[20],size,bold,fcolor,bcolor,alignment;
								GetDynamicObjectMaterialText(DynamicObject[slot],index,text,resolution,font,size,bold,fcolor,bcolor,alignment);
								SetDynamicObjectMaterialText(DynamicObject[slot2],index,text,resolution,font,size,bold,fcolor,bcolor,alignment);
								DynamicObjectMaterial[slot2][index] = MATERIAL_TYPE_MESSAGE;
							}
						}
                        Streamer_Update(playerid);
                        format(string,128,"OBJECT: Object with ID %d has been copied to object with ID %d, total object: %d",slot,slot2,Iter_Count(DynamicObjects));
                        SendClientMessage(playerid,X11_YELLOW,string);
                    }
                    else SEM(playerid,"ERROR: Full slot!");
                }
                else SEM(playerid,"ERROR: Invalid objectid!");
            }
            else SEM(playerid,"KEGUNAAN: /object copy [object id]");
        }
        case 5:
        {
            new slot,direction,Float:amount,Float:speed;
            if(!sscanf(subparam,"dcfF(5.0)",slot,direction,amount,speed))
            {
                if(floatround(speed) <= 0) return SEM(playerid,"ERROR: Speed cannot go below 1!");
                if(Iter_Contains(DynamicObjects,slot))
                {
                    new Float:cPos[4];
                    GetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Z,cPos[3]);
                    switch(direction)
                    {
                        case 'N','n': cPos[1] += amount;
                        case 'S','s': cPos[1] -= amount;
                        case 'E','e': cPos[0] += amount;
                        case 'W','w': cPos[0] -= amount;
                        case 'U','u': cPos[2] += amount;
                        case 'D','d': cPos[2] -= amount;
                        case 'L','l': GetXYLeftOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                        case 'R','r': GetXYRightOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                        case 'F','f': GetXYInFrontOfPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                        case 'B','b': GetXYBehindPoint(cPos[0],cPos[1],cPos[0],cPos[1],cPos[3],amount);
                        default: return SEM(playerid,"ERROR: Invalid direction!");
                    }
                    MoveDynamicObjectEx(DynamicObject[slot],cPos[0],cPos[1],cPos[2],speed);
                }
                else SEM(playerid,"ERROR: Invalid objectid!");
            }
            else SEM(playerid,"KEGUNAAN: /object move [object id] [directoion (N/S/E/W/U(p)/D(own))] [amount] [opt:speed = 5.0]");
        }
        case 6:
        {
            new slot,Float:cRot[3];
            if(!sscanf(subparam,"dfff",slot,cRot[0],cRot[1],cRot[2]))
            {
                if(Iter_Contains(DynamicObjects,slot))
                {
                    SetDynamicObjectRot(DynamicObject[slot],cRot[0],cRot[1],cRot[2]);
                }
            }
            else SEM(playerid,"KEGUNAAN: /object rotate [object id] [rotation X] [rotation Y] [rotation Z]");
        }
        case 7:
        {
        	if(GetPVarType(playerid,"EditingObject") > 0) return SEM(playerid,"<ERROR> : You must first release your current object!");
            if(!IsNull(subparam))
            {
                new slot = strval(subparam);
                if(Iter_Contains(DynamicObjects,slot))
                {
                    if(ObjectEditor[slot] != INVALID_PLAYER_ID)
                    {
                        new editor = ObjectEditor[slot];
						if(GetPVarType(editor,"EditingObject") == slot)
						{
							format(string,sizeof(string),"ERROR: %s is currently editing this object!",ReturnPlayerName(editor));
                            return SEM(playerid,string);
						}
                    }
                    format(string,sizeof(string),"OBJECT: Selected object id %d!",slot);
                    ObjectEditor[slot] = playerid;
                    SetPVarInt(playerid,"EditingObject",slot);
                    EditDynamicObject(playerid,DynamicObject[slot]);
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
                else SEM(playerid,"ERROR: Invalid object id!");
            }
            else 
            {
				SelectObjectType[playerid] = OBJECT_SELECT_EDITOR;
                SelectObject(playerid);
            }
        }
        case 10:
        {
            if(!IsNull(subparam))
            {
                new slot = strval(subparam);
                if(Iter_Contains(DynamicObjects,slot))
                {
                    new Float:cPos[3];
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,cPos[0]);
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,cPos[1]);
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,cPos[2]);
                    SetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
                    format(string,128,"OBJECT: Teleported to object id %d!",slot);
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
            }
        }
        case 11:
        {
            if((!IsNull(subparam)) && (!strcmp(subparam,"confirm",true)))
            {
                new Float:myPos[3],Float:Pos1[3],Float:Pos2[3],Float:newPos[3];
                GetPlayerPos(playerid,myPos[0],myPos[1],myPos[2]);
                foreach(new slot : DynamicObjects)
                {
                    if(slot == 0)
                    {
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,Pos1[0]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,Pos1[1]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,Pos1[2]);
                        SetDynamicObjectPos(DynamicObject[slot],myPos[0],myPos[1],myPos[2]);
                    }
                    else
                    {
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,Pos2[0]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,Pos2[1]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,Pos2[2]);
                        newPos[0] = (myPos[0]+(Pos2[0]-Pos1[0]));
                        newPos[1] = (myPos[1]+(Pos2[1]-Pos1[1]));
                        newPos[2] = (myPos[2]+(Pos2[2]-Pos1[2]));
                        SetDynamicObjectPos(DynamicObject[slot],newPos[0],newPos[1],newPos[2]);
                    }
                }
                format(string,128,"OBJECT: %d objects moved to your location!",Iter_Count(DynamicObjects));
                SendClientMessage(playerid,X11_YELLOW,string);
            }
            else SEM(playerid,"WARNING: Are you sure you want to clear all the objects ? (/object clear confirm)");
        }
        case 12:
        {
            new direction,Float:amount,Float:speed;
            if(!sscanf(subparam,"cfF(5.0)",direction,amount,speed))
            {
                if(floatround(speed) <= 0) return SEM(playerid,"ERROR: Speed cannot go below 1!");
                foreach(new slot : DynamicObjects)
                {
                    new Float:cPos[3];
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,cPos[0]);
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,cPos[1]);
                    Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,cPos[2]);
                    switch(direction)
                    {
                        case 'N','n': cPos[1] += amount;
                        case 'S','s': cPos[1] -= amount;
                        case 'E','e': cPos[0] += amount;
                        case 'W','w': cPos[0] -= amount;
                        case 'U','u': cPos[2] += amount;
                        case 'D','d': cPos[2] -= amount;
                        default: return SEM(playerid,"ERROR: Invalid direction!");
                    }
                    MoveDynamicObjectEx(DynamicObject[slot],cPos[0],cPos[1],cPos[2],speed);
                }
                format(string,128,"OBJECT: %d objects have been moved!",Iter_Count(DynamicObjects));
                SendClientMessage(playerid,X11_YELLOW,string);
            }
            else SEM(playerid,"KEGUNAAN: /object mmove [directoion (N/S/E/W/U(p)/D(own))] [amount] [opt:speed = 5.0]");
        }
        case 13:
        {
            if(!IsNull(params))
            {
            	new name[64];
            	format(name,sizeof(name),"output/objects/%s",params);
                if(Iter_Count(DynamicObjects) != 0)
                {
                	new File:hFile = fopen(name,io_write);
                    new model,Float:cPos[3],Float:cRot[3];
                    foreach(new slot : DynamicObjects)
                    {
                        model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID);
                        if(Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACHED_VEHICLE) != INVALID_VEHICLE_ID)
                        {
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACH_OFFSET_X,cPos[0]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACH_OFFSET_Y,cPos[1]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACH_OFFSET_Z,cPos[2]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACH_R_X,cRot[0]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACH_R_Y,cRot[1]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACH_R_Z,cRot[2]);
                            format(string,sizeof(string),"AttachObjectToVehicle(%d,vehicleid,%f,%f,%f,%f,%f,%f);\r\n",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
                        }
                        else
                        {
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,cPos[0]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,cPos[1]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,cPos[2]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_X,cRot[0]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Y,cRot[1]);
                            Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Z,cRot[2]);
                            format(string,sizeof(string),"CreateObject(%d,%f,%f,%f,%f,%f,%f);\r\n",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
                        }
                        fwrite(hFile,string);
                    }
                    fclose(hFile);
                    format(string,128,"OBJECT: %d objects saved to file %s",Iter_Count(DynamicObjects),name);
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
            }
        }
        case 14:
        {
            new slot,newmodel;
            if(!sscanf(subparam,"dd",slot,newmodel))
            {
                if(Iter_Contains(DynamicObjects,slot))
                {
                    Streamer_SetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID,newmodel);
                    Streamer_Update(playerid);
                }
                else SEM(playerid,"ERROR: Invalid object slot!");
            }
            else SEM(playerid,"KEGUNAAN: /object model [object id] [object model]");
        }
        case 15:
        {
            new slot,vid,Float:offsetx,Float:offsety,Float:offsetz,Float:rotX,Float:rotY,Float:rotZ;
            if(!sscanf(subparam,"ddF(0.0)F(0.0)F(0.0)F(0.0)F(0.0)F(0.0)",slot,vid,offsetx,offsety,offsetz,rotX,rotY,rotZ))
            {
                if(Iter_Contains(DynamicObjects,slot))
                {
                    if(GetVehicleModel(vid) > 0)
                    {
                        AttachDynamicObjectToVehicle(DynamicObject[slot],vid,offsetx,offsety,offsetz,rotX,rotY,rotZ);
                    }
                    else
                    {
                        Streamer_SetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_ATTACHED_VEHICLE,INVALID_VEHICLE_ID);
                    }
                }
                else SEM(playerid,"ERROR: Invalid object slot!");
            }
            else SEM(playerid,"KEGUNAAN: /object attach [object id] [offset X] [offset Y] [offset Z] [rotation X] [rotation Y] [rotation Z]");
        }
        case 16:
        {
            if(!IsNull(subparam))
            {
                new slot = strval(subparam);
                if(Iter_Contains(DynamicObjects,slot))
                {
                    new Float:cPos[3];
                    GetPlayerPos(playerid,cPos[0],cPos[1],cPos[2]);
                    SetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
                    format(string,128,"OBJECT: Teleported object id %d to your location!",slot);
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
            }
        }
        case 18:
        {
            new slot,index,model,txdname[32],texture[32],color[4];
            if(!sscanf(subparam,"ddds[32]s[32]D(0)D(0)D(0)D(0)",slot,index,model,txdname,texture,color[0],color[1],color[2],color[3]))
            {
				if((index >= MAX_OBJECT_MATERIAL_SLOT) || (index < 0)) return SEM(playerid,"ERROR: index cannot go below 0 or over 9!");
                if(Iter_Contains(DynamicObjects,slot))
                {
					if(model == 0)
					{
						model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID);
					}
                    SetDynamicObjectMaterial(DynamicObject[slot],index,model,txdname,texture,RGBAToInt(color[0],color[1],color[2],color[3]));
					DynamicObjectMaterial[slot][index] = MATERIAL_TYPE_TEXTURE;
                }
                else SEM(playerid,"ERROR: Invalid object slot!");
            }
            else SEM(playerid,"KEGUNAAN: /object material [object id] [index] [model] [txdname] [texture] [opt: alpha] [opt: red]  [opt: green]  [opt: blue]");
        }
        case 19:
        {
            new name[32],confirm[10];
            if(!sscanf(subparam,"s[32]S()[10]",name,confirm))
            {   
                if(IsNull(confirm)) return SEM(playerid,"WARNING: Are you sure you wanto to import this map ? (/object import [name] confirm)");
                if(Iter_Count(DynamicObjects) == 0)
                {
					new dir[64],model,Float:cPos[3],Float:cRot[3],loaded;
					format(dir,sizeof(dir),"input/objects/mta/%s.map",name);
                    if(fexist(dir))
					{
						new File:hFile = fopen(dir,io_read);
						while(fread(hFile,string,sizeof(string)) > 0)
						{
							if(!sscanf(string,"p<\">'object''model='d'posX='f'posY='f'posZ='f'rotX='f'rotY='f'rotZ='f",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2])) // "
							{
								new slot = Iter_Free(DynamicObjects);
								if(slot == cellmin) break;
								Iter_Add(DynamicObjects,slot);
								DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
								loaded++;
							}
						}
						fclose(hFile);
					}
					else
					{
						format(dir,sizeof(dir),"input/objects/samp/%s.pwn",name);
						if(fexist(dir))
						{
							new File:hFile = fopen(dir,io_read);
							new pos1,pos2,lastSlot;
							while(fread(hFile,string,sizeof(string)) > 0)
							{
								pos1 = strfind(string,"(",false);
								pos2 = strfind(string,")",false);
								if((pos1 != -1) && (pos2 != -1))
								{
									if((strfind(string,"CreateObject",false) != -1) || (strfind(string,"CreateDynamicObject",false) != -1))
									{
										strmid(string,string,(pos1+1),pos2,sizeof(string));
										if(!unformat(string,"p<,>dffffff",model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]))
										{
											new slot = lastSlot = Iter_Free(DynamicObjects);
											if(slot == cellmin) break;
											Iter_Add(DynamicObjects,slot);
											DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
											loaded++;
										}
									}
									else if((strfind(string,"SetObjectMaterial",false) != -1) || (strfind(string,"SetDynamicObjectMaterial",false) != -1))
									{
										new index,txdname[32],texture[32],color;
										strmid(string,string,(pos1+1),pos2,sizeof(string));
										if(!unformat(string,"p<,>{s[32]}dds[32]s[32]D(-1)",index,model,txdname,texture,color))
										{
											if(MAX_OBJECT_MATERIAL_SLOT > index >= 0)
											{
												SetDynamicObjectMaterial(DynamicObject[lastSlot],index,model,txdname,texture,color);
												DynamicObjectMaterial[lastSlot][index] = MATERIAL_TYPE_TEXTURE;
											}
										}
									}
									else if((strfind(string,"SetObjectMaterialText",false) != -1) || (strfind(string,"SetDynamicObjectMaterialText",false) != -1))
									{
										new index,text[128],resolution,font[32],fontsize,bold,fontcolor,color,alignment;
										strmid(string,string,(pos1+1),pos2,sizeof(string));
										if(!unformat(string,"p<,>{s[32]}ds[128]ds[32]D(24)D(1)D(-1)D(-16777216)D(1)",index,text,resolution,font,fontsize,bold,fontcolor,color,alignment))
										{
											if(MAX_OBJECT_MATERIAL_SLOT > index >= 0)
											{
												SetDynamicObjectMaterialText(DynamicObject[lastSlot],index,text,resolution,font,fontsize,bold,fontcolor,color,alignment);
												DynamicObjectMaterial[lastSlot][index] = MATERIAL_TYPE_MESSAGE;
											}
											
										}
									}
								}
							}
							fclose(hFile);
						}
					}
                    format(string,144,"OBJECT: %d objects loaded from file '%s'",loaded,dir);
                    SendClientMessage(playerid,X11_YELLOW,string);
                }
                else SEM(playerid,"ERROR: You must first clear your current map!");
            }
            else SEM(playerid,"KEGUNAAN: /object import [name] [confirm]");
        }
		case 20:
		{
			new slot,index;
            if(!sscanf(subparam,"dD(0)",slot,index))
            {
				if((index >= MAX_OBJECT_MATERIAL_SLOT) || (index < 0)) return SEM(playerid,"ERROR: index cannot go below 0 or over 9!");
                if(Iter_Contains(DynamicObjects,slot))
                {
					if(DynamicObjectMaterial[slot][index] != MATERIAL_TYPE_MESSAGE)
					{
						SetDynamicObjectMaterialText(DynamicObject[slot],index,"Text Here",OBJECT_MATERIAL_SIZE_256x128,"Arial",24,1,0xFFFFFFFF,0xFF000000);
						DynamicObjectMaterial[slot][index] = MATERIAL_TYPE_MESSAGE;
					}
					SetPVarInt(playerid,"EditingObject",slot);
					SetPVarInt(playerid,"EditingIndex",index);
					Dialog_Show(playerid,Object_TextMenu,DIALOG_STYLE_LIST,"Material Text","Text\nResolution\nFont\nFont Size\nToggle Bold\nFont Color\nBackground Color\nText Alignment\nReset","Select","Close");
                }
                else SEM(playerid,"ERROR: Invalid object slot!");
            }
            else SEM(playerid,"KEGUNAAN: /object textprop [object id] [index]");
		}
		case 21:
		{
			if(!IsNull(subparam))
            {
                new slot = strval(subparam);
                if(Iter_Contains(DynamicObjects,slot))
                {
					new model,Float:cPos[3],Float:cRot[3];
					model = Streamer_GetIntData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_MODEL_ID);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_X,cPos[0]);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Y,cPos[1]);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_Z,cPos[2]);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_X,cRot[0]);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Y,cRot[1]);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT,DynamicObject[slot],E_STREAMER_R_Z,cRot[2]);
                    DestroyDynamicObject(DynamicObject[slot]);
					Loop(index,MAX_OBJECT_MATERIAL_SLOT)
					{
						DynamicObjectMaterial[slot][index] = MATERIAL_TYPE_NONE;
					}
					DynamicObject[slot] = CreateDynamicObject(model,cPos[0],cPos[1],cPos[2],cRot[0],cRot[1],cRot[2]);
					Streamer_Update(playerid);
                }
                else SEM(playerid,"ERROR: Invalid objectid!");
            }
            else SEM(playerid,"KEGUNAAN: /object resetmaterial [object id]");
		}
		case 22:
		{
			SelectObjectType[playerid] = OBJECT_SELECT_DELETE;
			SendClientMessage(playerid,X11_WHITE,"EDITOR: Click an object to delete it!");
			SelectObject(playerid);
		}
		case 23:
		{
			new Float:radius;
			if(!sscanf(subparam,"f",radius))
			{
				new Float:cPos[3];
				new count = 0;
				radius = floatabs(radius);
				foreach(new slot : DynamicObjects)
				{
					GetDynamicObjectPos(DynamicObject[slot],cPos[0],cPos[1],cPos[2]);
					if(IsPlayerInRangeOfPoint(playerid,radius,cPos[0],cPos[1],cPos[2]))
					{
						new next;
						DestroyDynamicObject(DynamicObject[slot]);
						Loop(i,MAX_OBJECT_MATERIAL_SLOT)
						{
							DynamicObjectMaterial[slot][i] = MATERIAL_TYPE_NONE;
						}
						Iter_SafeRemove(DynamicObjects,slot,next);
						slot = next;
						count++;
					}
				}
				format(string,128,"OBJECT: %d objects has been deleted, total object: %d",count,Iter_Count(DynamicObjects));
                SendClientMessage(playerid,X11_YELLOW,string);
			}
			else SEM(playerid,"KEGUNAAN: /object rdelete [radius]");
		}
        case 24:
        {
            new index,model,txdname[32],texture[32],color[4];
            if(!sscanf(subparam,"dds[32]s[32]D(0)D(0)D(0)D(0)",index,model,txdname,texture,color[0],color[1],color[2],color[3]))
            {
				if((index >= MAX_OBJECT_MATERIAL_SLOT) || (index < 0)) return SEM(playerid,"ERROR: index cannot go below 0 or over 9!");
                format(string,sizeof(string),"%d|%d|%s|%s|%d",index,model,txdname,texture,RGBAToInt(color[0],color[1],color[2],color[3]));
                SetPVarString(playerid,"PaintParam",string);
                SelectObjectType[playerid] = OBJECT_SELECT_PAINT;
                SendClientMessage(playerid,X11_WHITE,"EDITOR: Click an object to paint it!");
                SelectObject(playerid);
            }
            else SEM(playerid,"KEGUNAAN: /object paintbrush [index] [model] [txdname] [texture] [opt: alpha] [opt: red]  [opt: green]  [opt: blue]");
        }
        case 25:
        {
            SelectObjectType[playerid] = OBJECT_SELECT_CLEAN;
            SendClientMessage(playerid,X11_WHITE,"EDITOR: Click an object to clean it!");
            SelectObject(playerid);
        }
        case 26:
        {
        	new slot,priority;
        	if(sscanf(params,"dd",slot,priority)) return SEM(playerid,"KEGUNAAN: /object priority [slot] [priority]");
        	if(Iter_Contains(DynamicObjects,slot))
        	{
        		if(priority > 5 || priority < 0) return SEM(playerid,"ERROR: Priority cannot go over 5 or below 0!");
        		format(string,144,"OBJECT: Object "RED"slot %d "YELLOW"priority is set to "GREEN"%d "YELLOW"from "CYAN"%d",slot,priority,DynamicObjectPriority[slot]);
        		DynamicObjectPriority[slot] = priority;
        		SendClientMessage(playerid,X11_YELLOW,string);
        	}
        }
        default:
        {
            SEM(playerid,"KEGUNAAN: /object [option]");
            SEM(playerid,"OPTIONS: create, remove, clear, copy, move, rotate, select, load, save, tele, mgethere, mmove");
            SEM(playerid,"OPTIONS: export, model, attach, gethere, listmap, material, import, textprop, resetmaterial, deletemode, rdelete");
            SEM(playerid,"OPTIONS: paintbrush, cleanbrush, priority");
        }
    }
    return 1;
}