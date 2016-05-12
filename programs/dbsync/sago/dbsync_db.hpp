/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   dbsync_db.hpp
 * Author: poul
 *
 * Created on 12. maj 2016, 10:30
 */

#ifndef DBSYNC_DB_HPP
#define DBSYNC_DB_HPP

#include "dbsync.hpp"

namespace sago {
namespace database {

class DbSyncDb {
	virtual bool TableExists(const std::string& tablename) { return false; };
	virtual bool ColumnExists(const std::string& tablename, const std::string& columnname) { return false; };
	virtual bool ConstraintExists(const std::string& name) { return false; };
	virtual bool ForeignKeyExists(const std::string& name) { return false; };
	
	virtual void CreateTable(const DbTable& t) {};
	virtual void CreateColumn(const DbColumn& c) {};
	virtual void CreateUniqueConstraint(const DbUniqueConstraint& c) {};
	virtual void CreateForeignKeyConstraint(const DbForeignKeyConstraint& c) {};
	virtual ~DbSyncDb() {};
};

}
}

#endif /* DBSYNC_DB_HPP */

