(define-module (high-level))
(use-modules (python))

(define-macro (with-python . code)
  (let ((exception (make-symbol "exception")))
    `(begin
       (use-modules (srfi srfi-18))
       (py-initialize)
       (with-exception-handler
        (lambda (,exception)
          (py-finalize)
          (raise ,exception))
        (lambda ()
          ,@code
          (py-finalize))))))


(define (incref! py-object)
  (py-incref py-object)
  py-object)


(define (get-builtins-copy)
  (pydict-copy
   (incref! (pymodule-get-dict
     (incref! (pyimport-add-module "__main__"))))))

(define (run-code code globals locals)
  (let ((compiled-code (py-compile-string code "<guile>" py-file-input)))
    (list (cons 'result (and compiled-code
                             (pyeval-eval-code compiled-code globals locals)))
          (cons 'locals locals)
          (cons 'globals globals))))


(export with-python)
(export run-code)
(export get-builtins-copy)
(export incref!)
