

TMPDIR=$(mktemp -d /tmp/github_download.XXXXXX)
echo "Using $TMPDIR as temporary directory"


export MYSQL_PASSWORD="$MYSQL_PASS"
export MYSQL_DATABASE=poul_raw

export SAGO_CONNECTION_STRING=$(echo $SAGO_CONNECTION_STRING | sed 's/database=[^;]*;/database=poul_raw;/')

python3 ~/git/sago_env/python/github_download_stats_reader/github_download_stats_reader.py > $TMPDIR/github_stats.csv
echo "Downloaded github stats to $TMPDIR/github_stats.csv"
dos2unix $TMPDIR/github_stats.csv

~/git/sago_env/programs/sago_mysql_csv_upsert/sago_mysql_csv_upsert --csv-file "$TMPDIR/github_stats.csv"\
 --table github_downloads --dest-columns "release_name,tag_name,asset_name,download_count,load_time" --csv-columns "release_name,tag_name,asset_name,download_count,load_time" --key-fields "release_name,asset_name,load_time"\
  --parse-iso-dates "load_time"
