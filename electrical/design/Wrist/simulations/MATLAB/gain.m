function [G,RG,V] = gain(Vin,Vref)

% Create result vectors, variables, and constants
steps = Vin/Vref;
RG = zeros(1,steps);
G = RG;
V = RG;
Rdif = 1000; % Sensing resistor
Rin = 21000; % Total input resistance
Ria = 49400; % LT1167 feedback resistance

% Create input voltage step vector
i=1;
while (i<=steps)
    V(i) = i*Vref;
    i=i+1;
end

% Calculate G and RG for each step
i=1;
while (i<=steps)
    Vdif = (V(i)/Rin)*Rdif;
    G(i) = Vref/Vdif;
    RG(i) = Ria/(G(i)-1);
    if(RG(i)<0)
        G = 'Vref is too small'
        RG = 0;
        return;
    end
    i = i+1;
end