(defun main ()
    (setq x 6)
    (setq y 5)
    (setq z 10)

    (write-line (write-to-string((lambda (x y z) (cond ((and (> x y) (> x z)) x) ((and (> y x) (> y z)) y) (t z))) x y z)))
)

(main)