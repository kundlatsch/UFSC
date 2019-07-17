(defun coprimos (x y)
	(if (= (lcm x y) 1)
        T
    NIL
    )
)



(defun main()
    (setq x (read))
    (setq y (read))
    (write-line (write-to-string (coprimos x y)))
)

(main)