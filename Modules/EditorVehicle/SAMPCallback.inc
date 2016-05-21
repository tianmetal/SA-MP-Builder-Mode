#include <YSI\y_hooks>

hook OnGameModeExit()
{
	foreach(new vehid : DynamicVehicles)
	{
	    DestroyVehicle(vehid);
	}
	Iter_Clear(DynamicVehicles);
}