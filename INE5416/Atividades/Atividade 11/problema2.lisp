(defun abso(x)
    (if (< x 0)
        (- x)
        (x)
    )
)

(defun main()
    (setq x (read))
    (write-line (write-to-string (abso x)))
)

(main)


