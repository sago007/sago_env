
#include "dbsync.hpp"

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

	} //database
}  // sago