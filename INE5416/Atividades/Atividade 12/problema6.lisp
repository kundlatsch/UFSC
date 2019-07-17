(defun ocorrencias (lista item)
	(if (null lista)
		0
		(if (= (car lista) item)
			(+ (ocorrencias (cdr lista) item) 1)
			(+ (ocorrencias (cdr lista) item) 0)
		)
	)
)

(defun main()
    (write-line (write-to-string (ocorrencias '(4 2 0 3 4 5 4) 4)))
)

(main)