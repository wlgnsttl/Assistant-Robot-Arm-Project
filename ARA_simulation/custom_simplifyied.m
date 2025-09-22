clc
T = T1_q;

syms q1 q2 q3 q4 q5 q6 real

% --- 링크 파라미터 심볼릭 정의 ---
syms L1x L1y L1z real
syms L2x L2y L2z real
syms L3x L3y L3z real
syms L4x L4y L4z real
syms L5x L5y L5z real
syms L6x L6y L6z real
syms L7x L7y L7z real

% --- pi도 심볼릭으로 선언 ---
pi = sym('pi');


T = subs(T, ...
    [cos(pi), sin(pi), cos(pi/2), sin(pi/2), cos(-pi/2), sin(-pi/2)], ...
    [-1, 0, 0, 1, 0, -1]);

T = subs(T, ...
    [L1x L1y L1z  L2x L2y L2z  L3x L3y L3z  L4x L4y L4z  L5x L5y L5z  L6x L6y L6z  L7x  L7y   L7z], ...
    [  0  7.5  0  -2   0  -7.5  -3.5014 19.6897 0   0 -17.8140 0   0 7.3 0   5.8937 0 0  -9.9246 -0.3533 -1.3497]);

digits(4);
T = vpa(T, 4); 