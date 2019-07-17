(defun bashkara1(n1 n2 n3) 
	(/ (+ (- n2) (sqrt (- (* n2 n2) (* 4 n1 n3)))) (* 2 n1))
)

(defun bashkara2(n1 n2 n3) 
	(/ (+ (+ n2) (sqrt (- (* n2 n2) (* 4 n1 n3)))) (* 2 n1))
)

(defun main()
    (setq x (read))
    (setq y (read))
    (setq z (read))
    (write-line (write-to-string (bashkara1 x y z)))
    (write-line (write-to-string (bashkara2 x y z)))
)

(main)


