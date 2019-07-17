(defun diferencaMaiorMenor (lista)
	(if (null lista)
		0
		(- (maior lista) (menor lista))
	)
)

(defun menor (lista)
	(if (= (elementos lista) 1)
		(car lista)
		(if (< (car lista) (car (cdr lista))) 
			(menor (cons (car lista) (cdr (cdr lista))))
			(menor (cdr lista))
		)
	)
)

(defun maior (lista)
	(if (= (elementos lista) 1)
		(car lista)
		(if (> (car lista) (car (cdr lista))) 
			(maior (cons (car lista) (cdr (cdr lista))))
			(maior (cdr lista))
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
    (write-line (write-to-string (diferencaMaiorMenor '(300 104 98 129 299) )))
)

(main)