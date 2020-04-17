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

    if found_group:
        for file in src_files['src_files']:
            if file.find('$RCV$') >= 0:
                file = file.replace('$RCV$', '')

                for subdir, dirs, files in os.walk(cwd + '/external/' + external_folder + '/' + file):
                    for file_ in files:
                        if file_.endswith('.cpp') or file_.endswith('.c'):
                            files_to_compile.append(subdir + '/' + file_)
            else:
                files_to_compile.append(cwd + '/external/' + external_folder + '/' + file)
    else:
        print('Please specify files to compile!')

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
        process.WriteInput('lib ' + obj_files + '/OUT:' + cwd + '/bin/' + external_folder + '.lib')
        process.Wait()

        if process.WasError():
            print("Error while compiling:", process.GetErrorMessage())