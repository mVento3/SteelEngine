import hashlib
import os
import json

def sha512(fname):
    hash = hashlib.sha512()

    with open(fname, 'rb') as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash.update(chunk)

    return hash.hexdigest()

def fullToRelative(projectPath, fullPath):
    projectPath = projectPath.replace('/', '\\')
    fullPath = fullPath.replace('/', '\\')

    splitted = projectPath.split(os.path.sep)
    splitted2 = fullPath.split(os.path.sep)
    newPath = ''

    for i in range(len(splitted), len(splitted2) - 1):
        newPath += splitted2[i] + '\\'

    return newPath + splitted2[len(splitted2) - 1]

def serverSide(projectPath):
    filesData = { 'name':os.path.basename(projectPath), 'files':[] }

    for subdir, dirs, files in os.walk(projectPath):
        if os.path.basename(subdir) != '.vscode':
            for file in files:
                filesData['files'].append({ 'filename': fullToRelative(projectPath, subdir + '/' + file), 'hash': sha512(subdir + '/' + file) })

    return json.dumps(filesData)

def clientSide():
    print('AAAAAA\n')