#! /usr/bin/python3
import sys
import argparse
import os
import hashlib
from pathlib import Path

VERSION_NUMBER='0.0.1'

def safe_print(message):
	"""Print a message, handling filenames with invalid UTF-8 encoding."""
	try:
		print(message)
	except UnicodeEncodeError:
		# If normal printing fails, encode with error handling
		print(message.encode(sys.stdout.encoding, errors='replace').decode(sys.stdout.encoding))

def calculate_sha256(filepath):
	"""Calculate SHA256 hash of a file."""
	sha256_hash = hashlib.sha256()
	with open(filepath, "rb") as f:
		# Read file in chunks to handle large files efficiently
		for byte_block in iter(lambda: f.read(4096), b""):
			sha256_hash.update(byte_block)
	return sha256_hash.hexdigest()

def files_identical(filepath1, filepath2):
	"""Compare two files byte-by-byte to verify they are identical."""
	# First check if file sizes are the same
	if os.path.getsize(filepath1) != os.path.getsize(filepath2):
		return False
	
	# Compare files byte-by-byte
	with open(filepath1, "rb") as f1, open(filepath2, "rb") as f2:
		while True:
			chunk1 = f1.read(4096)
			chunk2 = f2.read(4096)
			if chunk1 != chunk2:
				return False
			if not chunk1:  # End of file
				return True

def process_folder(folder_path, dry_run=False):
	"""Process all files in folder, delete duplicates based on SHA256 hash."""
	if not os.path.isdir(folder_path):
		safe_print(f"Error: '{folder_path}' is not a valid directory")
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
	# Custom sort: dot (".") comes before space (" ") and dash ("-")
	def sort_key(path):
		# Replace characters to control sort order: . < - < space
		return path.replace('.', '\x00').replace('-', '\x01').replace(' ', '\x02')

	all_files.sort(key=sort_key)

	if dry_run:
		safe_print(f"DRY RUN: Processing {len(all_files)} files in '{folder_path}'...")
	else:
		safe_print(f"Processing {len(all_files)} files in '{folder_path}'...")

	for filepath in all_files:
		try:
			processed_count += 1
			file_hash = calculate_sha256(filepath)
			
			if file_hash in seen_hashes:
				# Duplicate hash found - verify files are actually identical
				original_file = seen_hashes[file_hash]
				
				# Perform byte-by-byte comparison to ensure files are truly identical
				if not files_identical(filepath, original_file):
					safe_print(f"Warning: Hash collision detected for '{filepath}' and '{original_file}'!")
					safe_print(f"  Files have same hash but different content. Skipping deletion.")
					continue
				
				safe_print(f"Duplicate found: '{filepath}' (same as '{original_file}')")
				if not dry_run:
					os.remove(filepath)
				deleted_count += 1
				if dry_run:
					safe_print(f"  Would delete: '{filepath}'")
				else:
					safe_print(f"  Deleted: '{filepath}'")
			else:
				# First occurrence of this hash
				seen_hashes[file_hash] = filepath
		except Exception as e:
			safe_print(f"Error processing '{filepath}': {e}")
	
	print(f"\nSummary:")
	print(f"  Files processed: {processed_count}")
	if dry_run:
		print(f"  Duplicates that would be deleted: {deleted_count}")
	else:
		print(f"  Duplicates deleted: {deleted_count}")
	print(f"  Unique files remaining: {processed_count - deleted_count}")

def main():
	parser = argparse.ArgumentParser(
		description='Eliminate duplicate files based on SHA256 hash.\n'
		'Files are processed in sorted order, so the first occurrence is kept.',
		formatter_class=argparse.RawTextHelpFormatter
	)
	parser.add_argument('folder', nargs='?', help='Folder to process for duplicate files')
	parser.add_argument('--dry-run', action='store_true', help='Show what would be deleted without actually deleting')
	parser.add_argument('--version', action='store_true', help='Print version number and quit')
	args = parser.parse_args()
	
	if args.version:
		print(VERSION_NUMBER)
		sys.exit(0)
	
	if not args.folder:
		parser.print_help()
		sys.exit(1)
	
	process_folder(args.folder, dry_run=args.dry_run)

if __name__ == "__main__":
	main()
