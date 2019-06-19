import subprocess
import os
from pathlib import Path

# working directory: SteelEngine, main dir, like: build/ bin/ etc...

working_directory = 'build/Windows'

# general
flags = '/ZI /Od /DEBUG:FASTLINK /std:c++17'
definitions = '/DSE_WINDOWS /D_WINSOCKAPI_'
includes = '/I../../include /I../../bin/Runtime/GeneratedReflection /I../../external/Vulkan/Include /I../../external/SDL2-2.0.9/include'
libs = 'Ws2_32.lib ../../external/Vulkan/Lib/vulkan-1.lib ../../external/SDL2-2.0.9/lib/x64/SDL2.lib'

# dll's
dll_type = '/MT /LD'

subprocess.call(['bin/WindowsCompiler', working_directory, 'cl', '/ZI /Od /DEBUG:FASTLINK /DSE_WINDOWS', '/Fe../../bin/ReflectionGenerator.exe', 'main2.obj', '/link', '/LIBPATH:../../bin', 'Module.lib'])

project_types = [
    ['FileWatcher', 'lib'],
    ['Module', 'lib'],
    ['Utils', 'lib'],
    ['Rendering', 'dll'],
    ['Window', 'dll'],
    ['ModuleManager', 'dll'],
    ['RuntimeReflection', 'dll'],
    ['Core', 'dll'],
    ['RuntimeCompiler', 'dll'],
    ['RuntimeDatabase', 'dll'],
    ['Networking', 'dll'],
    ['Logger', 'dll'],
    ['FileSystem', 'dll']
]

for type in project_types:
    type.append([])

source_files = []
header_files = []
processed_libs = []

for subdir, dirs, files in os.walk('src'):
    for file in files:
        filepath = subdir + os.sep + file

        if filepath.endswith(".cpp"):
            source_files.append(filepath)

for subdir, dirs, files in os.walk('include'):
    for file in files:
        filepath = subdir + os.sep + file

        if filepath.endswith(".h"):
            header_files.append(filepath)

for type in project_types:
    arr = []
    cppFile = ''
    hFile = ''

    for file in source_files:
        project_name = file.split(os.sep)[1]

        if type[0] == project_name:
            arr.append(file.replace('..' + os.sep, ''))
            type[2].append(file)

    for file in header_files:
        project_name = file.split(os.sep)[1]

        for srcFile in source_files:
            hDirs = file.split(os.sep)
            cppDirs = srcFile.split(os.sep)
            found = False

            if len(hDirs) == len(cppDirs):
                for x in range(1, len(hDirs) - 1):
                    if hDirs[x] == cppDirs[x]:
                        found = True

            if found and type[0] == project_name and os.path.splitext(os.path.basename(file))[0] == os.path.splitext(os.path.basename(srcFile))[0]:
                if srcFile != '' and file != '':
                    # print("AAAAA")
                    try:
                        subprocess.call(['bin/ReflectionGenerator.exe', 'bin', srcFile, file])
                    except:
                        print(os.sys.exc_info())

    type.append(arr)

ff = []

for f in source_files:
    ff.append('../../' + f)

print('Building object files...')
subprocess.call(['bin/WindowsCompiler', working_directory, 'cl', [flags], [definitions], [includes], '/c'] + ff)

for type in project_types:
    objects = []
    generatedCppFiles = []
    index = 0
    for new_a in type[2]:
        splitted = new_a.split(os.sep)
        objects.append(os.path.splitext(splitted[len(splitted) - 1])[0] + '.obj')
        generatedCpp = splitted[len(splitted) - 1]
        generatedCppFinal = '../../bin/Runtime/GeneratedReflection/' + generatedCpp.split('.')[0] + '.Generated.cpp'

        # if os.path.isfile(generatedCppFinal):
        if index < len(type[2]) - 1:
            generatedCppFinal = generatedCppFinal + ' '

        generatedCppFiles.append(generatedCppFinal)
        index += 1

    if type[1] == 'dll':
        print('Building', type[0] + '.dll')

        subprocess.call(['bin/WindowsCompiler', working_directory, 'cl', [flags], [definitions], [includes], [dll_type], ['/Fe', '../../bin/' + type[0] + '.dll']] + [generatedCppFiles] + objects + ['/link', [libs]] + processed_libs)
    elif type[1] == 'lib':
        processed_libs.append('../../bin/' + type[0] + '.lib')
        print('Building', type[0] + '.lib')
        subprocess.call(['bin/WindowsCompiler', working_directory, 'lib'] + objects + [['/OUT:', '../../bin/' + type[0] + '.lib']])

print('Building Engine.exe')
subprocess.call(['bin/WindowsCompiler', working_directory, 'cl', [flags], [definitions], [includes], ['/Fe', '../../bin/Engine.exe'], 'main.obj', '/link', [libs]] + processed_libs)