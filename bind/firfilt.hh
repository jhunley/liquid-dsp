// firfilt c++ bindings
#ifndef __FIRFILT_HH__
#define __FIRFILT_HH__

#include <complex>
#include <liquid/liquid.h>
#include "liquid.python.hh"

class firfilt
{
  public:
    // external coefficients
    firfilt(float * _h, unsigned int _n) { q = firfilt_crcf_create(_h, _n); }

    // default: kaiser low-pass
    firfilt(unsigned int _n=51, float _fc=0.25, float _As=60.0f, float _mu=0.0f)
        { q = firfilt_crcf_create_kaiser(_n, _fc, _As, _mu); }

    // rnyquist
    firfilt(int _type, unsigned int _k, float _m=7, float _beta=0.25f, float _mu=0.0f)
        { q = firfilt_crcf_create_rnyquist(_type, _k, _m, _beta, _mu); }

    // destructor
    ~firfilt() { firfilt_crcf_destroy(q); }

    // reset object
    void reset() { firfilt_crcf_reset(q); }

    //
    void display() { firfilt_crcf_print(q); }

    // push/execute one sample at a time
    std::complex<float> execute(std::complex<float> _x)
    {
        std::complex<float> y;
        firfilt_crcf_push(q, _x);
        firfilt_crcf_execute(q, &y);
        return y;
    }

  private:
    firfilt_crcf q;

#ifdef PYTHONLIB
  public:
    void py_execute(py::array_t<std::complex<float>> & _buf)
    {
        // get buffer info
        py::buffer_info info = _buf.request();

        // verify input size and dimensions
        if (info.itemsize != sizeof(std::complex<float>))
            throw std::runtime_error("invalid input numpy size, use dtype=np.csingle");
        if (info.ndim != 1)
            throw std::runtime_error("invalid number of input dimensions, must be 1-D array");

        // comptue sample size, number of samples in buffer, and stride between samples
        size_t       s           = sizeof(std::complex<float>);
        unsigned int num_samples = info.shape[0];
        unsigned int stride      = info.strides[0]/s;

        // convert buffers to pointers
        std::complex<float> * ptr = (std::complex<float>*) info.ptr;

        // run filter (in place)
        for (auto i=0U; i<num_samples; i++)
            ptr[i*stride] = execute(ptr[i*stride]);
    }
#endif
};

#ifdef PYTHONLIB
void init_firfilt(py::module &m)
{
    py::class_<firfilt> f(m, "firfilt");
        f.def(py::init<unsigned int,float,float,float>(),
             py::arg("n")=51, py::arg("fc")=0.25, py::arg("As")=60, py::arg("mu")=0)
        .def("reset",   &firfilt::reset,      "reset object's internal state")
        .def("execute", &firfilt::py_execute, "execute on a block of samples")
        ;
}
#endif

#endif //__FIRFILT_HH__