#! /usr/bin/python3
import sys
import argparse
import rhash
import os

VERSION_NUMBER='0.0.1'


def hash_from_file(filename):
	h = rhash.RHash(rhash.MD5 | rhash.SHA1 | rhash.SHA256 | rhash.SHA3_512 | rhash.ED2K | rhash.AICH)
	filesize = 0
	with open(filename, "rb") as file:
		buf = file.read(8192)
		while len(buf) > 0:
			filesize += len(buf)
			h.update(buf)
			buf = file.read(8192)
	h.finish()
	ret = {"path":filename, "md5":h.hex(rhash.MD5), "sha256" : h.hex(rhash.SHA256), "sha3_512" : h.hex(rhash.SHA3_512), "ed2k" : h.hex(rhash.ED2K), "aich" : h.hex(rhash.AICH), "size": filesize}
	return ret




def process(dir, reference_file):
	try:
		ref_time = os.path.getmtime(reference_file)
	except FileNotFoundError:
		ref_time = 0
	ret = []
	for dirpath, dirnames, filenames in os.walk(dir):
		for filename in filenames:
			filepath = dirpath+"/"+filename
			print("working on", filepath)
			if os.path.getmtime(filepath) >= ref_time:
				h = hash_from_file(filepath)
				h["filename"] = filename
				ret.append(h)
			else:
				print("Skipping because {} has timestamp in the past")
	return ret
			


def main():
	parser = argparse.ArgumentParser(description='Basic program', formatter_class=argparse.RawTextHelpFormatter)
	parser.add_argument('--version', action='store_true', help='Print version number and quit')
	#parser.add_argument('filename', action='store',help="Filename to look at")
	parser.add_argument('scandir', action='store',help="Dir to scan for new files")
	parser.add_argument('destfile', action='store',help="Destination file to append the results to")
	args = parser.parse_args()
	if args.version:
		print(VERSION_NUMBER)
		sys.exit(0)
	#h = hash_from_file(args.filename)
	h = process(args.scandir, args.destfile)
	print(h)

if __name__ == "__main__":
	main()
