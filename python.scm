(define-module (python)
  #:export (py-initialize
            pylong-from-long
            pylong-as-long
            pyfloat-from-double
            pyfloat-as-double
            py-finalize))
            
(load-extension "python-guile" "init_python")





