/* 
 * File:   dbsync.hpp
 * Author: poul
 *
 * Created on 8. juli 2014, 14:51
 */

#ifndef DBSYNC_HPP
#define	DBSYNC_HPP

#include <cppdb/frontend.h>
#include <vector>

namespace sago {
	namespace database {
	
		enum DbType {TEXT,NUM,INTEGER,REAL,NONE};
		struct DbColumn {
			std::string name;
			DbType type = NONE;
			int length = 0;
			int precision = 0;
			bool nullable = false;
		};
		
		struct DbTable {
			std::string tablename;
			std::vector<DbColumn> columns;
		};
		
		struct DbUniqueConstraint {
			std::string name;
			std::string tablename;
			std::vector<std::string> columns;
		};
		
		struct DbForeignKeyConstraint {
			std::string name;
			std::string tablename;
			std::vector<std::string> columnnames;
			std::string foreigntablename;
			std::vector<std::string> foreigntablecolumnnames;
		};
		
		void SyncTableDataModel(cppdb::session &sql,const DbTable &table);

	} //database
}  // sago

#endif	/* DBSYNC_HPP */

