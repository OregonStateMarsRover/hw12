function [norm] = normalize1(input,A,B)

%Check for proper input format
%if(cols(input)>1)
%    norm = 'input must be a column vector'
%    return;
%end

%Get the minimum value of the input function and the ratio of B and max.
mn = min(input);
ratioBMax = abs(B)/abs(max(input));

%Compare A to the min value. Add or subtract the difference accordingly.
if(mn>A)
    input = input-(abs(A)-abs(mn));
elseif(mn<A)
    input = input+(abs(mn)-abs(A));
end

%Find the ratio of max to B. Multiply or divide accordingly.
if(ratioBMax>1)
    norm = input*ratioBMax;
    return;
elseif(ratioBMax<1)
    norm = input/ratioBMax;
    return;
end