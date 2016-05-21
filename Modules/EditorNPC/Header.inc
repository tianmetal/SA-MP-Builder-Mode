new bool:NPCLoading;
new bool:Recording[MAX_PLAYERS],
	RecordingState[MAX_PLAYERS],
	RecordingFile[MAX_PLAYERS][32];

enum E_NPC_INFO
{
	ID,
	Skin,
	State,
	Record[32],
	VehID
}
new NPCTest[E_NPC_INFO];