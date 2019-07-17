;lcm(a,b,c) = lcm(a,lcm(b,c))

(defun greatest-common-divisor (a b)
    (if (zerop b)
        a
        (greatest-common-divisor b (mod a b))
    )
)

(defun main()
    (setq x (read))
    (setq y (read))
    (setq z (read))
    (write-line (write-to-string (greatest-common-divisor x (greatest-common-divisor y z))))
)

(main)