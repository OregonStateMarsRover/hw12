function [G,RG,V,Vdif,Rreal,Rctl,Greal] = gain(Vin,Vref,bits)

% Create result vectors, variables, and constants
steps = ceil(Vin/Vref);
RG = zeros(1,steps);
Rctl = RG;
Vdif = RG;
G = RG;
V = RG;
Rdif = 1100;   % Sensing resistor
Rin = 21100;   % Total input resistance
Ria = 49400;   % LT1167 feedback resistance
Rpot = 100E3;  % Digital potentiometer value
Pstp = 2^bits; % Digital potentiometer steps

% Create input voltage step vector
i=1;
while (i<=steps)
    V(i) = i*Vref;
    i=i+1;
end

% Calculate ideal G and RG for each step
i=1;
while (i<=steps)
    Vdif(i) = (V(i)/Rin)*Rdif;
    G(i) = 2*Vref/Vdif(i);
    RG(i) = Ria/(G(i)-1);
    if(RG(i)<0)
        G = 'Vref is too small'
        RG = 0;
        return;
    end
    i = i+1;
end

% Calculate acheivable RG and G with potentiometer parameters
Rmin = Rpot/Pstp;
Rreal = Rmin*ceil(RG./Rmin);
Rerr = Rreal-RG;

Greal = 1+Ria./Rreal;
Gerr = Greal-G;

% Calculate measured voltage
Vout = (Vdif.*Greal)/2;
VOerr = Vout-Vref;

% Generate potentiometer control bytes
Rctl = Rreal./Rmin;

% Generate plots
figure
plot(Rerr)
title('Potentiometer Resistance Error From Calculated Value');
ylabel('Resistance (ohms)');
xlabel('Discrete gain steps');

figure
plot(Gerr);
title('Gain Error From Calculated Value');
ylabel('Gain');
xlabel('Discrete gain steps');

figure
plot(VOerr)
title('Output Voltage - Reference Voltage');
ylabel('Output Voltage (V)');
xlabel('Discrete gain steps');

