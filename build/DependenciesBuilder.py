import os
import SubprocessWrapper
import json

cwd = os.getcwd()
working_directory = 'build/Windows'

def compile_dep(process, config, external_folder):
    j_config = config['compiler']

    if 'groups' not in j_config:
        print('Please specify groups!')

        return

    src_files = j_config['groups']
    found_group = False

    for group in src_files:
        if group['name'] == 'MAIN':
            src_files = group
            found_group = True

            break

    files_to_compile = []
    files_to_exclude = []

    if found_group:
        for file in src_files['src_files']:
            if file.find('$RCV$') >= 0:
                file = file.replace('$RCV$', '')

                for subdir, dirs, files in os.walk(cwd + '/external/' + external_folder + '/' + file):
                    for file_ in files:
                        if file_.endswith('.cpp') or file_.endswith('.c'):
                            files_to_compile.append(subdir + '/' + file_)
            elif file.find('$NOT$') >= 0:
                file = file.replace('$NOT$', '')

                files_to_exclude.append(file)
            else:
                files_to_compile.append(cwd + '/external/' + external_folder + '/' + file)
    else:
        print('Please specify files to compile!')

    for i in range(len(files_to_compile)):
        files_to_compile[i] = os.path.normpath(files_to_compile[i])

    for i in range(len(files_to_exclude)):
        files_to_exclude[i] = os.path.normpath(files_to_exclude[i])

    files_to_compile_new = []

    for file in files_to_compile:
        found = False

        for exclude in files_to_exclude:
            if file.find(exclude) >= 0:
                found = True

                break

        if not found:
            files_to_compile_new.append(file)

    files_to_compile.clear()

    files_to_compile = files_to_compile_new

    for file in files_to_compile:
        print(file)

    process.WriteInput('cd ' + cwd + '/' + working_directory)
    process.Wait()

    check_path = cwd + '/' + working_directory + '/' + external_folder

    if os.path.isdir(check_path):
        process.WriteInput('cd ' + external_folder)
        process.Wait()
    else:
        os.mkdir(check_path)
        process.WriteInput('cd ' + external_folder)
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
        key = key.replace('$CWD$', cwd)
        includes += '/I' + key + ' '

    for key in j_config['lib_paths']:
        key = key.replace('$CWD$', cwd)
        external_libs += '/LIBPATH:' + key + ' '

    for key in j_config['libs']:
        external_libs += key + ' '

    for file_to_compile in files_to_compile:
        process.WriteInput('cl ' + flags + ' ' + defs + ' ' + includes + ' /c ' + file_to_compile)
        process.Wait()

        if process.WasError():
            print("Error while compiling:", process.GetErrorMessage())

        file_to_compile = cwd + '/' + working_directory + '/' + external_folder + '/' + os.path.splitext(os.path.basename(file_to_compile))[0] + '.obj '

        if os.path.isfile(file_to_compile):
            obj_files += file_to_compile

    if obj_files != '':
        if config['out_type'] == 'lib':
            process.WriteInput('lib ' + obj_files + '/OUT:' + cwd + '/bin/' + external_folder + '.lib')
            process.Wait()
        elif config['out_type'] == 'dll':
            process.WriteInput('cl ' +
                flags + ' ' +
                defs + ' ' +
                includes + ' ' +
                ' /Fe' + cwd + '/bin/' + external_folder + '.dll' + ' ' +
                obj_files + '/link ' + external_libs
            )
            process.Wait()

        if process.WasError():
            print("Error while compiling:", process.GetErrorMessage())