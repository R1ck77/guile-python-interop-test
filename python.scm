(define-module (python)
  #:export (py-initialize
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
            py-file-input
            py-eval-input
            py-single-input))
            
(load-extension "python-guile" "init_python")





