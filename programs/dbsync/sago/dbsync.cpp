
#include "dbsync.hpp"
#include "dbsync_db.hpp"

using namespace cppdb;

namespace sago {
	namespace database {
		
		const std::string& getTypeAsString(DbType type) {
			static std::string sTEXT = "TEXT";
			static std::string sNUM = "NUMBER";
			static std::string sDATE = "DATE";
			static std::string sNONE = "NONE";
			switch (type) {
				case NUMBER:
					return sNUM;
				case TEXT:
					return sTEXT;
				case DATE:
					return sDATE;
				case NONE:
				default:
					return sNONE;
			}
			return sNONE;
		}
		
		void SyncTableDataModel(cppdb::session &sql,const DbTable &table) {
			std::string stsql = "CREATE TABLE IF NOT EXISTS " + table.tablename + "(";
			for (size_t i=0; i< table.columns.size();i++) {
			
			}
			stsql += ")";
			statement st = sql << stsql;
			st.exec();
		}
		
		
		DbDatabaseModel ExtractDataModel(DbSyncDb& db);
		DbDatabaseModel ExtractDataModelForTables(DbSyncDb& db, const std::vector<std::string>& table_names);
		
		DbDatabaseModel ExtractDataModel(DbSyncDb& db) {
			DbDatabaseModel ret;
			std::vector<std::string> tablenames = db.GetTableNames();
			ret = ExtractDataModelForTables(db, tablenames);
			return ret;
		}
		
		DbDatabaseModel ExtractDataModelForTables(DbSyncDb& db, const std::vector<std::string>& table_names) {
			DbDatabaseModel ret;
			for (const std::string& s : table_names) {
				ret.tables.push_back(db.GetTable(s));
				std::vector<std::string> unique_constraint_names = db.GetUniqueConstraintNamesForTable(s);
				std::vector<std::string> unique_foreign_keys_names = db.GetForeignKeyNamesForTable(s);
				for (const std::string& un : unique_constraint_names) {
					ret.unique_constraints.push_back(db.GetUniqueConstraint(s, un));
				}
				for (const std::string& fn : unique_foreign_keys_names) {
					ret.foreign_keys.push_back(db.GetForeignKeyConstraint(s, fn));
				}
			}
			return ret;
		}

	} //database
}  // sago