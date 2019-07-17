;+, -, *, /
(defun seleciona (operacao x y)
	(cond
		((eq operacao '+) (+ x y))
		((eq operacao '-) (- x y))
		((eq operacao '*) (* x y))
		((eq operacao '/) (/ x y))
		(t "Outro")
	)
)

(defun main ()
	(setq operacao (read))
	(setq x (read))
	(setq y (read))
	(write-line (write-to-string (seleciona operacao x y)))
)

(main)