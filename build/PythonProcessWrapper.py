from ctypes import cdll

pythonProcess = cdll.LoadLibrary('bin/PythonProcess.dll')

class PythonProcess(object):
    def __init__(self):
        self.obj = pythonProcess.PythonProcess_new()

    def Setup(self):
        return pythonProcess.PythonProcess_Setup(self.obj)

    def WriteInput(self, input_):
        pythonProcess.PythonProcess_WriteInput(self.obj, input_)

    def IsCompileComplete(self):
        return pythonProcess.PythonProcess_IsCompileComplete(self.obj)

    def Wait(self):
        pythonProcess.PythonProcess_Wait(self.obj)

    def Delete(self):
        pythonProcess.PythonProcess_delete(self.obj)