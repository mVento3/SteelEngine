import Module
import os
import json
import PythonProcessWrapper

cwd = os.getcwd()
config = json.load(open('bin/config.json'))
state = json.load(open('build/state.json'))
process = PythonProcessWrapper.PythonProcess()
working_directory = 'build/Windows'
lib_updated = False

process.Setup()
process.WriteInput('cd ' + working_directory)
process.Wait()

modules = []

for key in config['modules']:
    modules.append(Module.Module(key['name'], key['type'], process, cwd, config['compiler'], working_directory, config['modules'], state['filesHash']))

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

generatedStaticLibReflection = False

for i in range(0, len(modules)):
    if not generatedStaticLibReflection and modules[i].type != 'lib':
        generatedStaticLibReflection = True
        staticLibReflectionModule = Module.Module('StaticLibraryReflection', 'dll', process, cwd, config['compiler'], working_directory, config['modules'], state['filesHash'])

        for j in range(0, i):
            module = modules[j]
            for k in range(0, len(module.generated_reflection_objs)):
                staticLibReflectionModule.object_files.append(module.generated_reflection_objs[k])

        staticLibReflectionModule.forceCompile = True
        staticLibReflectionModule.compileWhole(lib_updated)

    modules[i].generateReflection()
    lib_updated = modules[i].compileWhole(lib_updated)

with open('build/state.json', 'w') as f:
    json.dump(state, f, indent=4)