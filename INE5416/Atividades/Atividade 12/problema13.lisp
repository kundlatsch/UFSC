#| Consideramos o vator começando em 1 |#
(defun elementoFalso (lista func)
	(if (null lista)
		0
		(if (funcall func (car lista))
			(+ 1 (elementoFalso (cdr lista) func))
			0
		)
	)
)

(defun apagar (lista n)
	(if (> n 1)
		(apagar (cdr lista) (- n 1))
		(cdr lista)
	)
)

(defun apagarEnquanto (lista func)
	(apagar lista (elementoFalso lista func))
)

(defun impar (n)
	(if (= (mod n 2) 0)
		NIL
		T
	)
)


(defun main()
    (write-line (write-to-string (apagarEnquanto '(1 3 4 3 4 5 1) #'impar)))
)

(main)
