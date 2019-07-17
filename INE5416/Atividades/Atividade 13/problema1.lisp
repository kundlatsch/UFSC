(defstruct ponto
	x
	y
)

(setq p1
	(make-ponto
		:x 1
		:y 1
	)
)

(setq p2
	(make-ponto
		:x 3
		:y 1
	)
)

(setq p3
	(make-ponto
		:x 10
		:y 3
	)
)

(defun distancia (p1 p2)
	(sqrt (+ (expt (- (ponto-x p2)(ponto-x p1)) 2) (expt (- (ponto-y p2)(ponto-y p1)) 2) ))
)

(defun colineares (p1 p2 p3)
	(= (determinante p1 p2 p3) 0)
)

; FUNÇÃO AUXILIAR PARA VERIFICAR COLINEARIDADE
(defun determinante (a b c)
	(+ (* (ponto-x a) (ponto-y b)) (* (ponto-y a) (ponto-x c)) (* (ponto-x b) (ponto-y c)) (- (* (ponto-x c) (ponto-y b))) (- (* (ponto-y c) (ponto-x a))) (- (* (ponto-x b) (ponto-y a))) )
)

(defun formaTriangulo (p1 p2 p3)
	(and (verifica p1 p2 p3) (verifica p2 p1 p3) (verifica p3 p1 p2))
)

(defun verifica (a b c)
	(condicaoDistancias (distancia a b) (distancia b c) (distancia c a))
)

(defun condicaoDistancias (a b c)
	(and (< (abs (- b c)) a) (< a (+ b c)))
)

(defun main()
	(write-line (write-to-string (distancia p1 p2)))
	(write-line (write-to-string (colineares p1 p2 p3)))
	(write-line (write-to-string (formaTriangulo p1 p2 p3)))
)

(main)