
liquid-dsp
==========

Software-Defined Radio Digital Signal Processing Library -
[https://liquidsdr.org](https://liquidsdr.org)

[![Core CI](https://github.com/jgaeddert/liquid-dsp/actions/workflows/core.yml/badge.svg)](https://github.com/jgaeddert/liquid-dsp/actions/workflows/core.yml)
[![codecov](https://codecov.io/gh/jgaeddert/liquid-dsp/branch/master/graph/badge.svg?token=ht8VIhp302)](https://codecov.io/gh/jgaeddert/liquid-dsp)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://choosealicense.com/licenses/mit/)
[![Packaging status](https://repology.org/badge/tiny-repos/liquid-dsp.svg)](https://repology.org/project/liquid-dsp/versions)

liquid-dsp is a free and open-source digital signal processing (DSP)
library designed specifically for software-defined radios on embedded
platforms. The aim is to provide a lightweight DSP library that does not
rely on a myriad of external dependencies or proprietary and otherwise
cumbersome frameworks. All signal processing elements are designed to be
flexible, scalable, and dynamic, including filters, filter design,
oscillators, modems, synchronizers, complex mathematical operations, and
much more.

```c
// get in, manipulate data, get out
#include <liquid/liquid.h>
int main() {
    unsigned int M  = 4;     // interpolation factor
    unsigned int m  = 12;    // filter delay [symbols]
    float        As = 60.0f; // filter stop-band attenuation [dB]

    // create interpolator from prototype
    firinterp_crcf interp = firinterp_crcf_create_kaiser(M,m,As);
    float complex x = 1.0f;  // input sample
    float complex y[M];      // interpolated output buffer

    // repeat on input sample data as needed
    {
        firinterp_crcf_execute(interp, x, y);
    }

    // destroy interpolator object
    firinterp_crcf_destroy(interp);
    return 0;
}
```

For more information, please refer to the
[documentation](https://liquidsdr.org/doc) online.

## Installation and Dependencies ##

liquid-dsp only relies on `libc` and `libm` (standard C and math)
libraries to run; however liquid will take advantage of other libraries
(such as [FFTW](http://www.fftw.org)) if they are available.

If you build from the Git repository you will also need to install autotools
for generating the `configure.sh` script (e.g.
`brew install autoconf automake` on macOS,
`sudo apt-get install automake autoconf` on Debian variants).

### Installation ###

The recommended way to obtain the source code is to clone the entire
[repository](https://github.com/jgaeddert/liquid-dsp) from
[GitHub](https://github.com):

    git clone git://github.com/jgaeddert/liquid-dsp.git

Building and installing the main library is a simple as

    ./bootstrap.sh
    ./configure
    make
    sudo make install

If you are installing on Linux for the first time, you will also need
to rebind your dynamic libraries with `sudo ldconfig` to make the
shared object available.
This is not necessary on macOS.

If you decide that you want to remove the installed DSP library, simply
run

    sudo make uninstall

Seriously, I won't be offended.

### Run all test scripts ###

Source code validation is a critical step in any software library,
particularly for verifying the portability of code to different
processors and platforms. Packaged with liquid-dsp are a number of
automatic test scripts to validate the correctness of the source code.
The test scripts are located under each module's `tests/` directory and
take the form of a C source file. liquid includes a framework for
compiling, linking, and running the tests, and can be invoked with the
make target `check`, viz.

    make check

There are currently more than 110,000 checks to verify functional correctness.
Drop me a line if these aren't running on your platform.

### Testing Code Coverage ###

In addition to the full test suite, you can configure `gcc` to export symbol
files to check for code coverage and then use `gcovr` to generate a full
report of precisely which lines are covered in the autotests. These symbol
files aren't generated by default and need to be enabled at compile-time
through a configure flag:

    ./configure --enable-coverage

Running the tests and generating the report through `gcovr` can be invoked
with the `coverage` make target:

    make coverage

### Examples ###

Nearly all signal processing elements have a corresponding example in
the `examples/` directory.  Most example scripts generate an output
`.m` file for plotting with [GNU octave](https://www.gnu.org/software/octave/)
All examples are built as stand-alone programs and can be compiled with
the make target `examples`:

    make examples

Sometimes, however, it is useful to build one example individually.
This can be accomplished by directly targeting its binary
(e.g. `make examples/modem_example`). The example then can be run at the
command line, viz. `./examples/modem_example`.

### Benchmarking tool ###

Packaged with liquid are benchmarks to determine the speed each signal
processing element can run on your machine. Initially the tool provides
an estimate of the processor's clock frequency and will then estimate
the number of trials so that each benchmark will take between 50 and
500 ms to run. You can build and run the benchmark program with the
following command:

    make bench

### C++

Compiling and linking to C++ programs is straightforward.
Just include `<complex>` before `<liquid/liquid.h>` and use 
`std::complex<float>` in favor of `float complex`.
Here is the same example as the one above but in C++ instead of C:

```c++
// get in, manipulate data, get out
#include <complex>
#include <liquid/liquid.h>
int main() {
    unsigned int M  = 4;     // interpolation factor
    unsigned int m  = 12;    // filter delay [symbols]
    float        As = 60.0f; // filter stop-band attenuation [dB]

    // create interpolator from prototype
    firinterp_crcf interp = firinterp_crcf_create_kaiser(M,m,As);
    std::complex<float> x = 1.0f;   // input sample
    std::complex<float> y[M];       // interpolated output buffer

    // repeat on input sample data as needed
    {
        firinterp_crcf_execute(interp, x, y);
    }

    // destroy interpolator object
    firinterp_crcf_destroy(interp);
    return 0;
}
```

### PlatformIO ###

Install [platformio](https://platformio.org)
(`brew install platformio` on macOS,
`sudo -H python3 -m pip install -U platformio` on Linux).
Add `liquid-dsp` to your `platform.io` list of dependencies:

```ini
[env:native]
platform = native
lib_deps = https://github.com/jgaeddert/liquid-dsp.git
```

## Available Modules ##

  * _agc_: automatic gain control, received signal strength
  * _audio_: source audio encoders/decoders: cvsd, filterbanks
  * _buffer_: internal buffering, circular/static, ports (threaded)
  * _channel_: additive noise, multi-path fading, carrier phase/frequency
        offsets, timing phase/rate offsets
  * _dotprod_: inner dot products (real, complex), vector sum of squares
  * _equalization_: adaptive equalizers: least mean-squares, recursive
        least squares, semi-blind
  * _fec_: basic forward error correction codes including several
        Hamming codes, single error correction/double error detection,
        Golay block code, as well as several checksums and cyclic
        redundancy checks, interleaving, soft decoding
  * _fft_: fast Fourier transforms (arbitrary length), discrete sin/cos
        transforms
  * _filter_: finite/infinite impulse response, polyphase, hilbert,
        interpolation, decimation, filter design, resampling, symbol
        timing recovery
  * _framing_: flexible framing structures for amazingly easy packet
        software radio; dynamically adjust modulation and coding on the
        fly with single- and multi-carrier framing structures
  * _math_: transcendental functions not in the C standard library
        (gamma, besseli, etc.), polynomial operations (curve-fitting,
        root-finding, etc.)
  * _matrix_: basic math, LU/QR/Cholesky factorization, inversion,
        Gauss elimination, Gram-Schmidt decomposition, linear solver,
        sparse matrix representation
  * _modem_: modulate, demodulate, PSK, differential PSK, QAM, optimal
        QAM, as well as analog and non-linear digital modulations GMSK)
  * _multichannel_: filterbank channelizers, OFDM
  * _nco_: numerically-controlled oscillator: mixing, frequency
        synthesis, phase-locked loops
  * _optim_: (non-linear optimization) Newton-Raphson, evoluationary
        algorithms, gradient descent, line search
  * _quantization_: analog/digital converters, compression/expansion
  * _random_: (random number generators) uniform, exponential, gamma,
        Nakagami-m, Gauss, Rice-K, Weibull
  * _sequence_: linear feedback shift registers, complementary codes,
        maximal-length sequences
  * _utility_: useful miscellany, mostly bit manipulation (shifting,
        packing, and unpacking of arrays)
  * _vector_: generic vector operations

### License ###

liquid projects are released under the X11/MIT license.
By default, this project will try to link to [FFTW](http://www.fftw.org) if it
is available on your build platform.
Because FFTW starting with version 1.3 is
[licensed](http://www.fftw.org/faq/section1.html)
under the [GNU General Public License v2](http://www.fftw.org/doc/License-and-Copyright.html)
this unfortunately means that (and I'm clearly not a lawyer, here)
you cannot distribute `liquid-dsp` without also distributing the source code
if you link to FFTW.
This is a similar situation with the classic
[libfec](https://github.com/quiet/libfec)
which uses the
[GNU Lesser GPL](https://www.gnu.org/licenses/licenses.html#LGPL).
Finally, `liquid-dsp` makes extensive use of GNU
[autoconf](https://www.gnu.org/software/autoconf/),
[automake](https://www.gnu.org/software/automake/),
and related tools.
These are fantastic libraires with amazing functionality and their authors
should be lauded for their efforts.
In a similar vain, much the software I write for a living I give away for
free;
however I believe in more permissive licenses to allow individuals the
flexibility to use software with more flexibility.
If these restrictions are not acceptible, `liquid-dsp` can be compiled and run
without use of these external libraries, albeit a bit slower and with limited
functionality.

Short version: this code is copyrighted to me (Joseph D. Gaeddert),
I give you full permission to do whatever you want with it except remove my
name from the credits.
Seriously, go nuts! but take caution when linking to other libraries with
different licenses.
See the [license](https://opensource.org/licenses/MIT) for specific terms.

