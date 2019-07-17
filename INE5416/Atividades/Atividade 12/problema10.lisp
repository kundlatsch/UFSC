#| Consideramos o vator começando em 1 |#
(defun mapeia (lista func)
	(if (= (elementos lista) 1)
		(list (funcall func (car lista)))
		(concatenate 'list (list (funcall func (car lista))) (mapeia (cdr lista) func))
	)
	
)

(defun elementos (lista)
	(if (null lista)
		0
		(+ 1 (elementos (cdr lista)))
	)
)



(defun main()
    (write-line (write-to-string (mapeia '(4 2 0 3 4 5 1) #'sqrt)))
)

(main)
