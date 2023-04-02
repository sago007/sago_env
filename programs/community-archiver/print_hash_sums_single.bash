
FILENAME=$1
rhash --printf="FILESTART\n%f\nsha256 %{sha-256}\nmd5 %{md5}\ned2k-link %L\nsha1 %{sha1}\n" "$1"