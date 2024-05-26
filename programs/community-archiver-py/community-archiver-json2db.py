import json
import mariadb
import os
import argparse
import sys


VERSION_NUMBER='0.0.1'


#            host=os.getenv("MYSQL_HOST","127.0.0.1"),
#            user=os.getenv("MYSQL_USER","root"),
#            password=os.getenv("MYSQL_PASS","password"),
#            database=os.getenv("MYSQL_DATABASE","community_archiver"),

# Function to connect to MySQL database
def connect_to_mysql():
    try:
        connection = mariadb.connect(
            host=os.getenv("MYSQL_HOST","127.0.0.1"),
            user=os.getenv("MYSQL_USER","root"),
            password=os.getenv("MYSQL_PASS","password"),
            database=os.getenv("MYSQL_DATABASE","community_archiver"),
        )
        return connection
    except mariadb.Error as error:
        print("Error while connecting to MySQL", error)
        raise


def is_connected(connection):
    try:
        connection.ping()
    except mariadb.Error:
        return False
    return True

# Function to insert or update records in the database
def insert_or_update_records(data):
    connection = None
    try:
        connection = connect_to_mysql()
        if connection is None:
            print("Error while connecting to MySQL")
            sys.exit(1)

        cursor = connection.cursor()

        for sha256, values in data.items():
            filename = values.get('filename')
            md5 = values.get('md5')
            sha3_512 = values.get('sha3_512')
            ed2klink = values.get('ed2klink')
            size = values.get('size')

            # Check if the record exists
            cursor.execute("SELECT * FROM archive_files WHERE sha256 = %s", (sha256,))
            existing_record = cursor.fetchone()

            if existing_record:
                # Update existing record
                update_query = "UPDATE archive_files SET filename = %s, md5 = %s, sha3_512 = %s, ed2klink = %s, size = %s WHERE sha256 = %s"
                cursor.execute(update_query, (filename, md5, sha3_512, ed2klink, size, sha256))
            else:
                # Insert new record
                insert_query = "INSERT INTO archive_files (sha256, filename, md5, sha3_512, ed2klink, size) VALUES (%s, %s, %s, %s, %s, %s)"
                cursor.execute(insert_query, (sha256, filename, md5, sha3_512, ed2klink, size))

            zipcontent = values.get('zipcontent')
            if zipcontent:
                for file in zipcontent:
                    sub_sha256 = file.get('sha256')
                    filename = file.get('filename')
                    size = file.get('size')

                    # Check if the record exists
                    cursor.execute("SELECT * FROM archive_files_zipcontent WHERE archive_sha256 = %s and sha256 = %s", (sha256,sub_sha256))
                    existing_record = cursor.fetchone()

                    if existing_record:
                        # Update existing record
                        update_query = "UPDATE archive_files_zipcontent SET filename = %s, size = %s WHERE archive_sha256 = %s and sha256 = %s"
                        cursor.execute(update_query, (filename, size, sha256, sub_sha256))
                    else:
                        # Insert new record
                        insert_query = "INSERT INTO archive_files_zipcontent (archive_sha256, sha256, filename, size) VALUES (%s, %s, %s, %s)"
                        cursor.execute(insert_query, (sha256, sub_sha256, filename, size))

        connection.commit()
        print("Records inserted/updated successfully!")

    except mariadb.Error as error:
        print("Error while inserting/updating records:", error)
        raise

    finally:
        if connection and is_connected(connection):
            cursor.close()
            connection.close()
            print("MySQL connection is closed.")


def main():
    parser = argparse.ArgumentParser(description='Basic program', formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('--version', action='store_true', help='Print version number and quit')
    parser.add_argument('filename', action='store',help="Filename to look at")
    args = parser.parse_args()
    if args.version:
        print(VERSION_NUMBER)
        sys.exit(0)
    with open(args.filename) as f:
        data = json.load(f)
        insert_or_update_records(data)
    print("Done!")

if __name__ == "__main__":
    main()