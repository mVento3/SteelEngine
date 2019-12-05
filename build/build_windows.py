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

for module in modules:
    module.generateReflection()
    lib_updated = module.compileWhole(lib_updated)

with open('build/state.json', 'w') as f:
    json.dump(state, f, indent=4)