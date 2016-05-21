#define MATERIAL_TYPE_TEXTURE	1
#define MATERIAL_TYPE_MESSAGE	2

new Iterator:DynamicObjects<MAX_EDITING_OBJECT>;
new DynamicObject[MAX_EDITING_OBJECT];
new ObjectEditor[MAX_EDITING_OBJECT];
new ObjectMaterial[MAX_EDITING_OBJECT][MAX_OBJECT_MATERIAL_SLOT];

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