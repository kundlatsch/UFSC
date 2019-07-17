(defun primeiros (lista n)
	(if (= n 1)
		(list (car lista))
		(cons (car lista) (primeiros (cdr lista) (- n 1)))
	)
	
)

(defun main()
    (write-line (write-to-string (primeiros '(4 2 0 3 4 5 4) 3)))
)

(main)