/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : ScriptFunctions.h
*
***************************************************************/

#pragma once

#include	"SDK.h"

class ScriptFunctions
{
public:
	static	int		sq_mysql_connect(HSQUIRRELVM vm);
	static	int		sq_mysql_close(HSQUIRRELVM vm);
	static	int		sq_mysql_escape_string(HSQUIRRELVM vm);
	static	int		sq_mysql_ping(HSQUIRRELVM vm);
	static	int		sq_mysql_errno(HSQUIRRELVM vm);
	static	int		sq_mysql_error(HSQUIRRELVM vm);
	static	int		sq_mysql_query(HSQUIRRELVM vm);
	static	int		sq_mysql_free_result(HSQUIRRELVM vm);
	static	int		sq_mysql_fetch_assoc(HSQUIRRELVM vm);
	static	int		sq_mysql_insert_id(HSQUIRRELVM vm);
	static	int		sq_mysql_affected_rows(HSQUIRRELVM vm);
};