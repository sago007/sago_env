#! /usr/bin/python3
import sys
import argparse
import urllib.request
import json
import csv
from datetime import datetime, timezone

VERSION_NUMBER='0.0.1'

def get_releases(owner, repo):
	url = 'https://api.github.com/repos/{}/{}/releases'.format(owner, repo)
	webUrl  = urllib.request.urlopen(url)
	return_code = webUrl.getcode()
	if return_code < 200 or return_code >= 300:
		print("Failed to fetch: {}. Return code: {}".format(url, return_code))
		sys.exit(1)
	data = webUrl.read()
	return data
	

def main():
	parser = argparse.ArgumentParser(description='Basic program', formatter_class=argparse.RawTextHelpFormatter)
	parser.add_argument('--version', action='store_true', help='Print version number and quit')
	args = parser.parse_args()
	if args.version:
		print(VERSION_NUMBER)
		sys.exit(0)
	data_raw = get_releases('blockattack', 'blockattack-game')  # curl  https://api.github.com/repos/blockattack/blockattack-game/releases
	json_data = json.loads(data_raw)
	csv_writer = csv.writer(sys.stdout)
	now = datetime.now(timezone.utc).isoformat()
	for element in json_data:
		#print (element['name'],',', element['tag_name'])
		release_name = element['name']
		tag_name = element['tag_name']
		for asset in element['assets']:
			#print(asset['name'], asset['download_count'])
			asset_name = asset['name']
			download_count = asset['download_count']
			csv_writer.writerow([release_name, tag_name, asset_name, download_count, now])

if __name__ == "__main__":
	main()
