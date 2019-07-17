(defun fibonacci(n)
	(if (or (= n 1) (= n 2))
		1
		( + ( fibonacci (- n 1) ) ( fibonacci (- n 2) ) )
    
    )
)

(defun main()
    (setq x (read))
    (write-line (write-to-string (fibonacci x)))
)

(main)