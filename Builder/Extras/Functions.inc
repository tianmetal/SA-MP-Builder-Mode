GetXYLeftOfPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x - (distance * floatsin(-A-90.0,degrees));
	y2 = y - (distance * floatcos(-A-90.0,degrees));
}
GetXYRightOfPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x - (distance * floatsin(-A+90.0,degrees));
	y2 = y - (distance * floatcos(-A+90.0,degrees));
}
GetXYInFrontOfPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x + (distance * floatsin(-A,degrees));
	y2 = y + (distance * floatcos(-A,degrees));
}
GetXYBehindPoint(Float:x,Float:y,&Float:x2,&Float:y2,Float:A,Float:distance)
{
	x2 = x - (distance * floatsin(-A,degrees));
	y2 = y - (distance * floatcos(-A,degrees));
}
MoveDynamicObjectEx(objectid,Float:x,Float:y,Float:z,Float:speed)
{
	if(IsDynamicObjectMoving(objectid)) StopDynamicObject(objectid);
	return MoveDynamicObject(objectid,x,y,z,speed);
}