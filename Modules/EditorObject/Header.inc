#define MATERIAL_TYPE_NONE		0
#define MATERIAL_TYPE_TEXTURE	1
#define MATERIAL_TYPE_MESSAGE	2

#define OBJECT_SELECT_EDITOR	1
#define OBJECT_SELECT_DELETE	2
#define OBJECT_SELECT_PAINT		3
#define OBJECT_SELECT_CLEAN		4

new Iterator:DynamicObjects<MAX_EDITING_OBJECT>;
new DynamicObject[MAX_EDITING_OBJECT];
new ObjectEditor[MAX_EDITING_OBJECT];
new DynamicObjectMaterial[MAX_EDITING_OBJECT][MAX_OBJECT_MATERIAL_SLOT];
new DynamicObjectPriority[MAX_EDITING_OBJECT];
new SelectObjectType[MAX_PLAYERS];

new const WinFonts[15][20] = {
{"Arial"},
{"Calibri"},
{"Comic Sans MS"},
{"Georgia"},
{"Times New Roman"},
{"Consolas"},
{"Constantia"},
{"Corbel"},
{"Courier New"},
{"Impact"},
{"Lucida Console"},
{"Palatino Linotype"},
{"Tahoma"},
{"Trebuchet MS"},
{"Verdana"}
};