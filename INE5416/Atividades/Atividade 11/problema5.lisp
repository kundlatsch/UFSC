(defun media(x y z) (
	if (>= (/ (+ x y z) 3) 6)
        T
    NIL
    
    )
)

(defun main()
    (setq x (read))
    (setq y (read))
    (setq z (read))
    (write-line (write-to-string (media x y z)))
)

(main)