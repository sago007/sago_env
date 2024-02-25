#!/usr/bin/env python3

import os
import sys
import argparse
import base64
import requests
from nacl import encoding, public

GITHUB_API_URL = os.getenv("GITHUB_API_URL", "https://api.github.com")  # Only needs to be set if using GitHub Enterprise
GITHUB_TOKEN = os.getenv("GITHUB_TOKEN")

VERSION_NUMBER='1.0.0'

def encrypt(public_key, secret_value):
    """Encrypt a Utf-8 secret"""
    key = public.PublicKey(public_key.encode("utf-8"), encoding.Base64Encoder())
    sealed_box = public.SealedBox(key)
    encrypted = sealed_box.encrypt(secret_value.encode("utf-8"))
    return base64.b64encode(encrypted).decode("utf-8")

def main():
    parser = argparse.ArgumentParser(description="""
This is program to update secrets in Github repos.
Allows quickly retation of sercets.
Uses the environment variable GITHUB_TOKEN for login.""", formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("owner")
    parser.add_argument("repo")
    parser.add_argument("secret_name")
    parser.add_argument("path_to_secret")
    parser.add_argument('--version', action='store_true', help='Print version number and quit')
    args = parser.parse_args()

    if args.version:
        print(VERSION_NUMBER)
        sys.exit(0)

    with open(args.path_to_secret, "rb") as f:
        secret_value = f.read().decode()

    token = GITHUB_TOKEN
    if not token:
        print("No GITHUB_TOKEN found. Please provide a token.")
        sys.exit(1)

    headers = {
        "Authorization": "token " + token,
        "Accept": "application/vnd.github.v3+json"
    }

    public_key_url = f"{GITHUB_API_URL}/repos/{args.owner}/{args.repo}/actions/secrets/public-key"
    response = requests.get(public_key_url, headers=headers)
    response.raise_for_status()
    public_key = response.json()["key"]
    encrypted_value = encrypt(public_key, secret_value)

    update_url = f"{GITHUB_API_URL}/repos/{args.owner}/{args.repo}/actions/secrets/{args.secret_name}"
    response = requests.put(update_url, headers=headers, json={"encrypted_value": encrypted_value, "key_id": response.json()["key_id"]})
    response.raise_for_status()
    print("Secret updated.")

if __name__ == "__main__":
    main()
