import os
import PythonProcessWrapper
import json

external_path = 'D:/Projects/C++/SteelEngine/external'
folder_path = 'imgui'
process = PythonProcessWrapper.PythonProcess()
cwd = os.getcwd()
working_directory = 'build/Windows'
j_config = json.load(open(external_path + '/' + folder_path + '/config.json'))
j_config = j_config['compiler']

process.Setup()

for subdir, dirs, files in os.walk(external_path + '/' + folder_path):
    src_found = False
    include_found = False

    for dir in dirs:
        if dir == 'src':
            src_found = True
        elif dir == 'include':
            include_found = True

    if not src_found and not include_found:
        files_to_compile = []

        for file in files:
            file = subdir + '/' + file

            if os.path.splitext(file)[1] == '.cpp':
                files_to_compile.append(file)

        process.WriteInput('cd ' + cwd + '/' + working_directory)
        process.Wait()

        check_path = cwd + '/' + working_directory + '/' + folder_path

        if os.path.isdir(check_path):
            process.WriteInput('cd ' + folder_path)
            process.Wait()
        else:
            os.mkdir(check_path)
            process.WriteInput('cd ' + folder_path)
            process.Wait()

        flags = ''
        defs = ''
        includes = ''
        external_libs = ''
        obj_files = ''

        for key in j_config['flags']:
            flags += '/' + key + ' '

        for key in j_config['definitions']:
            defs += '/D' + key + ' '

        for key in j_config['includes']:
            includes += '/I' + cwd + '/' + key + ' '

        for key in j_config['lib_paths']:
            external_libs += '/LIBPATH:' + cwd + '/' + key + ' '

        for key in j_config['libs']:
            external_libs += key + ' '

        for file_to_compile in files_to_compile:
            process.WriteInput('cl ' + flags + ' ' + defs + ' ' + includes + ' /c ' + file_to_compile)
            process.Wait()

            file_to_compile = cwd + '/' + working_directory + '/' + folder_path + '/' + os.path.splitext(os.path.basename(file_to_compile))[0] + '.obj '

            if os.path.isfile(file_to_compile):
                obj_files += file_to_compile

        if obj_files != '':
            process.WriteInput('lib ' + obj_files + '/OUT:' + cwd + '/bin/' + folder_path + '.lib')
            process.Wait()