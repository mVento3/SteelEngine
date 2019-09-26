#include "PythonCore/Script.h"

namespace SteelEngine { namespace Script { namespace Python {

    void Script::LoadScript(const std::string& scriptFile)
    {
        PyObject* file = PyUnicode_DecodeFSDefault(scriptFile.c_str());
        m_Module = PyImport_Import(file);

        if(!m_Module)
        {
            PyErr_Print();
        }

        Py_DECREF(file);
    }

    Script::Script()
    {

    }

    Script::~Script()
    {

    }

    void* Script::MakeTuple(size_t size)
    {
        return PyTuple_New(size);
    }

    void Script::AddReference(void* object)
    {
        m_Objects.push_back((PyObject*)object);
    }

    void* Script::From(size_t value)
    {
        return PyLong_FromSize_t(value);
    }

    void* Script::From(int value)
    {
        return PyLong_FromSize_t(value);
    }

    void* Script::From(double value)
    {
        return PyLong_FromDouble(value);
    }

    void* Script::From(const std::string& value)
    {
        return PyUnicode_FromString(value.c_str());
    }

    void* Script::From(const char* value)
    {
        return PyUnicode_FromString(value);
    }

    size_t Script::ToSizeT(void* value)
    {
        return PyLong_AsLong((PyObject*)value);
    }

    double Script::ToDouble(void* value)
    {
        return PyLong_AsDouble((PyObject*)value);
    }

    const char* Script::ToString(void* value)
    {
        PyObject* str = PyUnicode_AsEncodedString((PyObject*)value, "utf-8", "~E~");
        const char* bytes = PyBytes_AsString(str);

        // Py_XDECREF(str);

        m_Objects.push_back(str);

        return bytes;
    }

    void Script::DecRef(void* ref)
    {
        Py_DECREF(ref);
    }

    void Script::SetTupleItem(void* args, size_t index, void* value)
    {
        PyTuple_SetItem((PyObject*)args, index, (PyObject*)value);
    }

    void* Script::Call(const std::string& name, void* args)
    {
        PyObject* function = PyObject_GetAttrString(m_Module, name.c_str());

        if(function && PyCallable_Check(function))
        {
            m_Objects.push_back(function);

            return PyObject_CallObject(function, (PyObject*)args);
        }

        return 0;
    }

    void Script::operator()(const RecompiledEvent& event)
    {

    }

}}}