
#include <Python.h>

#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

enum OP: std::uint_fast8_t { OP_Unk, OP_Gt, OP_Mult, OP_Ret };

typedef struct {


  OP op;

  std::uint_fast32_t arg1_idx;
  std::uint_fast32_t arg2_idx;

  void* next_left;
  void* next_right;

} AST;

static AST create_ast(PyObject* py_func) {
  AST ast;



  return ast;
}

static std::int_fast32_t evaluate_ast(AST* ast, std::int_fast32_t* row_data, std::uint_fast32_t num_cols) {
  std::int_fast32_t r = 0;


  return r;
}

static PyObject* vm_runit(PyObject *self, PyObject *args) {
  // always 1 object
  PyObject* py_func = PyTuple_GetItem(args, 0);
  if (!PyCallable_Check(py_func)) {
    std::cerr << "arg0 not a callable!" << std::endl;
    return NULL;
  }

  AST fast_func = create_ast(py_func);

  // Eval both on some data?
  std::uint_fast32_t num_rows = 10;
  std::uint_fast32_t num_cols = 2;
  std::vector<std::int_fast32_t> data;
  data.reserve(num_rows * num_cols);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> rand_dist(-100,100);

  for (std::uint_fast32_t i=0; i<num_rows*num_cols; i++) {
    data.push_back( rand_dist(rng) );
  }


  std::vector<PyObject*> py_row_data;
  for (std::uint_fast32_t row=0; row<num_rows; row++) {
    PyObject* tuple = PyTuple_New(num_cols);
    for (std::uint_fast32_t col=0; col<num_cols; col++) {
      PyTuple_SetItem(tuple, col, PyLong_FromLong( data[(num_cols * row) + col] ));
      //std::cerr << "pylong from long = " << std::setw(4) << data[(num_cols * row) + col] << "  py=" << PyLong_FromLong( data[(num_cols * row) + col] ) << std::endl;
    }
    py_row_data.push_back(tuple);
  }

  std::vector<PyObject*> py_return_data;
  for (std::uint_fast32_t row=0; row<num_rows; row++) {
    PyObject* py_ret = PyObject_Call(py_func, py_row_data[row], NULL);
    //std::cerr << "py_ret["<<row<<"] = " << py_ret << "  py_func=" << py_func << "   py_row" << py_row_data[row] <<  std::endl;
    py_return_data.push_back(py_ret);
  }


  // Now evaluate the AST;
  std::vector<std::int_fast32_t> ast_return_data;
  for (std::uint_fast32_t row=0; row<num_rows; row++) {
    ast_return_data.push_back(
      evaluate_ast(&fast_func, &data[row * num_cols], num_cols)
    );
  }

  // Finally print results as a table:
  for (std::uint_fast32_t col=0; col<num_cols; col++) {
    if (col == num_cols-1) {
      std::cout << std::setw(5) << "rows" << ",";
    }
    else {
      std::cout << std::setw(5) << "" << ",";
    }
  }
  std::cout << " |" << std::setw(5) << "py" << std::setw(5) << "vm" << std::endl;
  std::cout << "-------------|----------" << std::endl;
  for (std::uint_fast32_t row=0; row<num_rows; row++) {
    for (std::uint_fast32_t col=0; col<num_cols; col++) {
      std::cout << std::setw(5) << data[(num_cols * row) + col] << ",";
    }
    std::cout << " |" << std::setw(5) << PyLong_AsLong(py_return_data[row]) << std::setw(5) << ast_return_data[row] << std::endl;
  }


  Py_RETURN_NONE;
}


static PyMethodDef VMMethods[] = {
    {"runit",  vm_runit, METH_VARARGS, "Execute a vm on some dummy data."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef vm_module = {
    PyModuleDef_HEAD_INIT,
    "vm",   /* name of module */
    "some vm logic", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    VMMethods
};

PyMODINIT_FUNC PyInit_vm(void) {
    PyObject* m = PyModule_Create(&vm_module);

    

    return m;
}





