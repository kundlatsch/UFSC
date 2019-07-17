(defun apagar (lista n)
	(if (> n 1)
		(apagar (cdr lista) (- n 1))
		(cdr lista)
	)
	
)

(defun main()
    (write-line (write-to-string (apagar '(4 2 0 3 4 5 4) 3)))
)

(main)