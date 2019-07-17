(defun enesimo (lista n)
	(if (= n 0)
		NIL
		(if (= (- n 1) 0)
			(car lista)
			(enesimo (cdr lista) (- n 1))
		)
	) 
	
)

(defun main()
    (write-line (write-to-string (enesimo '(4 2 0 3 4 5 4) 3)))
)

(main)
