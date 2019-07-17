(defun main()
    (setq x 1)
    (setq y 10)
    (setq z 7)
    (write-line (write-to-string((lambda (x y z) (>= (/ (+ x y z) 3) 6)) x y z)))
)

(main)