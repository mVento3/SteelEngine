import ctypes

pythonProcess = ctypes.cdll.LoadLibrary('build/PythonProcess.dll')

class PythonProcess(object):
    class c_char_p_c4d(ctypes.c_char_p):
        def __str__(self):
            return str(self.value, 'utf-8')

    def __init__(self):
        self.obj = pythonProcess.PythonProcess_new()

        pythonProcess.PythonProcess_GetErrorMessage.restype = PythonProcess.c_char_p_c4d

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

    def WasError(self):
        return pythonProcess.PythonProcess_WasError(self.obj)

    def GetErrorMessage(self):
        return pythonProcess.PythonProcess_GetErrorMessage(self.obj)