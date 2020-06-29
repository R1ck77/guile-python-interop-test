## Guile interoperability tests

The purpose of this test is to check the capabilities of Guile as an extensible/glue language.

In order to better understand what the language is capable of and spot critical areas, I'm going to test a basic Python C/API integration

**Disclaimer** I'm a complete n00b about Guile and Scheme in general. I know a bit of Emacs Lisp, a bit of Clojure and that's it. I hope it will do.

## What's here

There is a `python` module which, when loaded, provides the following functions:

    py-initialize
    pylong-from-long
    pylong-as-long
    pyfloat-from-double
    pyfloat-as-double
    pydict-new
    pydict-copy
    py-compile-string
    py-finalize

and the following constants:

    py-file-input
    py-eval-input
    py-single-input
    
and that's it.

## Desiderata

Being able to compile and run a python script, as well as having a bunch of other functions available would be nice.

I'd like to explore also automatic code generation: being able to generate the Guile extension using Guile to generate the functions would be recursively nice.
