(defun main ()
    (setq x1 1)
    (setq y1 2)
    (setq z1 2)

    (setq x2 3)
    (setq y2 4)
    (setq z2 5)

    (write-line (write-to-string((lambda (x1 y1 z1 x2 y2 z2) (sqrt (+ (expt (- x2 x1) 2) (expt (- y2 y1) 2) (expt (- z2 z1) 2)))) x1 y1 z1 x2 y2 z2 )))
)

(main)