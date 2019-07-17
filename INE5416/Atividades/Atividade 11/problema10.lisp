(defun area(x y z)
	(cond
		( ( and (> x y) (> x z) ) x)
		( ( > y z ) y)
		( T z)
	)
)

(defun main()
    (setq x (read))
    (setq y (read))
    (setq z (read))
    (write-line (write-to-string (area x y z)))
)

(main)


