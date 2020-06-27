
(define python-lib (dynamic-link "libpython-guile.so"))
(dynamic-call "init_python" python-lib)

(display "Calling Python procedures\n")
(py-initialize)
(py-finalize)
(display "Invocation successful!\n")
