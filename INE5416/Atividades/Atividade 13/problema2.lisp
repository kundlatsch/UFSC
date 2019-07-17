(defstruct no
    n
    esq
    dir
)

(setq minhaArvore
    (make-no 
        :n 52
        :esq (make-no :n 32               ;pode omitir o NIL 
                      :esq (make-no :n 12 :esq NIL :dir NIL)
                      :dir (make-no :n 32 :esq NIL :dir NIL)
             )
        :dir (make-no :n 56 
                      :esq (make-no :n 55 :esq NIL :dir NIL) 
                      :dir (make-no :n 64 :esq NIL :dir NIL)
             ) 
    )
)

(setq arvoreParesImpares
    (make-no 
        :n 4
        :esq (make-no :n 2               ;pode omitir o NIL 
                      :esq (make-no :n 1 :esq NIL :dir NIL)
                      :dir (make-no :n 2 :esq NIL :dir NIL)
             )
        :dir (make-no :n 2 
                      :esq (make-no :n 1 :esq NIL :dir NIL) 
                      :dir (make-no :n 5 :esq NIL :dir NIL)
             ) 
    )
)

(defun soma (arv)
    (if (null arv)
        0
        (+ 
            (no-n arv) 
            (soma (no-esq arv)) 
            (soma (no-dir arv))
        )
    )
)

(defun buscaElemento (arv x)
    (if (null arv)
        NIL
        (or 
            (= (no-n arv) x)
            (buscaElemento (no-esq arv) x) 
            (buscaElemento (no-dir arv) x)
        )
    )
)

(defun minimo (x y)
    (if (< x y)
        x
        y
    )
)

(setq INF 1000)

(defun minimoElemento (arv)
    (if (null arv)
        INF
        (minimo 
            (no-n arv) 
            (minimo 
                (minimoElemento (no-esq arv)) 
                (minimoElemento (no-dir arv))
            )
        )
    )
)

(defun incrementa (arv x)
    (if (not (null arv))
        (progn
            (setf (no-n arv) (+ (no-n arv) x))
            (incrementa (no-esq arv) x)
            (incrementa (no-dir arv) x)
        )
    )
)

(defun ocorrenciasElemento (arv x)
	(if (null arv)
		0
		(if (= (no-n arv) x)
			(+ 1 (ocorrenciasElemento (no-esq arv) x) (ocorrenciasElemento (no-dir arv) x))
			(+ (ocorrenciasElemento (no-esq arv) x) (ocorrenciasElemento (no-dir arv) x))
		)
	)
)

(defun maioresQueElemento (arv x)
	(if (null arv)
		0
		(if (> (no-n arv) x)
			(+ 1 (ocorrenciasElemento (no-esq arv) x) (ocorrenciasElemento (no-dir arv) x))
			(+ (ocorrenciasElemento (no-esq arv) x) (ocorrenciasElemento (no-dir arv) x))
		)
	)
)

(defun mediaElementos (arv)
	(/ (soma arv) (quantidade arv))
)

(defun quantidade (arv)
	(if (null arv)
		0
		(+ 1 (quantidade (no-esq arv)) (quantidade (no-dir arv)))
	)
)

(defun elementos (arv)
    (if (not (null arv))
        (cons (no-n arv) (append (elementos (no-esq arv)) (elementos (no-dir arv))))
    )
)

(defun substituir (arv x y)
	 (if (not (null arv))
        (progn
            (setf (no-n arv) (if (= (no-n arv) x) y (no-n arv)))
            (substituir (no-esq arv) x y)
            (substituir (no-dir arv) x y)
        )
    )
)

(defun posOrdem(arv)
    (if (not (null arv))
        (append (posOrder (no-esq arv)) (posOrder (no-dir arv)) (list (no-n arv)))
    )
)

(defun preOrdem(arv)
    (if (not (null arv))
        (append (list (no-n arv)) (preOrder (no-esq arv)) (preOrder (no-dir arv)))
    )
)

(defun emOrdem(arv)
    (if (not (null arv))
        (append (inOrder (no-esq arv)) (list (no-n arv)) (inOrder (no-dir arv)))
    )
)

(defun subtraiParesImpares (arv)
	(- (somaPares arv) (somaImpares arv))
)

(defun somaPares (arv)
	(if (null arv)
		0
		(if (= 0 (mod (no-n arv) 2))
			(+ (no-n arv) (somaPares (no-esq arv)) (somaPares (no-dir arv)))
			(+ (somaPares (no-esq arv)) (somaPares (no-dir arv)))
		)
	)
)

(defun somaImpares (arv)
	(if (null arv)
		0
		(if (/= 0 (mod (no-n arv) 2))
			(+ (no-n arv) (somaImpares (no-esq arv)) (somaImpares (no-dir arv)))
			(+ (somaImpares (no-esq arv)) (somaImpares (no-dir arv)))
		)
	)
)

(defun novoNo (y)
	(make-no 	:n y
				:esq NIL
				:dir NIL
		)
)

(defun main()
    (write-line (write-to-string (ocorrenciasElemento minhaArvore 32)))
    (write-line (write-to-string (maioresQueElemento minhaArvore 32)))
    (write-line (write-to-string (mediaElementos minhaArvore)))
    (write-line (write-to-string (quantidade minhaArvore)))
    (write-line (write-to-string (elementos minhaArvore)))
    (write-line (write-to-string (substituir minhaArvore 32 999)))
    (write-line (write-to-string minhaArvore))
    (write-line (write-to-string (subtraiParesImpares arvoreParesImpares)))
    (write-line (write-to-string (novoNo 4)))
)

(main)