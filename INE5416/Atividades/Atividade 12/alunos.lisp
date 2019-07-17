(defun alunos()
    '((1 "Bob" (5.6 8 9.3))
      (2 "Ana" (8.2 9 7 6.5))
      (3 "Tom" (3.2 7 5.7 8.3))
      (4 "Bin" (7.5 5.3 8.5 6.2 3.1))
      (5 "Bia" (6.7 4.1 5.5)))
)

(defun getNome (aluno)
    (car (cdr aluno))
)

(defun getNomes (lista)
    (if (null lista)
        ()
        (cons (getNome (car lista)) (getNomes (cdr lista)))
    )
)

(defun main ()
    (write-line (write-to-string (getNomes (alunos))))
)

(main)