import Module
import os
import json
import PythonProcessWrapper
import concurrent.futures
import time

cwd = os.getcwd()
config = json.load(open('bin/config.json'))
state = json.load(open('build/state.json'))
working_directory = 'build/Windows'
lib_updated = False

process = PythonProcessWrapper.PythonProcessWrapper()

process.Setup()
process.WriteInput('cd ' + working_directory)
process.Wait()

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

# def threaded_compile(module, process):
#     global lib_updated

#     module.generateReflection()
#     lib_updated = module.compileWhole(lib_updated, process)

#     processes.append(process)

# libs = []

# for i in range(0, len(modules)):
#     module = modules[i]

#     if module.type == 'lib':
#         libs.append(module)

# for i in range(0, len(libs)):
#     modules.remove(libs[i])

# with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
#     for i in range(0, len(libs)):
#         lib = libs[i]

#         while not processes:
#             time.sleep(0.1)

#         executor.submit(threaded_compile, lib, processes.pop())

# staticLibReflectionModule = Module.Module('StaticLibraryReflection', 'dll', cwd, config['compiler'], working_directory, config['modules'], state['filesHash'])

# while libs:
#     lib = libs.pop(0)

#     for j in range(0, len(lib.generated_reflection_objs)):
#         staticLibReflectionModule.object_files.append(lib.generated_reflection_objs[j])

# staticLibReflectionModule.forceCompile = True
# staticLibReflectionModule.compileWhole(lib_updated, process)

# with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
#     for i in range(0, len(modules)):
#         module = modules[i]

#         while not processes:
#             time.sleep(0.1)

#         executor.submit(threaded_compile, module, processes.pop())

generatedStaticLibReflection = False

for i in range(0, len(modules)):
    if not generatedStaticLibReflection and modules[i].type != 'lib':
        generatedStaticLibReflection = True
        staticLibReflectionModule = Module.Module('StaticLibraryReflection', 'dll', cwd, config['compiler'], working_directory, config['modules'], state['filesHash'])

        for j in range(0, i):
            module = modules[j]
            for k in range(0, len(module.generated_reflection_objs)):
                staticLibReflectionModule.object_files.append(module.generated_reflection_objs[k])

        staticLibReflectionModule.forceCompile = True
        staticLibReflectionModule.compileWhole(lib_updated, process)

    modules[i].generateReflection()
    lib_updated = modules[i].compileWhole(lib_updated, process)

with open('build/state.json', 'w') as f:
    json.dump(state, f, indent=4)