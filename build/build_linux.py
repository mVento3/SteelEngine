import subprocess
import os
from pathlib import Path

working_directory = "build/Linux"

subprocess.call(['g++', '-std=c++17', '-g', '-Iinclude', ['-o', 'bin/ReflectionGenerator.exe'], working_directory + os.sep + 'main2.o', '-Lbin', '-lstdc++fs', '-lModule'])

project_types = [
    ['FileWatcher', 'lib'],
    ['Module', 'lib'],
    ['Utils', 'lib'],
    ['Core', 'dll'],
    ['RuntimeCompiler', 'dll'],
    ['RuntimeReflection', 'dll'],
    ['ModuleManager', 'dll'],
    ['RuntimeDatabase', 'dll'],
    ['Networking', 'dll']
]

for type in project_types:
    type.append([])

source_files = []
header_files = []
processed_libs = []

for subdir, dirs, files in os.walk('src'):
    for file in files:
        filepath = '..' + os.sep + '..' + os.sep + subdir + os.sep + file

        if filepath.endswith(".cpp"):
            source_files.append(filepath)

for subdir, dirs, files in os.walk('include'):
    for file in files:
        filepath = '..' + os.sep + subdir + os.sep + file

        if filepath.endswith(".h"):
            header_files.append(filepath)

for type in project_types:
    arr = []
    cppFile = ''
    hFile = ''

    for file in source_files:
        project_name = file.split(os.sep)[2]

        if type[0] == project_name:
            arr.append(file.replace('..' + os.sep, ''))
            type[2].append(file)
    
    for file in header_files:
        project_name = file.split(os.sep)[2]

        for srcFile in source_files:
            if type[0] == project_name and os.path.splitext(os.path.basename(file))[0] == os.path.splitext(os.path.basename(srcFile))[0]:
                hFile = file.replace('..' + os.sep, '../SteelEngine/')
                cppFile = srcFile.replace('..' + os.sep, '../SteelEngine/')

                if cppFile != '' and hFile != '':
                    try:
                        subprocess.call(['bin/ReflectionGenerator.exe', cppFile, hFile])
                    except:
                        print(os.sys.exc_info())
    
    type.append(arr)

print('Building object files...')
subprocess.call(['clang++', '-std=c++17', '-c', '-g', '-I../../include', ['-working-directory', working_directory]] + source_files)

for type in project_types:
    objects = []
    generatedCppFiles = []
    index = 0
    for new_a in type[2]:
        splitted = new_a.split(os.sep)
        objects.append(working_directory + os.sep + os.path.splitext(splitted[len(splitted) - 1])[0] + '.o')
        generatedCpp = splitted[len(splitted) - 1]
        generatedCppFinal = '__generated_reflection__/' + generatedCpp.split('.')[0] + '.Generated.cpp'

        if os.path.isfile(generatedCppFinal):
            if index < len(type[2]) - 1:
                generatedCppFinal = generatedCppFinal + ' '

        generatedCppFiles.append(generatedCppFinal)
        index += 1

    if type[1] == 'dll':
        print('Building', type[0] + '.dll')

        subprocess.call(['clang++', '-std=c++17', '-shared', '-g', '-Iinclude', ['-o', 'bin/' + type[0] + '.so']] + [generatedCppFiles] + objects + ['-Lbin', '-lstdc++fs'] + processed_libs + ['-LC:/msys64/mingw64/x86_64-w64-mingw32/lib', '-lws2_32'])
    elif type[1] == 'lib':
        processed_libs.append('-l' + type[0])
        print('Building', type[0] + '.lib')
        subprocess.call(['ar', '-rcs', 'bin/' + 'lib' + type[0] + '.a'] + objects)

print('Building Engine.exe')
subprocess.call(['g++', '-std=c++17', '-g', '-Iinclude', ['-o', 'bin/Engine.exe'], working_directory + os.sep + 'main.o', '-Lbin'] + processed_libs + ['-LC:/msys64/mingw64/x86_64-w64-mingw32/lib', '-lws2_32'])