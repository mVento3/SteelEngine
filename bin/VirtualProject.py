import hashlib
import json
import os

def sha512(fname):
    hash = hashlib.sha512()

    with open(fname, 'rb') as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash.update(chunk)

    return hash.hexdigest()

def getCWD():
    return os.getcwd()