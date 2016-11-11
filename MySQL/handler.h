#pragma once

#include <string.h>
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "../Libraries/mysql.h"
#include "result.h"

class MySQL
{
public:
	MySQL( const char* hostname, const char* username, const char* password, const char* database );
	~MySQL( );

	void Close( );

	const bool			OK( ) { return m_ok; }
	const unsigned int	Errno( ) { return mysql_errno( m_handle ); }
	const char*			Error( ) { return mysql_error( m_handle ); }

	const size_t		EscapeString( const char* str, char* dest, size_t len );
	const bool			Ping( );

	MySQL_Result*		Query( const char* query );
	const int			InsertID( );
	const int			AffectedRows( );
private:
	MYSQL* m_handle;
	bool m_ok;
};