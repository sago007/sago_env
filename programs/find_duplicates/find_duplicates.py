import os
import hashlib
from collections import defaultdict
import sys



def process_dir(directory):
    files_by_hash = defaultdict(list)
    for x in os.walk(directory):
        for y in [x[2]]:
            for z in y:
                openedFile = open(directory+"/"+z,"br")
                readFile = openedFile.read()
                md5Hash = hashlib.md5(readFile)
                md5Hashed = md5Hash.hexdigest()
                openedFile.close()
                files_by_hash[md5Hashed].append(z)
    for x in files_by_hash:
        list_of_files = files_by_hash[x]
        for y in list_of_files[1:]:
            print("rm {}/{} # Duplicate of {}".format(directory, y, list_of_files[0]))

if len(sys.argv) < 2:
    print("No arguments given. Arguments myst be a list of files.")
    sys.exit(1)
for dir in sys.argv[1:]:
    print("# Checking dir: {}".format(dir))
    process_dir(dir)
    print("# Done dir: {}".format(dir))
