/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DbSyncDbMySql.cpp
 * Author: poul
 * 
 * Created on 20. maj 2016, 10:59
 */

#include "DbSyncDbMySql.hpp"

using std::string;
using std::vector;

DbSyncDbMySql::DbSyncDbMySql(std::shared_ptr<cppdb::session>& sql, const std::string& schema) : sql(sql), schema(schema) {
}

DbSyncDbMySql::DbSyncDbMySql(const DbSyncDbMySql& orig) {
}

DbSyncDbMySql::~DbSyncDbMySql() {
}

bool DbSyncDbMySql::TableExists(const std::string& tablename) {
	cppdb::result res = *sql << "SELECT table_name "
		"FROM information_schema.tables "
		"WHERE table_schema = ? "
		"AND table_name = ?" << schema << tablename;
	if (res.next()) {
		return true;
	}
	return false;
}

bool DbSyncDbMySql::ColumnExists(const std::string& tablename, const std::string& columnname) {
	cppdb::result res = *sql << "SELECT column_name FROM INFORMATION_SCHEMA.COLUMNS "
           "WHERE table_schema = ? AND TABLE_NAME=? AND column_name=?" << schema << tablename << columnname;
	if (res.next()) {
		return true;
	}
	return false;
}

bool DbSyncDbMySql::UniqueConstraintExists(const std::string& tablename, const std::string& name) {
	cppdb::result res = *sql << "SELECT CONSTRAINT_NAME "
		"FROM INFORMATION_SCHEMA.TABLE_CONSTRAINTS WHERE CONSTRAINT_TYPE IN ('PRIMARY KEY', 'UNIQUE') "
		"AND table_schema = ? AND TABLE_NAME=? AND CONSTRAINT_NAME = ?" << schema << tablename << name;
	if (res.next()) {
		return true;
	}
	return false;
}

bool DbSyncDbMySql::ForeignKeyExists(const std::string& tablename, const std::string& name) {
	cppdb::result res = *sql << "SELECT CONSTRAINT_NAME "
    "FROM INFORMATION_SCHEMA.REFERENTIAL_CONSTRAINTS WHERE CONSTRAINT_SCHEMA = ? AND TABLE_NAME = ? AND CONSTRAINT_NAME = ?" << schema << tablename << name;
	if (res.next()) {
		return true;
	}
	return false;
}

std::vector<std::string> DbSyncDbMySql::GetTableNames() {
	std::vector<std::string> ret;
	cppdb::result res = *sql << "SELECT table_name "
		"FROM information_schema.tables "
		"WHERE table_schema = ?" << schema;
	while (res.next()) {
		std::string value;
		res >> value;
		ret.push_back(value);
	}
	return ret;
}

std::vector<std::string> DbSyncDbMySql::GetColoumNamesFromTable(const std::string& tablename) {
	std::vector<std::string> ret;
	cppdb::result res = *sql << "SELECT column_name FROM INFORMATION_SCHEMA.COLUMNS "
           "WHERE table_schema = ? AND TABLE_NAME = ?" << schema << tablename;
	while (res.next()) {
		std::string value;
		res >> value;
		ret.push_back(value);
	}
	return ret;
}

std::vector<std::string> DbSyncDbMySql::GetUniqueConstraintNamesForTable(const std::string& tablename) {
	std::vector<std::string> ret;
	cppdb::result res = *sql << "SELECT CONSTRAINT_NAME FROM INFORMATION_SCHEMA.TABLE_CONSTRAINTS "
		"WHERE table_schema = ? AND TABLE_NAME = ? AND CONSTRAINT_TYPE IN ('PRIMARY KEY', 'UNIQUE')" << schema << tablename;
	while (res.next()) {
		std::string value;
		res >> value;
		ret.push_back(value);
	}
	return ret;
}

std::vector<std::string> DbSyncDbMySql::GetForeignKeyNamesForTable(const std::string& tablename) {
	std::vector<std::string> ret;
	cppdb::result res = *sql << "SELECT CONSTRAINT_NAME "
		"FROM INFORMATION_SCHEMA.REFERENTIAL_CONSTRAINTS WHERE CONSTRAINT_SCHEMA = ? AND TABLE_NAME = ?" << schema << tablename;
	while (res.next()) {
		std::string value;
		res >> value;
		ret.push_back(value);
	}
	return ret;
}

sago::database::DbColumn DbSyncDbMySql::GetColumn(const std::string& tablename, const std::string& columnname) {
	sago::database::DbColumn ret;
	cppdb::result res = *sql << "SELECT COLUMN_NAME,DATA_TYPE,CHARACTER_MAXIMUM_LENGTH,NUMERIC_PRECISION,NUMERIC_SCALE,IS_NULLABLE FROM INFORMATION_SCHEMA.COLUMNS "
           "WHERE table_schema = ? AND TABLE_NAME = ? AND COLUMN_NAME = ?" << schema << tablename << columnname; 
	if (res.next()) {
		string name;
		string data_type;
		int max_length = 0;
		int numeric_precision = 0;
		int numeric_scale = 0;
		string nullable;
		res >> name >> data_type >> max_length >> numeric_precision >> numeric_scale >> nullable;
		ret.name = name;
		bool type_recognized = false;
		if (data_type == "int" || data_type == "bigint") {
			ret.length = numeric_precision;
			ret.scale = numeric_scale;
			ret.type = sago::database::NUMBER;
			type_recognized = true;
		}
		if (data_type == "varchar") {
			ret.length = max_length;
			ret.type = sago::database::TEXT;
			type_recognized = true;
		}
		if (!type_recognized) {
			std::cerr << "Failure\n";
			throw sago::database::DbException("Unregognized type", string("This was not regonized: ")+data_type, tablename, schema);
		}
		if (nullable == "YES") {
			ret.nullable = true;
		}
		else {
			ret.nullable = false;
		}
	}
	return ret;
}

sago::database::DbTable DbSyncDbMySql::GetTable(const std::string& tablename) {
	sago::database::DbTable ret;
	ret.tablename = tablename;
	std::vector<std::string> column_names = GetColoumNamesFromTable(tablename);
	for (const string& s : column_names) {
		ret.columns.push_back(GetColumn(tablename, s));
	}
	return ret;
}

sago::database::DbUniqueConstraint DbSyncDbMySql::GetUniqueConstraint(const std::string& tablename, const std::string& name) {
	sago::database::DbUniqueConstraint ret;
	ret.tablename = tablename;
	ret.name = name;
	cppdb::result res = *sql << "select COLUMN_NAME from information_schema.STATISTICS "
			"WHERE INDEX_SCHEMA = ? AND TABLE_NAME = ? and index_name = ? ORDER BY SEQ_IN_INDEX" << schema << tablename << name;
	while (res.next()) {
		string value;
		res >> value;
		ret.columns.push_back(value);
	}
	return ret;
}

sago::database::DbForeignKeyConstraint DbSyncDbMySql::GetForeignKeyConstraint(const std::string& tablename, const std::string& name) {
	sago::database::DbForeignKeyConstraint ret;
	ret.tablename = tablename;
	ret.name = name;
	cppdb::result res = *sql << "SELECT COLUMN_NAME,REFERENCED_TABLE_NAME,REFERENCED_COLUMN_NAME "
		"FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE "
		"WHERE "
		"REFERENCED_TABLE_SCHEMA = ? "
		"AND TABLE_NAME = ? "
		"AND CONSTRAINT_NAME = ?"
		"ORDER BY POSITION_IN_UNIQUE_CONSTRAINT" << schema << tablename << name;
	while (res.next()) {
		string columnname;
		string reftablename;
		string refcolumnname;
		res >> columnname >> reftablename >> refcolumnname;
		ret.foreigntablename = reftablename;
		ret.columnnames.push_back(columnname);
		ret.foreigntablecolumnnames.push_back(refcolumnname);
	}
	return ret;
}

void  DbSyncDbMySql::CreateTable(const sago::database::DbTable& t) {
	std::string create_table_sql = "CREATE TABLE "+t.tablename+" ( " + this->sago_id + " SERIAL )";
	cppdb::statement st = *sql << create_table_sql;
	st.exec();
}