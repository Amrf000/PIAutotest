#include "workerthread.h"
#include <iostream>
#include <QCoreApplication>
#include <QTextStream>
#include "mainwindow.h"
#include <frameobject.h>
//================
static PyObject* getRunFlag(PyObject *self, PyObject *args)
{
    //if(!PyArg_ParseTuple(args, ":numargs"))
    //   return NULL;
    return PyLong_FromLong((int)MainWindow::isRunnig);//numargs);
}
static PyObject* saveData(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if(!PyArg_ParseTuple(args, "O",&obj))
    {
        return PyLong_FromLong(0);
    }
    PyObject *iter = PyObject_GetIter(obj);
    if (!iter) {
        return PyLong_FromLong(0);
    }
    QVariantList ret;
    while (true) {
      PyObject *next = PyIter_Next(iter);
      if (!next) {
        break;
      }
      if (PyLong_Check(next)) {
          long foo = PyLong_AsLong(next);
          ret.append(foo);
          printf("%d\n",foo);
      }
      else if (PyFloat_Check(next)) {
          double foo = PyFloat_AsDouble(next);
          ret.append(foo);
          printf("%g\n",foo);
      }else{
          qDebug("saveData unknown type");
      }
    }
    WorkerThread* pt = dynamic_cast<WorkerThread*>(QThread::currentThread());
    if(pt)
    {
       emit pt->onReceive(ret);
    }
    return PyLong_FromLong(0);
}
static PyMethodDef EmbMethods[] = {
    {"getRunFlag", getRunFlag, METH_VARARGS,"Return the number of arguments received by the process."},
    {"saveData", saveData, METH_VARARGS,""},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "ekk", NULL, -1, EmbMethods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_ekk(void)
{
    return PyModule_Create(&EmbModule);
}
//============================
WorkerThread::WorkerThread(MainWindow* pMain,QPushButton* pStart,InstConfig vec):
    m_pMain(pMain),m_pStart(pStart),m_vec(vec)
{
}

WorkerThread::~WorkerThread()
{
   qDebug("~WorkerThread");
}

void WorkerThread::stop()
{
    MainWindow::isRunnig = false;
}
// overriding the QThread's run() method
void WorkerThread::run()
{
    QString workPath = QCoreApplication::applicationDirPath()+"/Orlando_AutoTest/vector/";
    QString scriptPath = workPath+m_vec.suiteName;

    if(MainWindow::isRunnig)
    {
        return;
    }
    MainWindow::isRunnig = true;
    PyImport_AppendInittab("emb", emb::PyInit_emb);
    PyImport_AppendInittab("ekk", &PyInit_ekk);
    Py_Initialize();
    PyImport_ImportModule("emb");
    //PyImport_ImportModule("ekk");

    PyRun_SimpleString("print(\'hello to console\')");
    //PyRun_SimpleString("import eb");
    //PyRun_SimpleString("print(eb.getRunFlag()");

    // here comes the ***magic***
    std::string buffer;
    //QTextEdit* pedit = m_pEdit;
    MainWindow* pMain = m_pMain;
    {
        // switch sys.stdout to custom handler
        emb::stdout_write_type write = [pMain,&buffer] (std::string s) {
            buffer += s;
            emit pMain->append(s.c_str());
        };
        emb::set_stdout(write);
        PyRun_SimpleString("import os,ekk");
        workPath = "os.chdir(\'"+workPath+"\')";
        PyRun_SimpleString(workPath.toStdString().c_str());
        PyRun_SimpleString("print(os.getcwd())");
        //======================
        PyRun_SimpleString("volt_big = 1000");
        PyRun_SimpleString("freq_big = 2180000");
        PyRun_SimpleString("volt_mem = 1000");
        //==========================
        std::string filename = scriptPath.toStdString();
        QFile fn(scriptPath);
        if(!fn.open(QIODevice::ReadOnly))
        {
            std::string info = "open fail errno ="+std::to_string(errno)+" reason ="+strerror(errno);
            emit pMain->append(info.c_str());
        }else{
            QTextStream s(&fn);
            QString script = s.readAll();
            std::string cod = script.toStdString();
            //=========================
            PyObject* code = Py_CompileString(cod.c_str(), filename.c_str(), Py_file_input);//Py_file_input
            PyObject *ptype, *pvalue, *ptraceback;
            if(code!=NULL)
            {
                PyObject* main_module = PyImport_AddModule("__main__");
                PyObject* global_dict = PyModule_GetDict(main_module);
                PyObject* obj = PyEval_EvalCode(code, global_dict, global_dict);//local_dict
                PyObject* result = PyObject_Str(obj);
                PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);
                if((ptype != NULL))
                {
                    //std::string the_error_string, the_traceback_string;
                    if (pvalue != NULL)
                    {
                        PyObject* str_exc_value = PyObject_Repr(pvalue);
                        PyObject* pyExcValueStr = PyUnicode_AsEncodedString(str_exc_value, "utf-8", "Error ~");
                        const char *strExcValue =  PyBytes_AS_STRING(pyExcValueStr);
                        emit pMain->append(strExcValue);
                    }
                }
            }
        }
        emb::reset_stdout();
    }

    PyRun_SimpleString("print(\'hello to console again\')");
    Py_Finalize();

    // output what was written to buffer object
    std::cout << buffer << std::endl;
    MainWindow::isRunnig = false;
    emit m_pStart->toggle();
}

void WorkerThread::onReceive(QVariantList data)
{
    QString s;
    QTextStream out(&s);
    for(int i=0;i<data.size();++i)
    {
         out << " ";
         out << data[i].toString();
    }
    emit m_pMain->append(s);
}

//PyRun_SimpleString("#!/usr/bin/python");
//PyRun_SimpleString("import time");
//PyRun_SimpleString("print(\'hello to buffer\')");
//PyRun_SimpleString("time.sleep(3)");
//PyRun_SimpleString("print(3.14)");
//PyRun_SimpleString("time.sleep(3)");
//PyRun_SimpleString("print(\'still talking to buffer\')");
//PyRun_SimpleString("time.sleep(3)");
//FILE *fp = _Py_fopen(filename.c_str(), "r");//fopen
//if(fp==NULL)
//{
//    std::string info = "open fail errno ="+std::to_string(errno)+" reason ="+strerror(errno);
//   emit pMain->append(info.c_str());
//}else
//{
    // Get a reference to the main module.
//    PyObject* main_module = PyImport_AddModule("__main__");

    // Get the main module's dictionary
    // and make a copy of it.
//    PyObject* main_dict = PyModule_GetDict(main_module);
    //PyObject* main_dict_copy =
    //   PyDict_Copy(main_dict);
    //PyRun_SimpleFileEx(fp, filename,1);
//    PyCompilerFlags* flags;
//    PyRun_FileExFlags(fp,filename.c_str(),Py_file_input,main_dict,main_dict,true,flags);

//}
//===============
//PyTracebackObject* traceback = get_the_traceback();

//===============================
/*PyThreadState *tstate = PyThreadState_GET();
if (NULL != tstate && NULL != tstate->frame) {
    PyFrameObject *frame = tstate->frame;

    printf("Python stack trace:\n");
    while (NULL != frame) {
        // int line = frame->f_lineno;
        int line = PyCode_Addr2Line(frame->f_code, frame->f_lasti);
        const char *filename = PyBytes_AsString(frame->f_code->co_filename);
        const char *funcname = PyBytes_AsString(frame->f_code->co_name);
        printf("    %s(%d): %s\n", filename, line, funcname);
        frame = frame->f_back;
    }
}*/
//PyTracebackObject* traceback = get_the_traceback();
//int line = traceback->tb_lineno;
//const char* filename = PyBytes_AsString(traceback->tb_frame->f_code->co_filename);
//if(ptraceback != NULL){
//    auto the_traceback_string = PyBytes_AsString(ptraceback);
//}
//std::string str_error(the_error_string);
//std::string str_traceback(the_traceback_string);
//string message(str_error + "\n Traceback: " + str_traceback);
//emit pMain->append((str_error + "\n Traceback: " + str_traceback).c_str());
//Py_XDECREF(error_type);
//Py_XDECREF(the_error);
//Py_XDECREF(the_traceback);
//return message;
//PyObject * py_error_string = PyObject_Str(pvalue);
//PyObject * py_error_unicode = PyUnicode_FromObject(py_error_string);
//const char* char_string = PyUnicode_AS_DATA(py_error_unicode);
//auto char_length = PyUnicode_GET_DATA_SIZE(py_error_string);
//the_error_string = std::string(char_string, char_length);
//emit pMain->append((char*)(result));
//PyObject* local_dict = PyDict_New();
/*void eval(const char* s)
{
    PyCodeObject* code = (PyCodeObject*) Py_CompileString(s, "test", Py_file_input);
    PyObject* main_module = PyImport_AddModule("__main__");
    PyObject* global_dict = PyModule_GetDict(main_module);
    PyObject* local_dict = PyDict_New();
    PyObject* obj = PyEval_EvalCode(code, global_dict, local_dict);

    PyObject* result = PyObject_Str(obj);
    PyObject_Print(result, stdout, 0);
}*/
/*static int
tb_displayline(PyObject *f, PyObject *filename, int lineno, PyObject *name)
{
    int err;
    PyObject *line;

    if (filename == NULL || name == NULL)
        return -1;
    line = PyUnicode_FromFormat("  File \"%U\", line %d, in %U\n",
                                filename, lineno, name);
    if (line == NULL)
        return -1;
    err = PyFile_WriteObject(line, f, Py_PRINT_RAW);
    Py_DECREF(line);
    if (err != 0)
        return err;
    if (_Py_DisplaySourceLine(f, filename, lineno, 4))
        PyErr_Clear();
    return err;
}

static int
tb_printinternal(PyTracebackObject *tb, PyObject *f, long limit)
{
    int err = 0;
    long depth = 0;
    PyTracebackObject *tb1 = tb;
    while (tb1 != NULL) {
        depth++;
        tb1 = tb1->tb_next;
    }
    while (tb != NULL && err == 0) {
        if (depth <= limit) {
            err = tb_displayline(f,
                                 tb->tb_frame->f_code->co_filename,
                                 tb->tb_lineno,
                                 tb->tb_frame->f_code->co_name);
        }
        depth--;
        tb = tb->tb_next;
        if (err == 0)
            err = PyErr_CheckSignals();
    }
    return err;
}*/
