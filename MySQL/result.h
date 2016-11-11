#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include "../Libraries/mysql.h"

class MySQL_Result
{
	friend class MySQL;

private:
	MySQL_Result(MYSQL_RES* res);

public:
	~MySQL_Result();
	const bool		Empty();

	MYSQL_ROW		FetchRow();
	unsigned int	FieldSeek(unsigned int offset);
	MYSQL_FIELD*	FetchField();

private:
	MYSQL_RES* m_result;
};