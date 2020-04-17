import ctypes

class SubprocessWrapper(object):
    class c_char_p_c4d(ctypes.c_char_p):
        def __str__(self):
            return str(self.value, 'utf-8', errors='ignore')

    def __init__(self):
        self.wrapper = ctypes.cdll.LoadLibrary('build/Subprocess.dll')

        self.wrapper.Subprocess_new.restype = ctypes.c_void_p
        self.wrapper.Subprocess_GetErrorMessage.restype = SubprocessWrapper.c_char_p_c4d
        self.wrapper.Subprocess_GetErrorMessage.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_Setup.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_WriteInput.argtypes = [ctypes.c_void_p, ctypes.c_wchar_p]
        self.wrapper.Subprocess_IsCompileComplete.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_delete.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_WasError.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_Wait.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_Release.argtypes = [ctypes.c_void_p]
        self.wrapper.Subprocess_SetError.argtypes = [ctypes.c_void_p, ctypes.c_bool]

        self.obj = self.wrapper.Subprocess_new()

    def Setup(self):
        return self.wrapper.Subprocess_Setup(self.obj)

    def WriteInput(self, input_):
        self.wrapper.Subprocess_WriteInput(self.obj, input_)

    def IsCompileComplete(self):
        return self.wrapper.Subprocess_IsCompileComplete(self.obj)

    def Wait(self):
        self.wrapper.Subprocess_Wait(self.obj)

    def Delete(self):
        self.wrapper.Subprocess_delete(self.obj)

    def WasError(self):
        return self.wrapper.Subprocess_WasError(self.obj)

    def GetErrorMessage(self):
        return self.wrapper.Subprocess_GetErrorMessage(self.obj)

    def Release(self):
        self.wrapper.Subprocess_Release(self.obj)

    def SetError(self, error):
        self.wrapper.Subprocess_SetError(self.obj, error)