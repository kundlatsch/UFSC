#| Consideramos o vator começando em 1 |#
(defun inverte (lista)
	(if (= (elementos lista) 1)
		(list (car lista))
		(concatenate 'list (inverte (cdr lista)) (list (car lista)))
	)
	
)

(defun elementos (lista)
	(if (null lista)
		0
		(+ 1 (elementos (cdr lista)))
	)
)



(defun main()
    (write-line (write-to-string (inverte '(4 2 0 3 4 5 1))))
)

(main)
