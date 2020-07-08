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
    
All C code required for the Guile C wrapping around the Python is automatically generated by Guile itself via the incomplete (and probably amateurish) `meta-create-c-binding.scm` script.

`meta-create-c-binding.scm` reads a list of Python C/API definitions from `template.scm` (which is not valid Scheme…) and generates the appropriate dynamic code, which is then included in the C module code.

I know: SWIG can do this already, but it's still kinda cool because scheme bootstraps itself (anyway this is my opinion on the matter :) ).

## Desiderata

It would be great to being able to push the code generation to a point where all Python C/API functions can be added by just expanding the template.

## Bugs

Many, here is a semi-updated list of the ones I know:

- automatic code generation supports a somewhat limited combination of arguments/return values
- the template doesn't generate all code: the list of published symbols in `python.scm` module needs to be updated manually
