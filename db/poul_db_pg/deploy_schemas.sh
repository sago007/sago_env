sago_quick_sql --sql "CREATE SCHEMA IF NOT EXISTS poul_raw" --exec
dbsync --schema poul_raw -i ../poul_db/poul_raw.json
sago_quick_sql --sql "CREATE SCHEMA IF NOT EXISTS oa_archive" --exec
dbsync --schema oa_archive -i ../poul_db/community_archiver.json --validate-name-length 0
