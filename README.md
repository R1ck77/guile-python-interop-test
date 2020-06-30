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

I'd like to explore also automatic code generation: being able to generate the Guile extension using Guile to generate the functions would be recursively nice.

## Bugs

Many, but those are the ones I know:

- I have yet to figure out if/how to deallocate C strings obtained from SCM values
- functions are an arbitrary mix of literal translation (values, etc) and misguided high-level meddling (errors are thrown if NULL is returned)
