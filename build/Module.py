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
    def __init__(self, name, type, process, cwd, compile_config, working_directory, modules, hashes):
        self.name = name
        self.type = type
        self.source_files = []
        self.header_files = []
        self.object_files = []
        self.reflection_src_files = []
        self.process = process
        self.cwd = cwd
        self.compile_config = compile_config    
        self.working_directory = working_directory
        self.modules = modules
        self.hashes = hashes
        self.directory = ''

    def generateReflection(self):
        for h in self.header_files:
            try:
                subprocess.call(['bin/ReflectionGenerator.exe', '-b bin', '-i ' + h, '-cwd ' + self.cwd])
            except:
                print(os.sys.exc_info())

            # for src in self.source_files:
            #     if os.path.splitext(os.path.basename(h))[0] == os.path.splitext(os.path.basename(src))[0]:
            splitted = h.split(os.sep)
            res = ''

            for i in range(1, len(splitted) - 1):
                res += splitted[i] + '\\'

            res += splitted[len(splitted) - 1].split('.')[0] + '.Generated.cpp'

            if os.path.isfile(self.cwd + '/build/GeneratedReflection/' + res):
                self.reflection_src_files.append(res)

    def compileWhole(self):
        flags = ''
        defs = ''
        whole_compile = False
        includes = ''
        external_libs = ''

        for key in self.compile_config['flags']:
            flags += '/' + key + ' '

        for key in self.compile_config['definitions']:
            defs += '/D' + key + ' '

        for key in self.compile_config['includes']:
            includes += '/I' + self.cwd + '/' + key + ' '

        for key in self.compile_config['lib_paths']:
            external_libs += '/LIBPATH:' + self.cwd + '/' + key + ' '

        for key in self.compile_config['libs2']:
            external_libs += key + ' '

        for src in self.reflection_src_files:
            splitted = src.split(os.sep)
            dir_ = self.cwd + '/' + self.working_directory
            obj_dir = ''

            for s in range(0, len(splitted) - 1):
                dir_ += '/' + splitted[s]
                obj_dir += splitted[s] + '/'

                if os.path.isdir(dir_):
                    self.process.WriteInput('cd ' + dir_)
                    self.process.Wait()
                else:
                    os.mkdir(dir_)
                    self.process.WriteInput('cd ' + dir_)
                    self.process.Wait()

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
                self.process.WriteInput('cl ' + flags + ' ' + defs + ' ' + includes + ' /c ' + self.cwd + '/build/GeneratedReflection/' + src)
                self.process.Wait()
                self.process.WriteInput('cd ' + self.cwd + '/' + self.working_directory)
                self.process.Wait()

            self.object_files.append(self.working_directory + '/' + obj_dir + splitted[len(splitted) - 1].split('.')[0] + '.Generated.obj')

        for src in self.source_files:
            splitted = src.split(os.sep)
            dir_ = self.cwd + '/' + self.working_directory
            obj_dir = ''

            for s in range(1, len(splitted) - 1):
                dir_ += '/' + splitted[s]
                obj_dir += splitted[s] + '/'

                if os.path.isdir(dir_):
                    self.process.WriteInput('cd ' + dir_)
                    self.process.Wait()
                else:
                    os.mkdir(dir_)
                    self.process.WriteInput('cd ' + dir_)
                    self.process.Wait()

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
                self.process.WriteInput('cl ' + flags + ' ' + defs + ' ' + includes + ' /c ' + self.cwd + '/' + src)
                self.process.Wait()
                self.process.WriteInput('cd ' + self.cwd + '/' + self.working_directory)
                self.process.Wait()

            self.object_files.append(self.working_directory + '/' + obj_dir + splitted[len(splitted) - 1].split('.')[0] + '.obj')

        if whole_compile:
            dll_type = ''

            for key in self.compile_config['dll']:
                dll_type += '/' + key + ' '

            libs = ''

            for key in self.modules:
                if key['type'] == 'lib':
                    libs += self.cwd + '/bin/' + key['name'] + '.lib '

            obj_files = ''

            for o in self.object_files:
                obj_files += self.cwd + '/' + o + ' '

            self.process.WriteInput('cd ' + self.cwd + '/' + self.working_directory + '/' + self.name)
            self.process.Wait()

            if self.type == 'lib':
                self.process.WriteInput('lib ' + obj_files + '/OUT:' + self.cwd + '/bin/' + self.name + '.lib')
                self.process.Wait()
            elif self.type == 'dll':
                self.process.WriteInput('cl ' +
                    flags + ' ' +
                    defs + ' ' +
                    includes + ' ' +
                    dll_type + '/Fe' + self.cwd + '/bin/' + self.name + '.dll ' +
                    obj_files + '/link ' + libs + ' ' + external_libs
                )
                self.process.Wait()
            elif self.type == 'exe':
                self.process.WriteInput('cl ' +
                    flags + ' ' +
                    defs + ' ' +
                    includes + ' ' +
                    '/Fe' + self.cwd + '/bin/' + self.name + '.exe ' +
                    obj_files + ' /link ' + libs + ' ' + external_libs
                )
                self.process.Wait()