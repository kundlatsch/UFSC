(defun verificar(n1 n2 n3) (
	if (and (< (- n1 n2) n3) (< n3 (+ n1 n2)))
        T
    NIL
    
    )

)

(defun triangulo(n1 n2 n3) (
	if (and (verificar n1 n2 n3) (verificar n1 n3 n2) (verificar n2 n3 n1))
        T
    NIL
    
    )
)

(defun main()
    (setq n1 (read))
    (setq n2 (read))
    (setq n3 (read))
    (write-line (write-to-string (triangulo n1 n2 n3)))
)

(main)