(use-modules (ice-9 format))
(use-modules (python))

;;; Initialize Python
(display "Calling Python procedures\n")
(py-initialize)

(define (conversion-test value)
  (format #t "About to convert ~d to Python…\n" value)  
  (let ((py-long (pylong-from-long value)))
    (format #t "Value converted to ~a! Getting the result back…\n" py-long)
    (let ((result (pylong-as-long py-long)))
      (format #t "Got ~d back\n" result)
      (if (not (= value result))
          (error (format #f "Value not converted correctly: ~d expected, got ~d!" value result))))))

(define (finalization-test repetitions)
  (do ((i 0 (1+ i)))
      ((> i repetitions))
      (let ((value (+ i 100000)))
       (format #t "Creating a Python integer with ~d…\n" value)
       (pylong-from-long value)))
  (display "Forcing garbage collection…\n")
  (gc))

(conversion-test 42)
(finalization-test 10)

;;; Finalize Python
(py-finalize)
(display "Invocation successful!\n")
