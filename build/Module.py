import PythonProcessWrapper
import subprocess
import os
import hashlib

def sha512(fname):
    hash = hashlib.sha512()

    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash.update(chunk)

    return hash.hexdigest()

class Module:
    def __init__(self, name, type, cwd, compile_config, working_directory, modules, hashes):
        self.name = name
        self.type = type
        self.source_files = []
        self.header_files = []
        self.object_files = []
        self.reflection_src_files = []
        self.cwd = cwd
        self.compile_config = compile_config    
        self.working_directory = working_directory
        self.modules = modules
        self.hashes = hashes
        self.directory = ''
        self.generated_reflection_objs = []
        self.forceCompile = False

    def generateReflection(self):
        for h in self.header_files:
            try:
                subprocess.call(['bin/ReflectionGenerator.exe', '-b bin', '-i ' + h, '-cwd ' + self.cwd])
            except:
                print(os.sys.exc_info())

            splitted = h.split(os.sep)
            res = ''

            for i in range(1, len(splitted) - 1):
                res += splitted[i] + '\\'

            res += splitted[len(splitted) - 1].split('.')[0] + '.Generated.cpp'

            if os.path.isfile(self.cwd + '/build/GeneratedReflection/' + res):
                self.reflection_src_files.append(res)

    def compileWhole(self, lib_updated, process):
        flags = ''
        defs = ''
        whole_compile = False
        includes = ''
        libs = ''

        for key in self.compile_config['flags']:
            flags += key + ' '

        for key in self.compile_config['definitions']:
            defs += '/D' + key + ' '

        for key in self.compile_config['includes']:
            key = key.replace('$CWD$', self.cwd)
            includes += '/I"' + key + '" '

        for key in self.compile_config['lib_paths']:
            key = key.replace('$CWD$', self.cwd)
            libs += '/LIBPATH:"' + key + '" '

        for key in self.compile_config['libs']:
            libs += key + ' '

        for key in self.modules:
            if key['type'] == 'lib':
                libs += key['name'] + '.lib '

        for src in self.reflection_src_files:
            splitted = src.split(os.sep)
            dir_ = self.cwd + '/' + self.working_directory
            obj_dir = ''

            for s in range(0, len(splitted) - 1):
                dir_ += '/' + splitted[s]
                obj_dir += splitted[s] + '/'

                if os.path.isdir(dir_):
                    process.WriteInput('cd ' + dir_)
                    process.Wait()
                else:
                    os.mkdir(dir_)
                    process.WriteInput('cd ' + dir_)
                    process.Wait()

            compile = True
            found = False
            res = ''

            for i in range(0, len(splitted) - 1):
                res += splitted[i]

                if i < len(splitted) - 2:
                    res += '/'

            for key in self.hashes:
                if key['folder'] == res:
                    found = True
                    found2 = False
                    for f in key['files']:
                        if f['filename'] == os.path.basename(src):
                            found2 = True

                            if f['hash'] == sha512(self.cwd + '/build/GeneratedReflection/' + src):
                                compile = False
                                break
                            else:
                                f['hash'] = sha512(self.cwd + '/build/GeneratedReflection/' + src)

                    if not found2:
                        key['files'].append({ 'filename': os.path.basename(src), 'hash': sha512(self.cwd + '/build/GeneratedReflection/' + src) })
                    else:
                        break

            if not found:
                self.hashes.append({ 'folder': res, 'files': [{ 'filename': os.path.basename(src), 'hash': sha512(self.cwd + '/build/GeneratedReflection/' + src) }] })

            if compile or not os.path.isfile(dir_ + '/' + splitted[len(splitted) - 1].split('.')[0] + '.Generated.obj'):
                whole_compile = True
                process.WriteInput('cl ' + flags + ' ' + defs + ' ' + includes + ' /c ' + self.cwd + '/build/GeneratedReflection/' + src)
                process.Wait()

                if process.WasError():
                    print("Error while compiling:", process.GetErrorMessage())

                process.WriteInput('cd ' + self.cwd + '/' + self.working_directory)
                process.Wait()

            obj = self.working_directory + '/' + obj_dir + splitted[len(splitted) - 1].split('.')[0] + '.Generated.obj'
            self.object_files.append(obj)
            self.generated_reflection_objs.append(obj)

        for src in self.source_files:
            splitted = src.split(os.sep)
            dir_ = self.cwd + '/' + self.working_directory
            obj_dir = ''

            for s in range(1, len(splitted) - 1):
                dir_ += '/' + splitted[s]
                obj_dir += splitted[s] + '/'

                if os.path.isdir(dir_):
                    process.WriteInput('cd ' + dir_)
                    process.Wait()
                else:
                    os.mkdir(dir_)
                    process.WriteInput('cd ' + dir_)
                    process.Wait()

            compile = True
            found = False
            res = ''

            for i in range(0, len(splitted) - 1):
                res += splitted[i]

                if i < len(splitted) - 2:
                    res += '/'

            for key in self.hashes:
                if key['folder'] == res:
                    found = True
                    found2 = False
                    for f in key['files']:
                        if f['filename'] == os.path.basename(src):
                            found2 = True

                            if f['hash'] == sha512(src):
                                compile = False
                                break
                            else:
                                f['hash'] = sha512(src)

                    if not found2:
                        key['files'].append({ 'filename': os.path.basename(src), 'hash': sha512(src) })
                    else:
                        break

            if not found:
                self.hashes.append({ 'folder': res, 'files': [{ 'filename': os.path.basename(src), 'hash': sha512(src) }] })

            if compile or not os.path.isfile(dir_ + '/' + splitted[len(splitted) - 1].split('.')[0] + '.obj'):
                whole_compile = True
                process.WriteInput('cl ' + flags + ' ' + defs + ' ' + includes + ' /c ' + self.cwd + '/' + src)
                process.Wait()

                if process.WasError():
                    print("Error while compiling:", process.GetErrorMessage())

                process.WriteInput('cd ' + self.cwd + '/' + self.working_directory)
                process.Wait()

            self.object_files.append(self.working_directory + '/' + obj_dir + splitted[len(splitted) - 1].split('.')[0] + '.obj')

        if whole_compile or lib_updated or self.forceCompile:
            obj_files = ''

            for o in self.object_files:
                obj_files += self.cwd + '/' + o + ' '

            process.WriteInput('cd ' + self.cwd + '/' + self.working_directory + '/' + self.name)
            process.Wait()

            if self.type == 'lib':
                lib_updated = True
                process.WriteInput('lib ' + obj_files + '/OUT:' + self.cwd + '/bin/' + self.name + '.lib')
                process.Wait()

                if process.WasError():
                    print("Error while compiling:", process.GetErrorMessage())

            elif self.type == 'dll':
                for key in self.compile_config['dll']:
                    flags += key + ' '

                process.WriteInput('cl ' +
                    flags + ' ' +
                    defs + ' ' +
                    includes + ' ' +
                    ' /Fe' + self.cwd + '/bin/' + self.name + '.dll ' +
                    obj_files + '/link ' + ' ' + libs
                )
                process.Wait()

                if process.WasError():
                    print("Error while compiling:", process.GetErrorMessage())
            elif self.type == 'exe':
                process.WriteInput('cl ' +
                    flags + ' ' +
                    defs + ' ' +
                    includes + ' ' +
                    '/Fe' + self.cwd + '/bin/' + self.name + '.exe ' +
                    obj_files + ' /link ' + ' ' + libs
                )
                process.Wait()

                if process.WasError():
                    print("Error while compiling:", process.GetErrorMessage())

        return lib_updated