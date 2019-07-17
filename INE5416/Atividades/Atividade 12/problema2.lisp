(defun media (lista)
	(if (null lista)
		0
		(/ (soma lista) (elementos lista))
	)
)

(defun soma (lista)
	(if (null lista)
		0
		(+ (car lista) (soma (cdr lista)))
	)
)


(defun elementos (lista)
	(if (null lista)
		0
		(+ 1 (elementos (cdr lista)))
	)
)

(defun main()
    (write-line (write-to-string ( media '(3 10 90 14 8) )))
)

(main)