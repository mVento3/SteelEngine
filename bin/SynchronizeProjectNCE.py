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

def compareFiles(project_path, client_data):
    client_json = json.loads(client_data)
    res_json = { 'files':[] }

    # Legend:
    # 0 - UP TO DATE
    # 1 - OVERRIDE
    # 2 - NONE

    for subdir, dirs, files in os.walk(project_path):
        if os.path.basename(subdir) != '.vscode':
            for file in files:
                file_found = False
                file = subdir + '\\' + file
                file = fullToRelative(project_path, file)

                for j_file_info in client_json['files']:
                    if j_file_info['filename'] == file:
                        file_found = True

                        if j_file_info['hash'] != sha512(project_path + '\\' + file):
                            res_json['files'].append({ 'filename':file, 'state':1 })
                        else:
                            res_json['files'].append({ 'filename':file, 'state':0 })

                        break

                if not file_found:
                    res_json['files'].append({ 'filename':file, 'state':2 })

    return json.dumps(res_json)

def getFiles(project_path):
    filesData = { 'name':os.path.basename(project_path), 'files':[] }

    for subdir, dirs, files in os.walk(project_path):
        if os.path.basename(subdir) != '.vscode':
            for file in files:
                filesData['files'].append({ 'filename': fullToRelative(project_path, subdir + '/' + file), 'hash': sha512(subdir + '/' + file) })

    return json.dumps(filesData)