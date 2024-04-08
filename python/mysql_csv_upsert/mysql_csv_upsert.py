#! /usr/bin/python3
import sys
import argparse
import os

import csv
import pymysql
import datetime

VERSION_NUMBER='0.0.1'

MYSQL_HOST = os.getenv('MYSQL_HOST','127.0.0.1')
MYSQL_USER = os.getenv('MYSQL_USER','root')
MYSQL_PASSWORD = os.getenv('MYSQL_PASSWORD')
MYSQL_DATABASE = os.getenv('MYSQL_DATABASE','database')

def upsert_csv_to_mysql(csv_file, table, dest_columns, csv_columns, key_fields, parse_iso_dates=None):
	# Connect to MySQL
	connection = pymysql.connect(host=MYSQL_HOST,
									user=MYSQL_USER,
									password=MYSQL_PASSWORD,
									database=MYSQL_DATABASE,
									cursorclass=pymysql.cursors.DictCursor)

	cursor = connection.cursor()

	# Read CSV file
	with open(csv_file, 'r') as file:
		csv_reader = csv.DictReader(file, fieldnames=csv_columns)
		#next(csv_reader)  # Skip header row
		
		for row in csv_reader:
			# Generate SQL query for upsert
			insert_query = f"INSERT INTO {table} ({', '.join(dest_columns)}) VALUES ({', '.join(['%s']*len(dest_columns))})"
			update_query = f"UPDATE {table} SET {', '.join([f'{col}=%s' for col in dest_columns])} WHERE {' AND '.join([f'{col}=%s' for col in key_fields])}"
			csv_columns_name_to_index = {col: i for i, col in enumerate(csv_columns)}
			values = [row[col] for col in csv_columns]
			update_values = [row[col] for col in csv_columns if col in dest_columns]
			update_values.extend([row[col] for col in csv_columns if col in key_fields])
			if not parse_iso_dates:
				parse_iso_dates = []
			for i, col in enumerate(dest_columns):
				if col in parse_iso_dates:
					values[i] = datetime.datetime.fromisoformat(values[i])
				if values[i] == '':
					values[i] = None
			update_values = values.copy()
			for i, col in enumerate(key_fields):
				update_values.append(values[csv_columns_name_to_index[col]])

			try:
				cursor.execute(insert_query, values)
			except pymysql.IntegrityError:
				cursor.execute(update_query, update_values)
				
		connection.commit()
		connection.close()

def main():
	parser = argparse.ArgumentParser(description='Basic program', formatter_class=argparse.RawTextHelpFormatter)
	parser.add_argument('--version', action='store_true', help='Print version number and quit')
	parser.add_argument('--csv-file', required=True, help='CSV file to import')
	parser.add_argument('--table', required=True, help='Table to import to')
	parser.add_argument('--dest-columns', required=True, help='Columns to import to')
	parser.add_argument('--csv-columns', required=True, help='Columns in CSV file')
	parser.add_argument('--key-fields', required=True, help='Key fields for upsert')
	parser.add_argument('--parse-iso-dates', help='Fields to parse from ISO date format')
	args = parser.parse_args()
	if args.version:
		print(VERSION_NUMBER)
		sys.exit(0)
	upsert_csv_to_mysql(args.csv_file, args.table, args.dest_columns.split(','), args.csv_columns.split(','), args.key_fields.split(','), args.parse_iso_dates.split(',') if args.parse_iso_dates else None);

if __name__ == "__main__":
	main()
