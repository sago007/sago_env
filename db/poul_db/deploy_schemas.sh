sago_quick_sql --sql "CREATE DATABASE IF NOT EXISTS poul_raw" --exec
export SAGO_CONNECTION_STRING=$(echo $SAGO_CONNECTION_STRING | sed 's/database=[^;]*;/database=poul_raw;/')
dbsync -i poul_raw.json
sago_quick_sql --sql "CREATE DATABASE IF NOT EXISTS oa_archive" --exec
export SAGO_CONNECTION_STRING=$(echo $SAGO_CONNECTION_STRING | sed 's/database=[^;]*;/database=oa_archive;/')
dbsync -i community_archiver.json
