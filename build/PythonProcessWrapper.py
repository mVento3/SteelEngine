import ctypes

wrapper = ctypes.cdll.LoadLibrary('build/PythonProcess.dll')

class PythonProcessWrapper(object):
    class c_char_p_c4d(ctypes.c_char_p):
        def __str__(self):
            return str(self.value, 'utf-8')

    def __init__(self):
        self.obj = wrapper.PythonProcess_new()

        wrapper.PythonProcess_GetErrorMessage.restype = PythonProcessWrapper.c_char_p_c4d

    def Setup(self):
        return wrapper.PythonProcess_Setup(self.obj)

    def WriteInput(self, input_):
        wrapper.PythonProcess_WriteInput(self.obj, input_)

    def IsCompileComplete(self):
        return wrapper.PythonProcess_IsCompileComplete(self.obj)

    def Wait(self):
        wrapper.PythonProcess_Wait(self.obj)

    def Delete(self):
        wrapper.PythonProcess_delete(self.obj)

    def WasError(self):
        return wrapper.PythonProcess_WasError(self.obj)

    def GetErrorMessage(self):
        return wrapper.PythonProcess_GetErrorMessage(self.obj)