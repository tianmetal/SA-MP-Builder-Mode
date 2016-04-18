#if defined mysql_included
	#define MYSQL_HOST				"localhost"
	#define MYSQL_USER 				"root"
	#define MYSQL_DATABASE			"builder"
	#define MYSQL_PASSWORD			""
	#define MYSQL_PORT				3306
	#define MYSQL_POOL_SIZE			2
#else
	#define SQLITE_DB_PATH 			"builder.db"
#endif

#define TABLE_PREFIX				"builder_"

#define DB_TABLE_USERS				"users"
#define DB_TABLE_PROJECTS			"projects"
#define DB_TABLE_OBJECTS			"objects"
#define DB_TABLE_VEHICLES			"vehicles"
#define DB_TABLE_MAP_ICONS			"mapicons"
#define DB_TABLE_PICKUPS			"pickups"
#define DB_TABLE_GANGZONES			"gangzones"
#define DB_TABLE_OBJECT_MATERIALS	"object_materials"
#define DB_TABLE_NPCS				"npcs"