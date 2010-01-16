      SUBROUTINE HEPNAM(ID,CHAU)

C...Purpose: to give the particle/parton name as a character string.

      CHARACTER CHAU*20
      CHARACTER CHSP(1600)*15

      DATA (CHSP(I), I=1,109) /'d','u','s','c','b','t','l','h',2*' ',
     1'e','nu_e','mu','nu_mu','tau','nu_tau','chi','nu_chi',2*' ',
     2'g','gamma','Z','W','H_1',' ','eta_tech','LQ','R','Z_2',
     3'W_2','H_2','H_2','H_2','Z_3','H_3','H_3','H_3','H_4','H_5',
     4'L susy d','L susy u','L susy s','L susy c',
     5'L susy b','L susy t','gluino','photino',2*' ',
     6'L susy e','L susy nu_e','L susy mu','L susy nu_mu',
     7'L susy tau','L susy nu_tau','susy HL','susy HH','susy HA',
     8' ','R susy d','R susy u','R susy s','R susy c',
     9'R susy b','R susy t','zino','susy Z2','susy Z3',
     *'susy Z4','R susy e','R susy nu_e','R susy mu',
     1'R susy nu_mu','R susy tau','R susy nu_tau','wino',
     2'susy W2',2*' ',19*'gen. code',' ','KS','KL','diquark',
     3 2*'pi_diffr','n_diffr','p_diffr','l-baryon','h-baryon'/
      DATA (CHSP(I), I=110,209) /'pi','a_0(980)','pi(1300)',7*' ',
     1'rho(770)','b_1(1235)','a_1(1260)','rho(1450)','rho(1700)',5*' ',
     2'a_2(1320)','pi_2(1670)',8*' ','rho_3(1690)',19*' ','eta',
     3'f_0(975)','eta(1295)','f_0(1400)','eta(1440)','f_0(1590)',
     4'f_0(1710)',3*' ','omega(783)','h_1(1170)','f_1(1285)',
     5'omega(1390)','f_1(1420)','f_1(1510)','omega(1600)',3*' ',
     6'f_2(1270)',' ','f_2(2010)','f_2(2300)','f_2(2340)',5*' ',
     7'omega_3(1670)',9*' ','f_4(2050)',9*' '/
      DATA (CHSP(I), I=210,359) /'eta''(958)','f''0',8*' ','phi(1020)',
     1 'h_1(1380)','f_1(1420)','phi(1680)','f''_1',5*' ','f''_2(1525)',
     2 9*' ','phi_3(1850)',19*' ','eta_c(1S)','chi_c0(1P)','eta_c(2S)',
     3 7*' ','J/PSI','hc(1P)','chi_c1(1P)','PSI(2S)','PSI(3770)',
     4 'PSI(4040)','PSI(4160)','PSI(4415)',2*' ','chi_c2(1P)',29*' ',
     5 'eta_b(1S)','chi_b0(1P)','eta_b(2S)','chi_b0(2P)','eta_b(3S)',
     6 5*' ','upsilon(1S)','h_b(1P)','chi_b1(1P)','upsilon(2S)',
     7 'h_b(2P)','chi_b1(2P)','upsilon(3S)','upsilon(4S)',
     8 'upsilon(10860)','upsilon(11020)','chi_b2(1P)','chi_b2(2P)',
     9 28*' '/
      DATA (CHSP(I), I=360,409) /'eta_t','chi_t0',8*' ','theta',
     1'h_1t','chi_t1',7*' ','chi_t2',29*' '/
      DATA (CHSP(I), I=410,439) /'eta_l','chi_l0',' ',
     2'theta_l','h_1l','chi_l1','chi_l2',8*' ','eta_h',
     3'chi_h0',' ','theta_h','h_1h','chi_h1','chi_h2',8*' '/
      DATA (CHSP(I), I=440,489) /'pi','a_0(980)','pi(1300)',7*' ',
     1'rho(770)','b_1(1235)','a_1(1260)','rho(1450)','rho(1700)',5*' ',
     2'a_2(1320)','pi_2(1670)',8*' ','rho_3(1690)',19*' '/
      DATA (CHSP(I), I=490,589) /'K','K*_0(1430)',8*' ','K*(892)',
     1'K_1(1270)','K_1(1400)','K*(1410)','K*(1680)',5*' ','K*_2(1430)',
     2'K_2(1770)',8*' ','K*_3(1780)',9*' ','K*_4(2045)',9*' ',
     3'K','K*_0(1430)',8*' ','K*(892)','K_1(1270)','K_1(1400)',
     4'K*(1410)','K*(1680)',5*' ','K*_2(1430)','K_2(1770)',8*' ',
     5'K*_3(1780)',9*' ','K*_4(2045)',9*' '/
      DATA (CHSP(I), I=590,739) /'D','D*_0',8*' ','D*(2010)',
     1'D_1(2420)','D*_1',7*' ','D*_2(2460)',29*' ','D','D*_0',8*' ',
     2'D*(2010)','D_1(2420)','D*_1',7*' ','D*_2(2460)',29*' ','D_s',
     3'D*_s0',8*' ','D*_s','D_s1(2536)','D*_s1',7*' ','D*_s2',29*' '/
      DATA (CHSP(I), I=740,939) /'B','B*_0',8*' ','B*',
     1'B_1','B*_1',7*' ','B*_2',29*' ','B','B*_0',8*' ',
     2'B*','B_1','B*_1',7*' ','B*_2',29*' ','B_s',
     3'B*_s0',8*' ','B*_s','B_s1','B*_s1',7*' ','B*_s2',29*' ','B_c',
     3'B*_c0',8*' ','B*_c','B_c1','B*_c1',7*' ','B*_c2',29*' '/
      DATA (CHSP(I), I=940,1189) /'T','T*_0',8*' ','T*',
     1'T_1','T*_1',7*' ','T*_2',29*' ','T','T*_0',8*' ',
     2'T*','T_1','T*_1',7*' ','T*_2',29*' ','T_s',
     3'T*_s0',8*' ','T*_s','T_s1','T*_s1',7*' ','T*_s2',29*' ','T_c',
     3'T*_c0',8*' ','T*_c','T_c1','T*_c1',7*' ','T*_c2',29*' ','T_b',
     3'T*_b0',8*' ','T*_b','T_b1','T*_b1',7*' ','T*_b2',29*' '/
      DATA (CHSP(I), I=1190,1279) /'L','L*_0',' ','L*','L_1','L*_1',
     1'L*_2',8*' ','L','L*_0',' ','L*','L_1','L*_1','L*_2',8*' ',
     2'L_s','L*_s0',' ','L*_s','L_s1','L*_s1','L*_s2',8*' ',
     3'L_c','L*_c0',' ','L*_c','L_c1','L*_c1','L*_c2',8*' ',
     4'L_b','L*_b0',' ','L*_b','L_b1','L*_b1','L*_b2',8*' ',
     5'L_t','L*_t0',' ','L*_t','L_t1','L*_t1','L*_t2',8*' '/
      DATA (CHSP(I), I=1280,1384) /'H','H*_0',' ','H*','H_1','H*_1',
     1'H*_2',8*' ','H','H*_0',' ','H*','H_1','H*_1','H*_2',8*' ',
     2'H_s','H*_s0',' ','H*_s','H_s1','H*_s1','H*_s2',8*' ',
     3'H_c','H*_c0',' ','H*_c','H_c1','H*_c1','H*_c2',8*' ',
     4'H_b','H*_b0',' ','H*_b','H_b1','H*_b1','H*_b2',8*' ',
     5'H_t','H*_t0',' ','H*_t','H_t1','H*_t1','H*_t2',8*' ',
     6'H_l','H*_l0',' ','H*_l','H_l1','H*_l1','H*_l2',8*' '/
      DATA (CHSP(I), I=1385,1425) /' ','Lambda','Lambda_c',
     1'Xi_c''','Xi_c''','Lambda_b','Xi_b''','Xi_b''','Xi_bc''',
     2'Xi_bc''','Omega_bc''','Lambda_t','Xi_t''','Xi_t''',
     3'Xi_tc''','Xi_tc''','Omega_tc''','Xi_tb''','Xi_tb''',
     4'Omega_tb''','Omega_tbc''','Lambda*','Lambda_c*',
     1'Xi_c*''','Xi_c*''','Lambda_b*','Xi_b*''','Xi_b*''',
     2'Xi_bc*''','Xi_bc*''','Omega_bc*''','Lambda_t*',
     3'Xi_t*''','Xi_t*''','Xi_tc*''','Xi_tc*''','Omega_tc*''',
     4'Xi_tb*''','Xi_tb*''','Omega_tb*''','Omega_tbc*'''/
      DATA (CHSP(I), I=1426,1485) /' ','n','p',' ','Sigma','Sigma',
     1'Sigma','Xi','Xi',' ','Sigma_c','Sigma_c','Sigma_c','Xi_c','Xi_c',
     2'Omega_c','Xi_cc','Xi_cc','Omega_cc',' ','Sigma_b','Sigma_b',
     3'Sigma_b','Xi_b','Xi_b','Omega_b','Xi_bc','Xi_bc','Omega_bc',
     4'Omega_bcc','Xi_bb','Xi_bb','Omega_bb','Omega_bbc',' ',
     5'Sigma_t','Sigma_t','Sigma_t','Xi_t','Xi_t','Omega_t','Xi_tc',
     6'Xi_tc','Omega_tc','Omega_tcc','Xi_tb','Xi_tb','Omega_tb',
     7'Omega_tbc','Omega_tbb','Xi_tt','Xi_tt','Omega_tt','Omega_ttc',
     8'Omega_ttb',5*' '/
      DATA (CHSP(I), I=1486,1545) /'Delta','Delta','Delta','Delta',
     1'Sigma*','Sigma*','Sigma*','Xi*','Xi*','Omega',
     2'Sigma*_c','Sigma*_c','Sigma*_c','Xi*_c','Xi*_c','Omega*_c',
     3'Xi*_cc','Xi*_cc','Omega*_cc','Omega*_ccc',
     4'Sigma*_b','Sigma*_b','Sigma*_b','Xi*_b','Xi*_b','Omega*_b',
     5'Xi*_bc','Xi*_bc','Omega*_bc','Omega*_bcc',
     6'Xi*_bb','Xi*_bb','Omega*_bb','Omega*_bbc','Omega*_bbb',
     7'Sigma*_t','Sigma*_t','Sigma*_t','Xi*_t','Xi*_t','Omega*_t',
     8'Xi*_tc','Xi*_tc','Omega*_tc','Omega*_tcc','Xi*_tb','Xi*_tb',
     9'Omega*_tb','Omega*_tbc','Omega*_tbb','Xi*_tt','Xi*_tt',
     *'Omega*_tt','Omega*_ttc','Omega*_ttb','Omega*_ttt',4*' '/

      SAVE CHSP


C...Initial values. Charge (in units of 1/3). Subdivide code.
      CHAU=' '
      IDA=IABS(ID)
      IF(IDA.EQ.0) RETURN

      IDCM=HEPCMP(ID)
      IF(IDCM.LE.0) RETURN

      LEN=0
      KQ=HEPCHG(ID)
      IDQ3=MOD(IDA/1000,10)
      IDQ2=MOD(IDA/100,10)
      IDQ1=MOD(IDA/10,10)
      IDJS=MOD(IDA,10)
      IDEX=MOD(IDA/10000,10)

C...Read out root name and spin for simple particles and special cases
      IF(IDA.LE.100) THEN
        CHAU=CHSP(IDCM)
        LEN=0
        DO 100 LEM=1,20
  100     IF(CHAU(LEM:LEM).NE.' ') LEN=LEM
      ELSEIF(IDJS.EQ.0)THEN
        CHAU=CHSP(IDCM)
        LEN=0
        DO 150 LEM=1,20
  150     IF(CHAU(LEM:LEM).NE.' ') LEN=LEM

C...Construct root name for diquark. Add on spin.
      ELSEIF(IDQ1.EQ.0) THEN
        CHAU(1:2)=CHSP(IDQ3)(1:1)//CHSP(IDQ2)(1:1)
        IF(IDJS.EQ.1) CHAU(3:4)='_0'
        IF(IDJS.EQ.3) CHAU(3:4)='_1'
        LEN=4

C...Construct root name for meson.
      ELSEIF(IDQ3.EQ.0) THEN
        CHAU=CHSP(IDCM)
        LEN=0
        DO 200 LEM=1,20
  200     IF(CHAU(LEM:LEM).NE.' ') LEN=LEM

C...Construct root name and spin for heavy baryon.
      ELSE
        IF(IDQ3.LE.6) THEN
          CHAU=CHSP(IDCM)
          LEN=0
          DO 300 LEM=1,20
  300       IF(CHAU(LEM:LEM).NE.' ') LEN=LEM
        ELSE
          LEN=0
          IF(IDQ2.LE.2 .AND. IDQ1.LE.2)THEN
            CHAU='Sigma'
            IF(IDJS.EQ.4) CHAU='Sigma*'
            IF(IDQ1.GT.IDQ2) CHAU='Lambda'
          ELSEIF(IDQ1.LE.2) THEN
            CHAU='Xi'
            IF(IDJS.EQ.4) CHAU='Xi*'
          ELSEIF(IDQ2.LE.2) THEN
            CHAU='Xi'''
            IF(IDJS.EQ.4) CHAU='Xi*'''
          ELSEIF(IDQ1.GT.IDQ2) THEN
            CHAU='Omega'''
            IF(IDJS.EQ.4) CHAU='Omega*'''
          ELSE
            CHAU='Omega'
            IF(IDJS.EQ.4) CHAU='Omega*'
          ENDIF
          DO 320 LEM=1,20
  320       IF(CHAU(LEM:LEM).NE.' ') LEN=LEM

C...Add on heavy flavour content for heavy baryon.
          CHAU(LEN+1:LEN+2)='_'//CHSP(IDQ3)(1:1)
          LEN=LEN+2
          IF(IDQ2.GE.IDQ1.AND.IDQ1.GE.4) THEN
            CHAU(LEN+1:LEN+2)=CHSP(IDQ2)(1:1)//CHSP(IDQ1)(1:1)
            LEN=LEN+2
          ELSEIF(IDQ2.GE.IDQ1.AND.IDQ2.GE.4) THEN
            CHAU(LEN+1:LEN+1)=CHSP(IDQ2)(1:1)
            LEN=LEN+1
          ELSEIF(IDQ1.GT.IDQ2.AND.IDQ2.GE.4) THEN
            CHAU(LEN+1:LEN+2)=CHSP(IDQ1)(1:1)//CHSP(IDQ2)(1:1)
            LEN=LEN+2
          ELSEIF(IDQ1.GT.IDQ2.AND.IDQ1.GE.4) THEN
            CHAU(LEN+1:LEN+1)=CHSP(IDQ1)(1:1)
            LEN=LEN+1
          ENDIF
        ENDIF
      ENDIF

C...Add on bar sign for antiparticle (where necessary).
      IF(ID.GT.0.OR.LEN.EQ.0) THEN
      ELSEIF(IDA.GT.10.AND.IDA.LE.40.AND.KQ.NE.0) THEN
      ELSEIF(IDA.GT.46.AND.IDA.LE.60.AND.KQ.NE.0) THEN
      ELSEIF(IDA.GT.66.AND.IDA.LE.80.AND.KQ.NE.0) THEN
      ELSEIF(IDA.GE.81.AND.IDA.LE.99) THEN
      ELSEIF(IDA.GT.100.AND.IDQ3.EQ.0.AND.KQ.NE.0) THEN
      ELSE
        CHAU(LEN+1:LEN+1)='~'
        LEN=LEN+1
      ENDIF

C...Add on charge where applicable (conventional cases skipped).
      IF(IDA.GE.81.AND.IDA.LE.100) THEN
C...generator specific codes
      ELSEIF(LEN.NE.0)THEN
C...everything else
        IF(KQ.EQ.6) CHAU(LEN+1:LEN+2)='++'
        IF(KQ.EQ.-6) CHAU(LEN+1:LEN+2)='--'
        IF(KQ.EQ.3) CHAU(LEN+1:LEN+1)='+'
        IF(KQ.EQ.-3) CHAU(LEN+1:LEN+1)='-'

        IF(KQ.EQ.0.AND.IDA.LE.22) THEN
C...quarks and leptons
        ELSEIF(KQ.EQ.0.AND.(IDA.GE.41.AND.IDA.LE.56)) THEN
C...left squarks, sleptons, etc.
        ELSEIF(KQ.EQ.0.AND.(IDA.GE.61.AND.IDA.LE.67)) THEN
C...right squarks
        ELSEIF(KQ.EQ.0.AND.(IDA.GE.71.AND.IDA.LE.76)) THEN
C...right sleptons
        ELSEIF(IDA.GT.100.AND.IDQ3.EQ.0.AND.IDQ2.EQ.IDQ1.AND.
     &      IDQ2.NE.1) THEN
C... eta, psi, upsilon, etc.
        ELSEIF(KQ.EQ.0) THEN
          CHAU(LEN+1:LEN+1)='0'
        ENDIF
      ENDIF

      RETURN
      END

      FUNCTION HEPCMP(KQ)

C...Purpose: to compress the standard ID codes for use in mass and decay
C...arrays; also to check whether a given code actually is defined.

C...Subdivide standard ID code into constituent pieces.
      HEPCMP=0
      KQA=IABS(KQ)
      KQX=MOD(KQA/10000,10)
      KQ3=MOD(KQA/1000,10)
      KQ2=MOD(KQA/100,10)
      KQ1=MOD(KQA/10,10)
      KQJ=MOD(KQA,10)
      KQSM=(KQJ-1)/2

C...Simple cases: direct translation or special codes.
      IF(KQA.EQ.0.OR.KQA.GE.100000) THEN
C... illegal
      ELSEIF(KQA.LE.99) THEN
        HEPCMP=KQA
C... need to check antiparticle legality
        ICH=HEPCHG(KQ)
        IF(KQ.LT.0.AND.ICH.EQ.0)THEN
          IF(KQA.GE.1.AND.KQA.LE.8)THEN
C... quarks
          ELSEIF(KQA.GE.11.AND.KQA.LE.18)THEN
C... leptons
          ELSEIF(KQA.EQ.28 .OR. KQA.EQ.29 .OR. KQA.EQ.40)THEN
          ELSEIF(KQA.GE.41.AND.KQA.LE.46)THEN
C... left squarks
          ELSEIF(KQA.GE.51.AND.KQA.LE.56)THEN
C... left sleptons
          ELSEIF(KQA.GE.61.AND.KQA.LE.66)THEN
C... right squarks
          ELSEIF(KQA.GE.71.AND.KQA.LE.76)THEN
C... right sleptons
          ELSEIF(KQA.GE.81.AND.KQA.LE.99)THEN
C... internal definitions
          ELSE
            HEPCMP=0
          ENDIF
        ENDIF
      ELSEIF(KQJ.EQ.0) THEN
C... KS and KL
        IF(KQ.EQ.130) HEPCMP=102
        IF(KQ.EQ.310) HEPCMP=101
C... jetset diffr particles
        IF(KQ.EQ.110) HEPCMP=104
        IF(KQA.EQ.210) HEPCMP=105
        IF(KQA.EQ.2110) HEPCMP=106
        IF(KQA.EQ.2210) HEPCMP=107
C...Mesons.
      ELSEIF(KQ3.EQ.0) THEN
        IF(KQ2.EQ.0.OR.KQ2.EQ.9.OR.KQ1.EQ.0.OR.KQ1.EQ.9) THEN
C... illegal (no gluons allowed)
        ELSEIF(KQ2.LT.KQ1) THEN
C... illegal quark order
        ELSEIF(KQ.LT.0.AND.(KQ2.EQ.KQ1)) THEN
C... illegal antiparticle
        ELSEIF(MOD(KQJ,2).EQ.0)THEN
C... illegal spin
        ELSEIF(KQ2.EQ.KQ1) THEN
          IF(KQ2.LE.6)THEN
            HEPCMP = 110 + (KQ2-1)*50 + KQSM*10 + KQX
          ELSEIF(KQX.LE.2)THEN
            HEPCMP = 410 + (KQ2-7)*15 + KQSM*3 + KQX
          ENDIF
        ELSEIF(KQ2.EQ.3 .OR. KQ2.EQ.2) THEN
          HEPCMP = 440 + (KQ2+KQ1-3)*50 + KQSM*10 + KQX
        ELSEIF(KQ2.EQ.4)THEN
          HEPCMP = 590 + (KQ1-1)*50 + KQSM*10 + KQX
        ELSEIF(KQ2.EQ.5)THEN
          HEPCMP = 740 + (KQ1-1)*50 + KQSM*10 + KQX
        ELSEIF(KQ2.EQ.6)THEN
          HEPCMP = 940 + (KQ1-1)*50 + KQSM*10 + KQX
        ELSEIF(KQ2.EQ.7.AND.KQX.LE.2)THEN
          HEPCMP = 1190 + (KQ1-1)*15 + KQSM*3 + KQX
        ELSEIF(KQ2.EQ.8.AND.KQX.LE.2)THEN
          HEPCMP = 1280 + (KQ1-1)*15 + KQSM*3 + KQX
        ENDIF

C...Diquarks.
      ELSEIF(KQ1.EQ.0) THEN
        IF(KQX.NE.0)THEN
C... no excited states
        ELSEIF(KQJ.NE.1.AND.KQJ.NE.3) THEN
C... only 2 spin states
        ELSEIF(KQ3.EQ.0.OR.KQ3.EQ.9.OR.KQ2.EQ.0.OR.KQ2.EQ.9) THEN
C... illegal (no gluons allowed)
        ELSEIF(KQ3.LT.KQ2) THEN
C... illegal quark order
        ELSE
C... all diquarks are the same...
          HEPCMP=103
        ENDIF

C...Baryons.
      ELSEIF(KQX.EQ.0.AND.(KQJ.EQ.2.OR.KQJ.EQ.4)) THEN
C... no excited states and only spins 1/2 and 3/2
        IF(KQ3.EQ.0.OR.KQ3.EQ.9.OR.KQ2.EQ.0.OR.KQ2.EQ.9.OR.
     1        KQ1.EQ.0.OR.KQ1.EQ.9) THEN
C... illegal (no gluons allowed)
        ELSEIF(KQ3.LT.KQ1.OR.KQ3.LT.KQ2) THEN
C... illegal quark order
        ELSEIF(KQ3.GE.7) THEN
C... l and h baryons are all the same
          HEPCMP=101+KQ3
          IF(KQ3.EQ.KQ1.AND.KQ3.EQ.KQ2.AND.KQJ.EQ.2) HEPCMP=0
          IF(KQ2.LT.KQ1 .AND. KQ3.EQ.KQ1) HEPCMP=0
          IF(KQ2.LT.KQ1 .AND. KQJ.EQ.4) HEPCMP=0
          IF(KQ3.GT.8) HEPCMP=0
        ELSEIF(KQJ.EQ.2) THEN
C...Spin 1/2 baryons.
          IF(KQ3.EQ.KQ1 .AND. KQ3.EQ.KQ2)THEN
          ELSEIF(KQ2.LT.KQ1 .AND. KQ3.EQ.KQ1)THEN
          ELSEIF(KQ2.LT.KQ1) THEN
            HEPCMP=1385+((KQ3-1)*(KQ3-2)*(KQ3-3))/6+
     1                  ((KQ1-1)*(KQ1-2))/2+KQ2
          ELSE
            HEPCMP=1425+((KQ3+1)*KQ3*(KQ3-1))/6+(KQ2*(KQ2-1))/2+KQ1
          ENDIF
        ELSEIF(KQJ.EQ.4) THEN
C...Spin 3/2 baryons.
          IF(KQ2.LT.KQ1 .AND. KQ3.EQ.KQ1)THEN
          ELSEIF(KQ2.LT.KQ1) THEN
            HEPCMP=1405+((KQ3-1)*(KQ3-2)*(KQ3-3))/6+
     1                  ((KQ1-1)*(KQ1-2))/2+KQ2
          ELSE
            HEPCMP=1485+((KQ3+1)*KQ3*(KQ3-1))/6+(KQ2*(KQ2-1))/2+KQ1
          ENDIF
        ENDIF

      ENDIF

      RETURN
      END

CDECK  ID>, HEPCHG. 
      FUNCTION HEPCHG(KQ)

C...Purpose: to give three times the charge for a particle/parton.
      INTEGER ICHG(109)
      DATA ICHG/-1,2,-1,2,-1,2,-1,2,2*0,-3,0,-3,0,-3,0,-3,6*0,
     1          3,6*0,3,0,3,6,2*0,3,6,2*0,-1,2,-1,2,-1,2,4*0,
     2          -3,0,-3,0,-3,5*0,-1,2,-1,2,-1,2,4*0,
     3		-3,0,-3,0,-3,0,2*3,2*0,29*0/
      SAVE ICHG

C...Initial values. Simple case of direct readout.
      HEPCHG=0

      KQA=IABS(KQ)
      KQX=MOD(KQA/10000,10)
      KQ3=MOD(KQA/1000,10)
      KQ2=MOD(KQA/100,10)
      KQ1=MOD(KQA/10,10)
      KQJ=MOD(KQA,10)

C...Simple cases: direct translation or special codes.
      IF(KQA.EQ.0.OR.KQA.GE.100000) THEN
C... illegal
      ELSEIF(KQA.LE.100) THEN
        HEPCHG = ICHG(KQA)
      ELSEIF(KQJ.EQ.0) THEN
C... KS and KL (and undefined)
        HEPCHG = 0
C... jetset diffr particles
        IF(KQA.EQ.210) HEPCHG=3
        IF(KQA.EQ.2210) HEPCHG=3

C...Construction from quark content for heavy meson, diquark, baryon.
      ELSEIF(KQ3.EQ.0) THEN
C...Mesons.
        HEPCHG = ICHG(KQ2) - ICHG(KQ1)
C...Strange mesons.
        IF(KQ2.EQ.3) HEPCHG = ICHG(KQ1) - ICHG(KQ2)
C...Beauty mesons.
        IF(KQ2.EQ.5) HEPCHG = ICHG(KQ1) - ICHG(KQ2)
      ELSEIF(KQ1.EQ.0) THEN
C...Diquarks.
        HEPCHG = ICHG(KQ3) + ICHG(KQ2)
      ELSE
C...Baryons
        HEPCHG = ICHG(KQ3) + ICHG(KQ2) + ICHG(KQ1)
      ENDIF

C... fix sign of charge
      IF(KQ.LT.0 .AND. HEPCHG.NE.0) HEPCHG = -HEPCHG

      RETURN
      END
