#include <YSI\y_hooks>

hook OnGameModeInit()
{
	#if defined mysql_included
		Database = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_DATABASE, MYSQL_PASSWORD, MYSQL_PORT, .pool_size = MYSQL_POOL_SIZE);
	#else
		Database = db_open(SQLITE_DB_PATH);
	#endif
}

hook OnGameModeExit()
{
	#if defined mysql_included
		mysql_close(Database);
	#else
		db_close(Database);
	#endif
}