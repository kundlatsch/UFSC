(defun abso(x)
    (if (< x 0)
        (- x)
        (x)
    )
)

(defun greatest-common-divisor (a b)
    (if (zerop b)
        a
        (greatest-common-divisor b (mod a b))
    )
)

(defun mmc (x y)
	(lcm x y)
)



(defun main()
    (setq x (read))
    (setq y (read))
    (write-line (write-to-string (lcm x y)))
)

(main)