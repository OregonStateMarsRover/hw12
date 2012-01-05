function [L] = digpotImp(RL,RH,Fc)

%Set up all the variables.
Wc = 2*pi*Fc;
C1 = 10E-11;
C2 = 25E-11;
Zc1 = -i/(Wc*C1);
Zc2 = -i/(Wc*C2);

%Calculate the input impedance of the network.
Zin = (Zc1*(RH+Zc2*RL/(Zc2+RL)))/(Zc1+(RH+Zc2*RL/(Zc2+RL)))

%Extract the imaginary part and make a complex conjugate
cpxCjt = -imag(Zin);

%Calculate value of matching inductor
L = cpxCjt/Wc;
return;