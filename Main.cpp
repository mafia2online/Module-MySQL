/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : M2Online-MySQL
* Special THX: Mabako for his IVMP-MySQL Module
* File       : Main.cpp
*
***************************************************************/

#include "Global.h"
#include "SDK.h"
#include "ScriptFunctions.h"

EXPORT bool ModuleInitialise( void )
{
	Log( "Initializing MySQL..." );

	return (true);
}

EXPORT void OnScriptLoad( HSQUIRRELVM S )
{
	RegisterFunction(S, "mysql_connect", ScriptFunctions::sq_mysql_connect);
	RegisterFunction(S, "mysql_close", ScriptFunctions::sq_mysql_close);
	RegisterFunction(S, "mysql_escape_string", ScriptFunctions::sq_mysql_escape_string);
	RegisterFunction(S, "mysql_ping", ScriptFunctions::sq_mysql_ping);
	RegisterFunction(S, "mysql_errno", ScriptFunctions::sq_mysql_errno);
	RegisterFunction(S, "mysql_error", ScriptFunctions::sq_mysql_error);
	RegisterFunction(S, "mysql_query", ScriptFunctions::sq_mysql_query);
	RegisterFunction(S, "mysql_free_result", ScriptFunctions::sq_mysql_free_result);
	RegisterFunction(S, "mysql_fetch_assoc", ScriptFunctions::sq_mysql_fetch_assoc);
	RegisterFunction(S, "mysql_insert_id", ScriptFunctions::sq_mysql_insert_id);
	RegisterFunction(S, "mysql_affected_rows", ScriptFunctions::sq_mysql_affected_rows);

	Log("[MySQL] : MySQL Module by Mafia2-Online loaded!");
}

EXPORT void OnScriptUnload( HSQUIRRELVM S )
{

}

EXPORT void Pulse( void )
{
}