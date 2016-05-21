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