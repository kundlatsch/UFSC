(defun filtrar(func a)
    (if (not (null a))
        (if (funcall func (car a))
            (append (list (car a)) (filtrar func (cdr a)))
            (filtrar func (cdr a))
        )
    )
)

(defun main ()
	(write-line(write-to-string(filtrar  '(4 2 0 3 4 5 1) (function (lambda (x) (= 1 (mod x 2)))))))
)

(main)