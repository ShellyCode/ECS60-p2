%hw5
clf
A=char('bx-','gx-','mx-','rx-','kx-','cx-');
M=[4,16,32,8,64,32];
sizes=16;
z_dB_max=12+15*log10(sizes);
z_dB=0:.1: z_dB_max;
z=10.^(z_dB/10);

%a) QPSK
x1=sqrt(2*z);
pb1=qfunc(x1);

%b)16_DPSK
a=1+ cos(pi/M(2));
b=2* cos(pi/M(2));
c=1-cos(pi/M(2));
A2=2*sqrt(a/b);
k2=2*log2(M(2))*c;
if M(2)~=2
pb2=(A2*qfunc(sqrt(k2*z)))/log2(M(2));
else
pb2=0.5*exp(-z);
end

%c) 32-NFSK
%A3=(M(3)/2*( M(3)-1 ));

pb3=zeros(size(z));
%for k=1:M(3)-1
%   a3=((-1)^(k+1))/(k+1);
%	B=(prod(1:(M(3)-1))/(prod(1:k)*prod(1:(M(3)-1-k))))*a3;
%   B2=a3*(prod((M(3)-k):M(3)-1)/prod(1:k));
%	k3=k*log2(M(3))/(k+1);
%   x=exp(-k3*z);
%  c=A3*B2*x;
%	pb3=pb3+c;
%end

for k = 1:31
Pb3 = Pb3 + nchoosek(31,k) * ((-1).^(k+1)) / (k+1) * exp(-k/(k+1)*5*z);
end
Pb3 = Pb3 * 32 /(2 * 31);

%d) 8-FSK
A4=M(4)/2;
k4=log2(M(4));
pb4=A4*qfunc(sqrt(k4*z));

%e) 64-QAM
a5=1-1/sqrt(M(5));
A5=(4*a5)/log2(M(5));
k5=3*log2(M(5))/(M(5)-1);
if M(5)~=2 && M(5)~=4
pb5=A5*qfunc(sqrt(k5*z));
else
pb5=pb2;
end

%f) 32-PSK
if M(6)~=2 && M(6)~=4
A6=2/log2(M(6));
k6=2*((sin(pi/M(6)))^2)*log2(M(6));
else
k6=1;
A6=1;
end
pb6=A6*qfunc(sqrt(k6*z));

semilogy(z_dB,pb1,A(1,:)),axis([0 z_dB_max 10^(-6) 1]),xlabel('Eb/N0,dB'),ylabel('Pb'),grid,'Linewidth',2
hold on
semilogy(z_dB,pb2,A(2,:)),'Linewidth',2
semilogy(z_dB,pb3,A(3,:)),'Linewidth',2
semilogy(z_dB,pb4,A(4,:)),'Linewidth',2
semilogy(z_dB,pb5,A(5,:)),'Linewidth',2
semilogy(z_dB,pb6,A(6,:)),'Linewidth',2
legend('QPSK', '16_DPSK','32-NFSK','8-FSK','64-QAM','32-PSK');









