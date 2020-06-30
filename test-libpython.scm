(use-modules (ice-9 format))
(use-modules (python))

;;; Initialize Python
(display "Calling Python procedures\n")
(py-initialize)

(define (conversion-test value format-specifier to-function from-function)
  (format #t (string-append "About to convert ~" format-specifier " to Python…\n") value)  
  (let ((py-value (to-function value)))
    (format #t "Value converted to ~a! Getting the result back…\n" py-value)
    (let ((result (from-function py-value)))
      (format #t (string-append "Got ~" format-specifier " back\n") result)
      (if (not (= value result))
          (error (format #f (string-append "Value not converted correctly: ~" format-specifier
                                           " expected, got ~" format-specifier
                                           "!")
                         value result))))))

(define (long-conversion-test value)
  (conversion-test value "d" pylong-from-long pylong-as-long))

(define (double-conversion-test value)
  (conversion-test value "f" pyfloat-from-double pyfloat-as-double))

(define (finalization-test repetitions)
  (do ((i 0 (1+ i)))
      ((> i repetitions))
      (let ((value (+ i 100000)))
       (format #t "Creating a Python integer with ~d…\n" value)
       (pylong-from-long value)))
  (display "Forcing garbage collection…\n")
  (gc))

(define (dict-creation-test)
  (pydict-copy (pydict-new))
  (display "Dictionary created (no crash detected at least…)\n"))

(define (execution-test)
  (let ((compiled-code (py-compile-string "import sys\nsys.stdout.write(\"HELLO WORLD (FROM PYTHON)!\\n\")" "<file>" py-file-input))
        (builtins (pyeval-get-builtins))
        (globals (pydict-new)))
    (when (nil? compiled-code)
      (error "Compilation failed!"))
    (pydict-set-item-string globals "__builtins__" builtins)
    (let ((result (pyeval-eval-code compiled-code globals globals)))
      (format #t "Got a result: ~a…\n" result))))

(long-conversion-test 42)
(double-conversion-test 3.1415)
(dict-creation-test)
(execution-test)
(finalization-test 10)

;;; Finalize Python
(py-finalize)
(display "Test successful!\n")
