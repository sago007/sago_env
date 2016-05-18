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
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"

namespace sago {
	namespace database {
	
		enum DbType {TEXT,NUM,INTEGER,REAL,NONE};
		struct DbColumn {
			std::string name;
			DbType type = NONE;
			int length = 0;
			int precision = 0;
			bool nullable = false;
			
			template <class Archive>
			void serialize( Archive & ar )
			{
				ar( CEREAL_NVP(name), CEREAL_NVP(type), CEREAL_NVP(length), CEREAL_NVP(precision), CEREAL_NVP(nullable) );
			}
			
		};
		
		struct DbTable {
			std::string tablename;
			std::vector<DbColumn> columns;
			
			template <class Archive>
			void serialize( Archive & ar )
			{
				ar( CEREAL_NVP(tablename), CEREAL_NVP(columns) );
			}
		};
		
		struct DbUniqueConstraint {
			std::string name;
			std::string tablename;
			std::vector<std::string> columns;
			
			template <class Archive>
			void serialize( Archive & ar )
			{
				ar( CEREAL_NVP(name), CEREAL_NVP(tablename), CEREAL_NVP(columns) );
			}
		};
		
		struct DbForeignKeyConstraint {
			std::string name;
			std::string tablename;
			std::vector<std::string> columnnames;
			std::string foreigntablename;
			std::vector<std::string> foreigntablecolumnnames;
			
			template <class Archive>
			void serialize( Archive & ar )
			{
				ar( CEREAL_NVP(name), CEREAL_NVP(tablename), CEREAL_NVP(columnnames), CEREAL_NVP(foreigntablename), CEREAL_NVP(foreigntablecolumnnames) );
			}
		};
		
		void SyncTableDataModel(cppdb::session &sql,const DbTable &table);

	} //database
}  // sago

#endif	/* DBSYNC_HPP */

