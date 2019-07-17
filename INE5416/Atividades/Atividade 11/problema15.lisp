(defun coprimos (x y)
	(if (= (lcm x y) 1)
        T
    NIL
    )
)

(defun totiente (n i counter)
	(if (= i n)
		counter
    (if (coprimos n i)
    	(totiente n (+ i 1) (+ counter 1))
    	
    (totiente n (+ i 1) counter)
    	)
    )
)



(defun main()
    (setq x (read))
    (write-line (write-to-string (totiente x 1 0)))
)

(main)