#include "handler.h"

MySQL::MySQL(const char* hostname, const char* username, const char* password, const char* database)
{
	m_ok = false;
	if ((m_handle = mysql_init(0)))
	{
		my_bool reconnect = false;
		mysql_options(m_handle, MYSQL_OPT_RECONNECT, &reconnect);

		if (mysql_real_connect(m_handle, hostname, username, password, database, 3306, 0, CLIENT_COMPRESS | CLIENT_SSL))
			m_ok = !mysql_select_db(m_handle, database);
	}
}

MySQL::~MySQL()
{
	Close();
}

void MySQL::Close()
{
	if (m_handle)
	{
		mysql_close(m_handle);
		m_handle = 0;
		m_ok = false;
	}
}

const size_t MySQL::EscapeString(const char* str, char* dest, size_t len)
{
	return mysql_real_escape_string(m_handle, dest, str, static_cast< unsigned long >(len));
}

const bool MySQL::Ping()
{
	return mysql_ping(m_handle) == 0;
}

MySQL_Result* MySQL::Query(const char* query)
{
	MYSQL_RES* res;

	if (mysql_real_query(m_handle, query, static_cast < unsigned long > (strlen(query))))
		return 0;

	res = mysql_store_result(m_handle);
	if (res == 0 && mysql_field_count(m_handle) > 0)
		return 0;

	MySQL_Result* result = new MySQL_Result(res);
	return result;
}

const int MySQL::InsertID()
{
	return static_cast < int > (mysql_insert_id(m_handle));
}

const int MySQL::AffectedRows()
{
	return static_cast < int > (mysql_affected_rows(m_handle));
}