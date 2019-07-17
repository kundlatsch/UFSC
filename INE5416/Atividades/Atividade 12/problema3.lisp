(defun menor (lista)
	(if (= (elementos lista) 1)
		(car lista)
		(if (< (car lista) (car (cdr lista))) 
			(menor (cons (car lista) (cdr (cdr lista))))
			(menor (cdr lista))
		)
	)
)

(defun elementos (lista)
	(if (null lista)
		0
		(+ 1 (elementos (cdr lista)))
	)
)

(defun main()
    (write-line (write-to-string (menor '(3 10 30 1 299) )))
)

(main)