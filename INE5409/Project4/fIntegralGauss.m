function Gm = fIntegralGauss(m, a, b, f)
    t= [
    [0 0 0 0 0 0 0 0 0 0];
    [-1/sqrt(3) 1/sqrt(3) 0 0 0 0 0 0 0 0];
    [-sqrt(3/5) 0 sqrt(3/5) 0 0 0 0 0 0 0];
    [-0.8611363115940525752  -0.3399810435848562648   0.3399810435848562648 0.8611363115940525752 0 0 0 0 0 0];
    [-0.90617984593866399280 -0.53846931010568309104  0                     0.53846931010568309104 0.90617984593866399280 0 0 0 0 0];
    [-0.93246951420315202781 -0.66120938646626451366 -0.23861918608319690863 0.23861918608319690863 0.66120938646626451366  0.93246951420315202781 0 0 0 0];
    [-0.94910791234275852453 -0.74153118559939443986 -0.40584515137739716691 0 0.40584515137739716691 0.74153118559939443986 0.94910791234275852453 0 0 0];
    [-0.960289856497537 -0.796666477413630 -0.525532409916328 -0.183434642495650  0.183434642495650 0.525532409916329 0.796666477413627 0.960289856497536 0 0];
    [-0.968160239507625 -0.836031107326636 -0.613371432700588 -0.324253423403809  0                 0.324253423403809 0.613371432700591 0.836031107326639 0.968160239507622 0]; 
    [-0.973906528517169 -0.865063366688988 -0.679409568299025 -0.433395394129247 -0.148874338981631 0.148874338981631 0.433395394129247 0.679409568299024 0.865063366688989 0.973906528517168] ];
C= [
    [2 0 0 0 0 0 0 0 0 0];
    [1 1 0 0 0 0 0 0 0 0];
    [5/9                     8/9                   5/9                    0                      0 0 0 0 0 0];
    [0.34785484513745385737 0.65214515486254614263 0.65214515486254614263 0.34785484513745385737 0 0 0 0 0 0];
    [0.23692688505618908751 0.47862867049936646804 128/225                0.47862867049936646804 0.23692688505618908751 0 0 0 0 0];
    [0.17132449237917034504 0.36076157304813860757 0.46791393457269104739 0.46791393457269104739                    0.36076157304813860757                 0.17132449237917034504 0 0 0 0];
    [0.129484966168869693271 0.27970539148927666790 0.38183005050511894495 512/1225 0.38183005050511894495 0.27970539148927666790 0.129484966168869693271 0 0 0];
    [0.101228536290375  0.222381034453373 0.313706645877895 0.362683783378353 0.362683783378369 0.313706645877885 0.222381034453374 0.101228536290377 0 0];
    [0.0812743883615794 0.180648160694844 0.260610696402962 0.312347077039962 0.330239355001305 0.312347077039964 0.260610696402964 0.180648160694839 0.0812743883615805 0];
    [0.0666713443086935 0.149451349150562 0.219086362516008 0.269266719309973 0.295524224714767 0.295524224714756 0.269266719309977 0.219086362516008 0.149451349150562 0.0666713443086937] ];
    
    x =  0.5.*((b-a).*t(m, 1:m) .+ (b+a));
    y = f(x);
    
    Gm = 0.5*(b-a) * sum(C(m, 1:m).*y(1:m));
    % Gm = 0.5*(b-a) * (C(1) * y(1) + C(2) * y(2));


end	 	  	 	      	      	 	     	    	     	      	 	
