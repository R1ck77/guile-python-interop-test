## Guile interoperability tests

The purpose of this test was to check the capabilities of Guile for embedding and extending and understand if it's fit for some ideas I have in mind.

In order to better understand what the language is capable of and spot critical areas, I have created a basic Python C/API integration.

The test proved successful, and now the code in this repo can:

- bootstrap most of the C code required to implement (many, not all) C Python API functions
- build a `python-guile.so` library that exposes the low-level Python API to Guile
- show how to leverage the power of LISP to provide higher level abstractions (see a small example in `high-level.scm`)

### Disclaimer

I'm a complete n00b about Guile and Scheme in general, this being the first serious stab at using it. 

I know a bit of Emacs Lisp, a bit of Clojure and that's it. I hope it will do, but if you see strange stuff in the code, don't be surprised.

## Requirements

You need Guile-2.2 with all development packages installed, on my Ubuntu machine this means having the following packages:

    guile-2.2
    guile-2.2-dev
    guile-2.2-libs
    
and a recent version of Python 3 (>= 3.2) with the development and shared libraries installed.

If you want to benefit from the compilation script you will need:

    GNU Make
    GCC
    pkg-config

but if you know what you are doing you can probably use whatever and chain preprocessor and library options on your own.

## Usage

Type `make` to compile everything and run the `python-demo.scm` script.

If the execution is successful, you will end up with the message:

    The result of the script execution is: 5.0

While the actual result will probably not blow you away, what's under the cover may.

You just executed a (simple) Python script in a Python 3 interpreter using Scheme, all with in-memory/low-latency communication.

## What's here

There is a `python` module which, when loaded, provides a number of functions and constants. 

The list of functions currently includes:
    
    py-initialize
    py-finalize
    pylong-from-long-long
    pyfloat-from-double
    pyeval-get-builtins
    pyeval-eval-code
    pyfloat-as-double
    pylong-as-long-long
    py-compile-string
    pydict-new
    pydict-copy
    pydict-set-item-string
    pydict-get-item-string
    pydict-get-item
    pydict-del-item
    pydict-del-item-string
    pydict-clear
    pytuple-new
    pytuple-get-item
    pytuple-set-item
    py-incref
    py-decref
    pyimport-add-module
    pymodule-get-dict

where only the following constants are exported

    py-file-input
    py-eval-input
    py-single-input
    
All C code required for the Guile C wrapping around the Python API is automatically generated by Guile itself by adding entries to the `template.scm` file and using the `meta-create-c-binding.scm` script on it (see the `Makefile` for details).

`meta-create-c-binding.scm` reads a list of Python C/API definitions from a text file and generates the appropriate dynamic code, which is then included in the C module code.

I know: SWIG can do all this already, but I still think it's cool because Guile is sort of bootstrapping itself.

## Bugs

There is little to no testing for most of the code, mostly due to the fact it's the first real Guile code I write. I have created the small `test-libpython.scm` to cope with this limitation during development, though (it is a form of testing, although a poor's man one).

The code generation module `meta-create-c-binding.scm` has an hideous name and doesn't support all possible combination of arguments, most notably it doesn't support Python objects handlers required to support most `PyErr_` functions.

More a feature than a bug, the Python/Guile interface is based on the low-level version of the Python API functions. This means it has the same reference counting quirks, and plays very poorly with GC without proper high level wrapping (see `high-level.scm`)

## License

This code is distributed under the GNU General Public License v2.

This code can be freely used in non profit software, as long as proper credit is provided.

Commercial use of the code is reserved.

No matter how the code is used, no warranty whatsoever is provided.


