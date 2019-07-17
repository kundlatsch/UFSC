(defun main()
    (setq x T)
    (setq y NIL)
    (write-line (write-to-string ((lambda (x y) (or (and x (not y)) (and (not x) y))) x y) ))
)

(main)