//#define VERBOSE

#ifdef VERBOSE
#include "common/get_time.h"
#endif

#include "spatialGraph.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<size_t> py_gabriel(py::array_t<double, py::array::c_style | py::array::forcecast> array)
{

  if (array.ndim() != 2)
    throw std::runtime_error("Input should be 2-D NumPy array");

  if (sizeof(pargeo::point<2>) != 16)
    throw std::runtime_error("sizeof(pargeo::point<2>) != 16, check point.h");

  int dim = array.shape()[1];

  size_t n = array.size() / dim;

#ifdef VERBOSE
  timer t;
#endif

  if (dim == 2) {
#ifdef VERBOSE
    t.start();
#endif
    parlay::sequence<pargeo::point<2>> array_vec(n);
    std::memcpy(array_vec.data(), array.data(), array.size() * sizeof(double));
#ifdef VERBOSE
    std::cout << "::copy-in-time = " << t.get_next() << std::endl;
#endif

    parlay::sequence<edge> result_vec = spatialGraph<2>(array_vec);

#ifdef VERBOSE
    std::cout << "::compute-time = " << t.get_next() << std::endl;
#endif
    auto result = py::array_t<size_t>(2 * result_vec.size());
    auto result_buffer = result.request();
    size_t *result_ptr = (size_t *) result_buffer.ptr;
    std::memcpy(result_ptr,result_vec.data(),result_vec.size()*sizeof(size_t)*2);
#ifdef VERBOSE
    std::cout << "::copy-out-time = " << t.stop() << std::endl;
#endif
    return result;
  } else {
    throw std::runtime_error("Only supports 2d points.");
  }

}

PYBIND11_MODULE(pygabriel, m)
{
  m.doc() = "Generates edge list for a Gabriel graph for a point data set in R^2.";

  m.def("gabriel_graph", &py_gabriel, "Input: 2d-numpy array containing n points in R^2. Output: array E of size n*2, where E[i] and E[i+1] forall i are point indices represents an undirected edge of the resulting Gabriel graph.");
}