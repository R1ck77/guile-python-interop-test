(define-module (high-level))
(use-modules (python))

(define (incref py-object)
  (py-incref py-object)
  py-object)

(define (get-builtins-copy)
  (pydict-copy
   (incref
    (pyeval-get-builtins))))

(define (run-code code globals locals)
  12
  
  )


(export run-code)
(export get-builtins-copy)


