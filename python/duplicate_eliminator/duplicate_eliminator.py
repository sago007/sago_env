#! /usr/bin/python3
import sys
import argparse
import os
import hashlib
from pathlib import Path

VERSION_NUMBER='0.0.1'

def calculate_sha256(filepath):
	"""Calculate SHA256 hash of a file."""
	sha256_hash = hashlib.sha256()
	with open(filepath, "rb") as f:
		# Read file in chunks to handle large files efficiently
		for byte_block in iter(lambda: f.read(4096), b""):
			sha256_hash.update(byte_block)
	return sha256_hash.hexdigest()

def process_folder(folder_path):
	"""Process all files in folder, delete duplicates based on SHA256 hash."""
	if not os.path.isdir(folder_path):
		print(f"Error: '{folder_path}' is not a valid directory")
		sys.exit(1)
	
	# Dictionary to store hash -> first file path mapping
	seen_hashes = {}
	deleted_count = 0
	processed_count = 0
	
	# Get all files recursively and sort them for predictable order
	all_files = []
	for root, dirs, files in os.walk(folder_path):
		for filename in files:
			filepath = os.path.join(root, filename)
			all_files.append(filepath)
	
	# Sort files to ensure predictable processing order
	all_files.sort()
	
	print(f"Processing {len(all_files)} files in '{folder_path}'...")
	
	for filepath in all_files:
		try:
			processed_count += 1
			file_hash = calculate_sha256(filepath)
			
			if file_hash in seen_hashes:
				# Duplicate found - delete this file
				original_file = seen_hashes[file_hash]
				print(f"Duplicate found: '{filepath}' (same as '{original_file}')")
				os.remove(filepath)
				deleted_count += 1
				print(f"  Deleted: '{filepath}'")
			else:
				# First occurrence of this hash
				seen_hashes[file_hash] = filepath
		except Exception as e:
			print(f"Error processing '{filepath}': {e}")
	
	print(f"\nSummary:")
	print(f"  Files processed: {processed_count}")
	print(f"  Duplicates deleted: {deleted_count}")
	print(f"  Unique files remaining: {processed_count - deleted_count}")

def main():
	parser = argparse.ArgumentParser(
		description='Eliminate duplicate files based on SHA256 hash.\n'
		'Files are processed in sorted order, so the first occurrence is kept.',
		formatter_class=argparse.RawTextHelpFormatter
	)
	parser.add_argument('folder', nargs='?', help='Folder to process for duplicate files')
	parser.add_argument('--version', action='store_true', help='Print version number and quit')
	args = parser.parse_args()
	
	if args.version:
		print(VERSION_NUMBER)
		sys.exit(0)
	
	if not args.folder:
		parser.print_help()
		sys.exit(1)
	
	process_folder(args.folder)

if __name__ == "__main__":
	main()
