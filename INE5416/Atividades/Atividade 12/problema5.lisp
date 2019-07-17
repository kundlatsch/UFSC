(defun busca (lista item)
	(if (null lista)
		NIL
		(if (= (car lista) item)
			T
			(busca (cdr lista) item)
		)
	)
)

(defun main()
    (write-line (write-to-string (busca '(1 2 1 4 5) 3)))
)

(main)