import Module
import os
import json
import SubprocessWrapper
import concurrent.futures
import time
import DependenciesBuilder
import subprocess
import multiprocessing
from shutil import copyfile

start_time = time.time()

cwd = os.getcwd()
config = json.load(open('bin/config.json'))
state = json.load(open('build/state.json'))
working_directory = 'build/Windows'
lib_updated = False
compile_options = config['compiler']
threads_count = multiprocessing.cpu_count() - 1

compile_options_str = ' '
libs = ' '
defs = ' '

for option in compile_options['flags']:
    compile_options_str += option + ' '

for option in compile_options['dll']:
    compile_options_str += option + ' '

for option in compile_options['lib_paths']:
    option = option.replace('$CWD$', cwd)
    libs += '/LIBPATH:"' + option + '" '

for option in config['modules']:
    if option['type'] == 'lib':
        libs += option['name'] + '.lib '

for option in compile_options['definitions']:
    defs += '/D' + option + ' '

# Fisrt we need to check if process library is present, if not we need to compile it
if not os.path.exists('build/Subprocess.dll'):
    if not os.path.exists('build/Windows/Subprocess'):
        os.mkdir('build/Windows/Subprocess')

    os.system(
        'vcvarsall x86_amd64 & cd build/Windows/Subprocess & cl /Fe"' + cwd + '/bin/Subprocess.dll"' +
        compile_options_str +
        '/I"' + cwd + '/include" ' +
        cwd + '/src/Subprocess/Subprocess.cpp'
    )

    copyfile('bin/Subprocess.dll', 'build/Subprocess.dll')

process = SubprocessWrapper.SubprocessWrapper()

process.Setup()
process.WriteInput('cd ' + working_directory)
process.Wait()

processes = []

for i in range(0, threads_count):
    process_ = SubprocessWrapper.SubprocessWrapper()

    process_.Setup()
    process_.WriteInput('cd ' + working_directory)
    process_.Wait()

    processes.append(process_)

# subprocess.call(['bin/ReflectionGenerator.exe', '-cwd ' + cwd])

for file in os.listdir('external'):
    if file.endswith('.json'):
        j_config = json.load(open(cwd + '/external/' + file))

        if 'type' in j_config and j_config['type'] == 'external_lib_config':
            DependenciesBuilder.compile_dep(process, j_config, j_config['name'])
    else:
        continue

modules = []

for key in config['modules']:
    modules.append(Module.Module(key['name'], key['type'], cwd, config['compiler'], working_directory, config['modules'], state['filesHash']))

for subdir, dirs, files in os.walk('src'):
    splitted = subdir.split(os.sep)

    if len(splitted) > 1:
        for module in modules:
            if module.name == splitted[1]:
                for file in files:
                    module.source_files.append(subdir + '\\' + file)

                module.directory = subdir

                break

for subdir, dirs, files in os.walk('include'):
    splitted = subdir.split(os.sep)

    if len(splitted) > 1:
        for module in modules:
            if module.name == splitted[1]:
                for file in files:
                    module.header_files.append(subdir + '\\' + file)

                break

def threaded_compile(module, process):
    global lib_updated

    module.generateReflection()
    lib_updated = module.compileWhole(lib_updated, process)

    processes.append(process)

libModules = []

for i in range(0, len(modules)):
    module = modules[i]

    if module.type == 'lib':
        libModules.append(module)

for i in range(0, len(libModules)):
    modules.remove(libModules[i])

with concurrent.futures.ThreadPoolExecutor(max_workers=threads_count) as executor:
    for i in range(0, len(libModules)):
        lib = libModules[i]

        while not processes:
            time.sleep(0.1)

        executor.submit(threaded_compile, lib, processes.pop())

staticLibReflectionModule = Module.Module('StaticLibraryReflection', 'dll', cwd, config['compiler'], working_directory, config['modules'], state['filesHash'])

while libModules:
    lib = libModules.pop(0)

    for j in range(0, len(lib.generated_reflection_objs)):
        staticLibReflectionModule.object_files.append(lib.generated_reflection_objs[j])

staticLibReflectionModule.forceCompile = True
staticLibReflectionModule.compileWhole(lib_updated, process)

with concurrent.futures.ThreadPoolExecutor(max_workers=threads_count) as executor:
    for i in range(0, len(modules)):
        module = modules[i]

        while not processes:
            time.sleep(0.1)

        executor.submit(threaded_compile, module, processes.pop())

while processes:
    process_ = processes.pop()

    process_.Release()
    process_.Delete()



# generatedStaticLibReflection = False

# for i in range(0, len(modules)):
#     if not generatedStaticLibReflection and modules[i].type != 'lib':
#         generatedStaticLibReflection = True
#         staticLibReflectionModule = Module.Module('StaticLibraryReflection', 'dll', cwd, config['compiler'], working_directory, config['modules'], state['filesHash'])

#         for j in range(0, i):
#             module = modules[j]
#             for k in range(0, len(module.generated_reflection_objs)):
#                 staticLibReflectionModule.object_files.append(module.generated_reflection_objs[k])

#         staticLibReflectionModule.forceCompile = True
#         staticLibReflectionModule.compileWhole(lib_updated, process)

#     modules[i].generateReflection()
#     lib_updated = modules[i].compileWhole(lib_updated, process)

print('Compile time:', time.time() - start_time)

with open('build/state.json', 'w') as f:
    json.dump(state, f, indent=4)