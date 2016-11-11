#include "result.h"

MySQL_Result::MySQL_Result(MYSQL_RES* res)
{
	m_result = res;
}

MySQL_Result::~MySQL_Result()
{
	if (!Empty())
		mysql_free_result(m_result);
}

const bool MySQL_Result::Empty()
{
	return m_result == 0;
}

MYSQL_ROW MySQL_Result::FetchRow()
{
	if (!Empty())
		return mysql_fetch_row(m_result);
	return 0;
}

unsigned int MySQL_Result::FieldSeek(unsigned int offset)
{
	return mysql_field_seek(m_result, offset);
}

MYSQL_FIELD* MySQL_Result::FetchField()
{
	return mysql_fetch_field(m_result);
}