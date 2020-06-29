(define-module (python)
  #:export (py-initialize
            pylong-from-long
            pylong-as-long
            pyfloat-from-double
            pyfloat-as-double
            py-compile-string
            py-finalize
            py-file-input
            py-eval-input
            py-single-input))
            
(load-extension "python-guile" "init_python")





