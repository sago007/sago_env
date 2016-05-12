
#include "dbsync.hpp"

using namespace cppdb;

namespace sago {
	namespace database {
		
		const std::string& getTypeAsString(DbType type) {
			static std::string sINTEGER = "INTEGER";
			static std::string sTEXT = "TEXT";
			static std::string sNUM = "NUM";
			static std::string sREAL = "REAL";
			static std::string sNONE = "NONE";
			switch (type) {
				case INTEGER:
					return sINTEGER;
				case TEXT:
					return sTEXT;
				case NUM:
					return sNUM;
				case REAL:
					return sREAL;
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