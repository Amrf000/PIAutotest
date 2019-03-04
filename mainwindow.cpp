#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <iostream>
#include <string>
#include <QThread>
#include <string.h>
#include <errno.h>
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

PyObject* Stdout_write(PyObject* self, PyObject* args)
{
    std::size_t written(0);
    Stdout* selfimpl = reinterpret_cast<Stdout*>(self);
    if (selfimpl->write)
    {
        char* data;
        if (!PyArg_ParseTuple(args, "s", &data))
            return 0;

        std::string str(data);
        selfimpl->write(str);
        written = str.size();
    }
    return PyLong_FromSize_t(written);
}

PyObject* Stdout_flush(PyObject* self, PyObject* args)
{
    // no-op
    return Py_BuildValue("");
}

PyMethodDef Stdout_methods[] =
{
    {"write", Stdout_write, METH_VARARGS, "sys.stdout.write"},
    {"flush", Stdout_flush, METH_VARARGS, "sys.stdout.flush"},
    {0, 0, 0, 0} // sentinel
};

PyTypeObject StdoutType =
{
    PyVarObject_HEAD_INIT(0, 0)
    "emb.StdoutType",     /* tp_name */
    sizeof(Stdout),       /* tp_basicsize */
    0,                    /* tp_itemsize */
    0,                    /* tp_dealloc */
    0,                    /* tp_print */
    0,                    /* tp_getattr */
    0,                    /* tp_setattr */
    0,                    /* tp_reserved */
    0,                    /* tp_repr */
    0,                    /* tp_as_number */
    0,                    /* tp_as_sequence */
    0,                    /* tp_as_mapping */
    0,                    /* tp_hash  */
    0,                    /* tp_call */
    0,                    /* tp_str */
    0,                    /* tp_getattro */
    0,                    /* tp_setattro */
    0,                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,   /* tp_flags */
    "emb.Stdout objects", /* tp_doc */
    0,                    /* tp_traverse */
    0,                    /* tp_clear */
    0,                    /* tp_richcompare */
    0,                    /* tp_weaklistoffset */
    0,                    /* tp_iter */
    0,                    /* tp_iternext */
    Stdout_methods,       /* tp_methods */
    0,                    /* tp_members */
    0,                    /* tp_getset */
    0,                    /* tp_base */
    0,                    /* tp_dict */
    0,                    /* tp_descr_get */
    0,                    /* tp_descr_set */
    0,                    /* tp_dictoffset */
    0,                    /* tp_init */
    0,                    /* tp_alloc */
    0,                    /* tp_new */
};

PyModuleDef embmodule =
{
    PyModuleDef_HEAD_INIT,
    "emb", 0, -1, 0,
};

// Internal state
PyObject* g_stdout;
PyObject* g_stdout_saved;

PyMODINIT_FUNC PyInit_emb(void)
{
    g_stdout = 0;
    g_stdout_saved = 0;

    StdoutType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&StdoutType) < 0)
        return 0;

    PyObject* m = PyModule_Create(&embmodule);
    if (m)
    {
        Py_INCREF(&StdoutType);
        PyModule_AddObject(m, "Stdout", reinterpret_cast<PyObject*>(&StdoutType));
    }
    return m;
}

void set_stdout(stdout_write_type write)
{
    if (!g_stdout)
    {
        g_stdout_saved = PySys_GetObject("stdout"); // borrowed
        g_stdout = StdoutType.tp_new(&StdoutType, 0, 0);
    }

    Stdout* impl = reinterpret_cast<Stdout*>(g_stdout);
    impl->write = write;
    PySys_SetObject("stdout", g_stdout);
}

void reset_stdout()
{
    if (g_stdout_saved)
        PySys_SetObject("stdout", g_stdout_saved);

    Py_XDECREF(g_stdout);
    g_stdout = 0;
}

} // namespace emb

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bool MainWindow::isRunnig = false;

MainWindow::~MainWindow()
{
    delete ui;
}
//================
static PyObject* getRunFlag(PyObject *self, PyObject *args)
{
    //if(!PyArg_ParseTuple(args, ":numargs"))
    //   return NULL;
    return PyLong_FromLong((int)MainWindow::isRunnig);//numargs);
}
static PyMethodDef EmbMethods[] = {
    {"getRunFlag", getRunFlag, METH_VARARGS,"Return the number of arguments received by the process."},
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

class WorkerThread : public QThread
{
public:
    // constructor
    // set name using initializer
    explicit WorkerThread(MainWindow* pMain,QPushButton* pStart,QTextEdit* pEdit):
        m_pMain(pMain),m_pStart(pStart),m_pEdit(pEdit)
    {

    }

    void stop()
    {
         MainWindow::isRunnig = false;
    }

    // overriding the QThread's run() method
    void run()
    {
        if(MainWindow::isRunnig)
        {
            return;
        }
        MainWindow::isRunnig = true;
        PyImport_AppendInittab("emb", emb::PyInit_emb);
        PyImport_AppendInittab("ekk", &PyInit_ekk);
        Py_Initialize();
        PyImport_ImportModule("emb");
        //PyImport_ImportModule("eb");

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
            //PyRun_SimpleString("#!/usr/bin/python");
            //PyRun_SimpleString("import time");
            //PyRun_SimpleString("print(\'hello to buffer\')");
            //PyRun_SimpleString("time.sleep(3)");
            //PyRun_SimpleString("print(3.14)");
            //PyRun_SimpleString("time.sleep(3)");
            //PyRun_SimpleString("print(\'still talking to buffer\')");
            //PyRun_SimpleString("time.sleep(3)");
            const char *filename = "hello.py";
            FILE *fp = _Py_fopen(filename, "r");//fopen
            if(fp==NULL)
            {
                std::string info = "open fail errno ="+std::to_string(errno)+" reason ="+strerror(errno);
                emit pMain->append(info.c_str());
            }else{
                // Get a reference to the main module.
                PyObject* main_module =
                   PyImport_AddModule("__main__");

                // Get the main module's dictionary
                // and make a copy of it.
                PyObject* main_dict =
                   PyModule_GetDict(main_module);
                PyObject* main_dict_copy =
                   PyDict_Copy(main_dict);
                //PyRun_SimpleFileEx(fp, filename,1);
                PyRun_File(fp,filename,Py_file_input,main_dict,main_dict);
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
private:
    QTextEdit* m_pEdit;
    MainWindow* m_pMain;
    QPushButton* m_pStart;
};

void MainWindow::init()
{
    connect(this, SIGNAL(append(QString)),ui->textEdit, SLOT(append(QString)));
}
WorkerThread* pWorker = NULL;
void MainWindow::on_start_toggled(bool running)
{
    //printf("running=%d\n",(int)running);
    if(running){//启动
        if(MainWindow::isRunnig){
            return;
        }
        if(pWorker!=NULL)
        {
            return;
        }
        //MainWindow::isRunnig = true;
        ui->start->setText("stop");
        pWorker = new WorkerThread(this,ui->start,ui->textEdit);
        connect( pWorker, &WorkerThread::finished,  pWorker, &QObject::deleteLater);
        pWorker->start();
    }else{//停止
        if(MainWindow::isRunnig){//还在运行
           ui->start->setChecked(true);
           ui->start->setDisabled(true);
           pWorker->stop();
        }else{
            //pWorker->terminate();
            //pWorker->deleteLater();
            //delete pWorker;
            pWorker = NULL;
            ui->start->setText("start");
            ui->start->setDisabled(false);
        }
    }
}

