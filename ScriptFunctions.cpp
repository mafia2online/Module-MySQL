/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : M2Online-MySQL
* File       : ScriptFunctions.cpp
*
***************************************************************/

#include "Global.h"
using namespace std;

list< MySQL* > handlers;
list< MySQL_Result* > results;

/* Checks if the parameter at index idx is a valid MySQL Handler, if so stores it in the 'handler' variable, otherwise returns an error and exits. */
#define CHECK_HANDLER(idx) \
	MySQL* handler = 0; \
	{ \
		SQUserPointer sqTemp; \
		sq_getuserpointer( vm, idx+1, &sqTemp ); \
		for( list< MySQL* >::iterator iter = handlers.begin( ); iter != handlers.end( ); ++ iter ) \
				{ \
			if( *iter == sqTemp ) \
						{ \
				handler = (MySQL*)sqTemp; \
				break; \
						} \
				} \
		if( handler == 0 ) \
				{ \
			Log( "Invalid MySQL-Handler" ); \
			sq_pushbool( vm, false ); \
			return 1; \
				} \
	}

/* Checks if the parameter at index idx is a valid MySQL Result, if so stores it in the 'result' variable, otherwise returns an error and exits. */
#define CHECK_RESULT(idx) \
	MySQL_Result* result = 0; \
	{ \
		SQUserPointer sqTemp; \
		sq_getuserpointer( vm, idx+1, &sqTemp ); \
		for( list< MySQL_Result* >::iterator iter = results.begin( ); iter != results.end( ); ++ iter ) \
				{ \
			if( *iter == sqTemp ) \
						{ \
				result = (MySQL_Result*)sqTemp; \
				break; \
						} \
				} \
		if( result == 0 ) \
				{ \
			Log( "Invalid MySQL-Result" ); \
			sq_pushbool( vm, false ); \
			return 1; \
				} \
	}

/* Checks if the function has AT LEAST the given number of parameters */
#define CHECK_PARAMS(count) \
	int numparams = sq_gettop(vm); \
	if( numparams <= count ) \
		{ \
		Log( "Invalid Parameter count, need %d, got %d", count, numparams - 1 ); \
		sq_pushbool( vm, false ); \
		return 1; \
		}

/* Obtains a string from the parameters */
#define GET_STRING(idx, to) \
	sq_getstring( vm, idx+1, &to )

int ScriptFunctions::sq_mysql_connect( HSQUIRRELVM vm )
{
	const char* hostname;
	const char* username;
	const char* password;
	const char* database;

	GET_STRING(1, hostname);
	GET_STRING(2, username);
	GET_STRING(3, password);
	GET_STRING(4, database);

	MySQL* handler = new MySQL(hostname, username, password, database);
	if (handler->OK())
	{
		handlers.push_back(handler);
		sq_pushuserpointer(vm, handler);
	}
	else
	{
		Log("MySQL Connection failed (#%d): %s", handler->Errno(), handler->Error());
		delete handler;
		sq_pushbool(vm, false);
	}
	return 1;
}

int ScriptFunctions::sq_mysql_close(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_HANDLER(1);

	handler->Close();
	handlers.remove(handler);
	delete handler;

	sq_pushbool(vm, true);
	return 1;
}

int ScriptFunctions::sq_mysql_escape_string(HSQUIRRELVM vm)
{
	CHECK_PARAMS(2);
	CHECK_HANDLER(1);

	const char* string;
	GET_STRING(2, string);

	size_t len = strlen(string);
	char* result = new char[(len << 1) + 1];
	size_t newlen = handler->EscapeString(string, result, len);
	sq_pushstring(vm, result, newlen);
	return 1;
}

int ScriptFunctions::sq_mysql_ping(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_HANDLER(1);

	sq_pushbool(vm, handler->Ping());
	return 1;
}

int ScriptFunctions::sq_mysql_errno(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_HANDLER(1);

	sq_pushinteger(vm, handler->Errno());
	return 1;
}

int ScriptFunctions::sq_mysql_error(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_HANDLER(1);

	const char* error = handler->Error();
	sq_pushstring(vm, error, strlen(error));
	return 1;
}

int ScriptFunctions::sq_mysql_query(HSQUIRRELVM vm)
{
	CHECK_PARAMS(2);
	CHECK_HANDLER(1);

	const char* query;
	GET_STRING(2, query);

	MySQL_Result* result = handler->Query(query);
	if (result){
		results.push_back(result);
		sq_pushuserpointer(vm, result);
	}
	else {
		sq_pushbool(vm, false);
	}
	return 1;
}

int ScriptFunctions::sq_mysql_free_result(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_RESULT(1);

	results.remove(result);
	delete result;

	sq_pushbool(vm, true);
	return 1;
}

int ScriptFunctions::sq_mysql_fetch_assoc(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_RESULT(1);

	if (!result->Empty())
	{
		MYSQL_ROW row = result->FetchRow();
		if (row)
		{
			/* Build our table */
			sq_newtable(vm);

			result->FieldSeek(0);
			MYSQL_FIELD* field;
			int i = 0;
			for (field = result->FetchField(), i = 0; field != 0; field = result->FetchField(), i++)
			{
				sq_pushstring(vm, field->name, field->name_length);
				if (row[i])
				{
					switch (field->type)
					{
					case MYSQL_TYPE_DECIMAL:
					case MYSQL_TYPE_NEWDECIMAL:
					case MYSQL_TYPE_FLOAT:
					case MYSQL_TYPE_DOUBLE:
						sq_pushfloat(vm, static_cast < float > (atof(row[i])));
						break;
					case MYSQL_TYPE_TINY:
					case MYSQL_TYPE_SHORT:
					case MYSQL_TYPE_LONG:
					case MYSQL_TYPE_LONGLONG:
					case MYSQL_TYPE_INT24:
					case MYSQL_TYPE_YEAR:
					case MYSQL_TYPE_BIT:
						sq_pushinteger(vm, atoi(row[i]));
						break;
					case MYSQL_TYPE_NULL:
						sq_pushnull(vm);
						break;
					default:
					case MYSQL_TYPE_VARCHAR:
					case MYSQL_TYPE_SET:
					case MYSQL_TYPE_VAR_STRING:
					case MYSQL_TYPE_STRING:
					case MYSQL_TYPE_TIMESTAMP:
					case MYSQL_TYPE_DATE:
					case MYSQL_TYPE_TIME:
					case MYSQL_TYPE_DATETIME:
					case MYSQL_TYPE_NEWDATE:
					case MYSQL_TYPE_TINY_BLOB:
					case MYSQL_TYPE_MEDIUM_BLOB:
					case MYSQL_TYPE_LONG_BLOB:
					case MYSQL_TYPE_BLOB:
						sq_pushstring(vm, row[i], strlen(row[i]));
						break;
					}
				}
				else
				{
					sq_pushnull(vm);
				}
				sq_rawset(vm, -3);
			}
		}
		else
			sq_pushbool(vm, false);
	}
	else
		sq_pushbool(vm, false);
	return 1;
}

int ScriptFunctions::sq_mysql_insert_id(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_HANDLER(1);

	sq_pushinteger(vm, handler->InsertID());
	return 1;
}

int ScriptFunctions::sq_mysql_affected_rows(HSQUIRRELVM vm)
{
	CHECK_PARAMS(1);
	CHECK_HANDLER(1);

	sq_pushinteger(vm, handler->AffectedRows());
	return 1;
}