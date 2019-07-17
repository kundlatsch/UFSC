(defun mapeia (lista func)
	(if (= (elementos lista) 1)
		(list (funcall func (car lista)))
		(concatenate 'list (list (funcall func (car lista))) (mapeia (cdr lista) func))
	)
	
)

(defun main ()
    (write-line (write-to-string (mapeia '(4 2 0 3 4 5 1) (function (lambda (x) (= 0 (mod x 2)))))))
)

(main)