(define-module (python-demo))
(use-modules (high-level))
(use-modules (python))

(py-initialize)

(define locals (get-builtins-copy))
(run-code "print(\"Hello, World!\")" locals locals)


(py-finalize)
