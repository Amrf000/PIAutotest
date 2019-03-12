#ifndef EMB_H
#define EMB_H
#include <functional>

#ifdef _DEBUG
#undef _DEBUG
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")
#define _DEBUG
#else
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")
#endif

namespace emb
{
typedef std::function<void(std::string)> stdout_write_type;

struct Stdout
{
    PyObject_HEAD
    stdout_write_type write;
};

PyObject* Stdout_write(PyObject* self, PyObject* args);
PyObject* Stdout_flush(PyObject* self, PyObject* args);
extern PyMethodDef Stdout_methods[];
extern PyTypeObject StdoutType;
// Internal state
extern PyObject* g_stdout;
extern PyObject* g_stdout_saved;
extern PyObject* g_stderr_saved;
extern PyModuleDef embmodule;
PyMODINIT_FUNC PyInit_emb(void);
void set_stdout(stdout_write_type write);
void reset_stdout();
} // namespace emb

#endif // EMB_H
