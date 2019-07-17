#| Consideramos o vator começando em 1 |#
(defun fatia (lista n m)
	(if (= (- n (- m 2)) 0)
		(list (car lista))
		(if (= n 1)
			(cons (car lista) (fatia (cdr lista) n (- m 1)))
			(fatia (cdr lista) (- n 1) m)
		)
	)
	
)

(defun main()
    (write-line (write-to-string (fatia '(4 2 0 3 4 5 4) 2 5)))
)

(main)
