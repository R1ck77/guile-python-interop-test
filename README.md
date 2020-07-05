## Guile interoperability tests

The purpose of this test is to check the capabilities of Guile as an extensible/glue language.

In order to better understand what the language is capable of and spot critical areas, I'm going to test a basic Python C/API integration

**Disclaimer** I'm a complete n00b about Guile and Scheme in general. I know a bit of Emacs Lisp, a bit of Clojure and that's it. I hope it will do, but if you see strange stuff in the code, don't be surprised.

## What's here

There is a `python` module which, when loaded, provides the following functions:
    
    py-initialize
    pylong-from-long
    pylong-as-long
    pyfloat-from-double
    pyfloat-as-double
    pydict-new
    pydict-copy
    pydict-set-item-string
    py-compile-string
    pyeval-eval-code
    pyeval-get-builtins
    py-finalize

and the following constants:

    py-file-input
    py-eval-input
    py-single-input
    
and that's it.

## Desiderata

Having more API functions available would be nice.

I'm also exploring automatic code generation: being able to generate the Guile extension using Guile to create the functions would be recursively nice, and I'm slowly expanding this functionality to allow the porting of a generic N-arity Python function.

## Bugs

Many, here is a semi-updated list of the ones I know:

- automatic code generation supports a very limited combination of arguments/return values
- some code is missing, specifically no `scm_c_define_gsubr` is created yet, and the `python.scm` module is not updated with the generated symbols yet
