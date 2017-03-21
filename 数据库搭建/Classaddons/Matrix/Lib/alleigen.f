C
C  $Id: alleigen.f,v 1.4 1993/06/10 13:59:27 tiller Exp $
C  ChangeLog:
C 	$Log: alleigen.f,v $
c Revision 1.4  1993/06/10  13:59:27  tiller
c Everything seems to be working under gcc-2.3.3
c
c Revision 1.4  1993/06/10  13:59:27  tiller
c Everything seems to be working under gcc-2.3.3
c
c Revision 1.3  1993/05/11  15:42:00  tiller
c Added support for memory leak traking...Everything looks good for this
c versions.
c
c Revision 1.3  1993/05/11  15:42:00  tiller
c Added support for memory leak traking...Everything looks good for this
c versions.
c
c Revision 1.2  1993/02/20  19:03:12  tiller
c Added RCS header information
c
c Revision 1.2  1993/02/20  19:03:12  tiller
c Added RCS header information
c
C

	SUBROUTINE alleig(A,eig,Q,beta,T,tol,N,jmax)

C	This subroutine computes all the eigenvalues
C	and eigenvectors of a FULL MATRIX
C
C
C	A		= NXN FULL MATRIX
C	EIG		= N Eigenvalues of A
C	Q		= N Eigenvectors of A
C	BETA	= Scratch
C	T		= Scratch
C	TOL		= Tolerance
C	JMAX	= Max Iterations
C
	implicit double precision (A-H,O-Z)
	dimension A(N,N),eig(N),Q(N,N),beta(N,N),T(N)
C
	call szero(Q,N*N)
	do 100 j = 1,N
		Q(J,J) = 1.0
  100	continue
	call tridia(A,eig,beta,Q,T,N)
	call qltrid(eig,beta,Q,tol,N,jmax)
	return
	end
C
C
	double precision FUNCTION dot(A,B,N)
C
	double precision A(N),B(N),C
C	double precision dot
C
	C = 0.0
	do 100 J=1,N
	    C = C + A(J)*B(J)
  100	continue
	dot=C
C
	return
	end
C
C
	SUBROUTINE tridia(A,alfa,beta,Q,T,N)
C
C	This subroutine tridiagonalizes a FULL MATRIX
C
	implicit double precision (A-H,O-Z)
	dimension A(N,N),alfa(N),beta(N),Q(N,N),T(N)
C
	do 500 J=1,N-2
	    bnn = dot(A(J+1,J),A(J+1,J),N-J)
	    bn = dsqrt(bnn)
	    alfa(J) = A(J,J)
	    beta(J) = dsign(bn,A(J+1,J))
	    gama = 1.0/(bnn+beta(J)*A(J+1,J))
	    A(J+1,J) = A(J+1,J)+beta(J)
	    beta(J) = -beta(J)
	    do 200 JJ = J+1,N
		T(JJ) = 0.0
		do 160 KK = J+1,JJ
		    T(JJ) = T(JJ)+A(JJ,KK)*A(KK,J)
  160		continue
		do 180 KK = JJ+1,N
		    T(JJ) = T(JJ) + A(KK,JJ)*A(KK,J)
  180		continue
		T(JJ) = T(JJ)*gama
  200	    continue
	    utq = dot(A(J+1,J),T(J+1),N-J)
	    CC = .5*utq*gama
	    do 220 JJ = J+1,N
		T(JJ) = T(JJ)-CC*A(JJ,J)
  220	    continue
	    do 260 JJ = J+1,N
		do 240 KK = J+1,JJ
		    A(JJ,KK) = A(JJ,KK)-A(JJ,J)*T(KK)-T(JJ)*A(KK,J)
  240		continue
  260	    continue
	    do 300 JJ=1,J
		T(JJ) = 0.0
		do 280 KK = J+1,N
		    T(JJ) = T(JJ)+Q(JJ,KK)*A(KK,J)
  280		continue
  300 	    continue
	    do 340 JJ = 1,J
		do 320 KK = J+1,N
		    Q(JJ,KK) = Q(JJ,KK)-gama*T(JJ)*A(KK,J)
  320		continue
  340	    continue
	    do 380 JJ=J+1,N
		T(JJ) = 0.0
		do 360 KK=J+1,N
		    T(JJ) = T(JJ)+Q(JJ,KK)*A(KK,J)
  360		continue
  380	    continue
	    do 420 JJ = J+1,N
		do 400 KK = J+1,N
		    Q(JJ,KK) = Q(JJ,KK)-gama*T(JJ)*A(KK,J)
  400		continue
  420	    continue
  500	continue
	alfa(N-1) = A(N-1,N-1)
	beta(N-1) = A(N,N-1)
	alfa(N) = A(N,N)
	beta(N) = 0.0
	return
	end
C
C
C
	SUBROUTINE qltrid(alfa,beta,Q,tol,N,jmax)
C
C	    This subroutine determines all the eigenvalues and
C	eigenvectors of a tridiagonal matrix
C
	implicit double precision (A-H,O-Z)
	dimension alfa(N),beta(N),Q(N,N)

	do 100 J = 1,N-1
	    A = beta(J)
	    beta(J) = dsign(A*A,A)
  100	continue
C
	do 500 J=1,N-1
	    jcount = 0
  150	    continue
	    jcount = jcount+1
	    if (jcount.gt.jmax) then
			call ae_error()
		return
	    endif
	    const = tol*tol*(alfa(J)+alfa(J+1))
	    if (dabs(beta(J)).LT.const) go to 400
	    A = .5*(alfa(J+1)-alfa(J))
	    B = dabs(beta(J))
	    C = B/(dabs(A)+dsqrt(a*a+b))
	    sigma = alfa(J)-dsign(c,a)
	    K = J
	    call qlstep(alfa,beta,Q,sigma,K,tol,N)
	    go to 150
  400	    continue
  500	continue
C
	return
C2001	format(//,5X,'Maximum Number of Iterations Exceeded at ',I5)
	end
C
	SUBROUTINE qlstep(T1,T2,Q,sigma,K,tol,N)
C
C	    This subroutine performs one step of QL
C	transformation through plane rotations
C
	implicit double precision (A-H,O-Z)
	dimension T1(N),T2(N),Q(N,N)
C
	do 100 J=N,K+1,-1
	    const = tol*tol*(T1(J)+T1(J-1))
	    if (dabs(T2(J-1)).GT.const) then
		M = J
		go to 120
	    endif
  100	continue
	return
  120	continue
	cs1 = 1.0
	cs2 = 1.0
	sn1 = 0.0
	sn2 = 0.0
	gamma = T1(M)-sigma
	P = gamma
	PP = P*P
	do 300 J=M-1,K,-1
	    B2 = dabs(T2(J))
	    R2 = PP+B2
	    T2(J+1) = dsign(sn2*R2,SN1)
	    pcs2 = cs2
	    cs2 = PP/R2
	    sn2 = B2/R2
	    sn1 = dsign(dsqrt(sn2),T2(J))
	    pcs1 = cs1
	    cs1 = dsign(dsqrt(CS2),P)
	    pgam = gamma
	    alfa = T1(J)
	    gamma = cs2*(alfa-sigma)-sn2*pgam
	    t1(J+1) = alfa+pgam-gamma
	    if (cs2 .lt. tol) then
		PP = B2*pcs2
		B = dsign(dsqrt(B2),T2(J))
		P = -B*pcs1
	    else
		PP = gamma*gamma/cs2
		P = gamma/cs1
	    endif
C
	    do 200 I=1,N
		Q1 = Q(I,J)
		Q2 = Q(I,J+1)
		Q(I,J) = cs1*Q1-sn1*Q2
		Q(I,J+1) = sn1*Q1+cs1*Q2
  200	    continue
  300	continue
	T1(K) = gamma+sigma
	T2(K) = dsign(sn2*PP,sn1*P)
C
	return
	end
C
C
C
	SUBROUTINE szero(A,N)
C
	implicit double precision (A-H,O-Z)
	dimension A(N)
C
	do 100 J=1,N
	    A(J) = 0.0
  100	continue
	return
	end
C
C
