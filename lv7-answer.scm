(define (sort l) 
(cond 
	((null? l) '())
	((= 1 (length l)) l)
	(else 
		(let ((l0 (car l)) (l1 (cdr l))) 
		(append (sort (filter (lambda (x) (<= x l0)) l1)) 
			(list l0) 
			(sort (filter (lambda (x) (> x l0)) l1)))))))

(displayln "input list:")
(define l (readline))
(print (sort l))