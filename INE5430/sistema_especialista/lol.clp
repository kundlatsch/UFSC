
;;;======================================================
;;;   League of Legends Expert System
;;;
;;;     Esse sistema especialista toma decisões para
;;;     League of Legends, um jogo de estratégia
;;;     multiplayer cinco contra cinco.
;;;
;;;     Sistema baseado no Automotive Expert System,
;;;     exemplo fornecido junto com o CLIPS versão 6.3.
;;;
;;;     To execute, merely load, reset and run.
;;;======================================================

;;****************
;;* DEFFUNCTIONS *
;;****************

(deffunction ask-question (?question $?allowed-values)
   (printout t ?question)
   (bind ?answer (read))
   (if (lexemep ?answer) 
       then (bind ?answer (lowcase ?answer)))
   (while (not (member$ ?answer ?allowed-values)) do
      (printout t ?question)
      (bind ?answer (read))
      (if (lexemep ?answer) 
          then (bind ?answer (lowcase ?answer))))
   ?answer)

(deffunction yes-or-no-p (?question)
   (bind ?response (ask-question ?question yes no y n))
   (if (or (eq ?response yes) (eq ?response y))
       then yes 
       else no))

;;;***************
;;;* QUERY RULES *
;;;***************

(defrule determine-lane-phase ""
   (not (lane-phase ?))
   (not (decision ?))
   =>
   (assert (lane-phase (yes-or-no-p "O jogo esta na lane phase (yes/no)? "))))
   
(defrule determine-dragon-alive-lp ""
   (lane-phase yes)
   (not (decision ?))
   =>
   (assert (dragon-alive-lp (yes-or-no-p "O dragao esta vivo (yes/no)? "))))

(defrule determine-herald-alive ""
   (dragon-alive-lp no)
   (not (decision ?))   
   =>
   (assert (herald-alive (yes-or-no-p "O aralto esta vivo (yes/no)? "))))

(defrule determine-baron-alive ""
   (lane-phase no)
   (not (decision ?))
   =>
   (assert (baron-alive (yes-or-no-p "O barao esta vivo (yes/no)? "))))

(defrule determine-dragon-alive-lg ""
   (lane-phase no)
   (baron-alive no)
   (not (decision ?))
   =>
   (assert (dragon-alive-lg (yes-or-no-p "O dragao esta vivo (yes/no)? "))))

(defrule determine-deep-vision ""
   (dragon-alive-lg no)
   (baron-alive no)
   (not (decision ?))
   =>
   (assert (deep-vision (yes-or-no-p "O seu time possui visao da selva adversaria (yes/no)? "))))

(defrule determine-pression ""
   (deep-vision no)
   (not (decision ?))
   =>
   (assert (lane-pression
      (ask-question "Voce possui pressao em alguma side lane (top/bot/no)? "
                    top bot no))))


;;;****************
;;;* REPAIR RULES *
;;;****************

(defrule dragon-alive ""
   (or (dragon-alive-lp yes) (dragon-alive-lg yes))
   (not (decision ?))
   =>
   (assert (decision "Fazer o dragao.")))

(defrule herald-alive ""
   (herald-alive yes)
   (not (decision ?))
   =>
   (assert (decision "Fazer o arauto.")))

(defrule baron-alive ""
   (baron-alive yes)
   (not (decision ?))
   =>
   (assert (decision "Fazer o barao.")))

(defrule keep-farming ""
   (or (herald-alive no) (lane-pression no))
   (not (decision ?))
   =>
   (assert (decision "Continuar farmando.")))

(defrule pick-off ""
   (deep-vision yes)
   (not (decision ?))
   =>
   (assert (decision "Invadir a selva adversaria.")))

(defrule top-pression ""
   (lane-pression top)
   (not (decision ?))
   =>
   (assert (decision "Domine a visao do barao.")))

(defrule bot-pression ""
   (lane-pression bot)
   (not (decision ?))
   =>
   (assert (decision "Domine a visao do dragao.")))

;;;********************************
;;;* STARTUP AND CONCLUSION RULES *
;;;********************************

(defrule system-banner ""
  (declare (salience 10))
  =>
  (printout t crlf crlf)
  (printout t "The League of Legends Expert System")
  (printout t crlf crlf))

(defrule print-repair ""
  (declare (salience 10))
  (decision ?item)
  =>
  (printout t crlf crlf)
  (printout t "Estrategia sugerida:")
  (printout t crlf crlf)
  (format t " %s%n%n%n" ?item))