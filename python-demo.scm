(define-module (python-demo))
(use-modules (ice-9 format))
(use-modules (high-level))
(use-modules (python))

(with-python
 (let ((locals (get-builtins-copy)))
   (pydict-set-item-string locals "a" (pyfloat-from-double 4))
   (pydict-set-item-string locals "b" (pyfloat-from-double 3))   
   (let ((result (run-code "import math

def f(x, y):
    return math.sqrt(math.pow(x, 2) + math.pow(y, 2))

c = f(a, b)
" locals locals)))
     (format #t "The result of the script execution is: ~g\n" (pyfloat-as-double
                                                               (incref!
                                                                (pydict-get-item-string (assq-ref result 'locals) "c")))))))


