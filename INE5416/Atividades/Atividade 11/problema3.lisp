(defun area(x y) 
	(/ (* x y) 2)
)

(defun main()
    (setq x (read))
    (setq y (read))
    (write-line (write-to-string (area x y)))
)

(main)


