clear; close all; clc;

% --- 조인트 변수 (rad) ---
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

%% J1
T1 = myTrvec2tform([L1x L1y L1z]) * myAxang2tform([1 0 0 pi/2]);
T1_q = T1 * myAxang2tform([0 0 1 q1]);

%% J2
T2 = T1_q * myTrvec2tform([L2x L2y L2z]) ...
            * myAxang2tform([0 0 1 pi]) ...
            * myAxang2tform([1 0 0 -pi/2]);
T2_q = T2 * myAxang2tform([0 0 1 q2]);

%% J3
T3 = T2_q * myTrvec2tform([L3x L3y L3z]) ...
            * myAxang2tform([0 0 1 pi]) ...
            * myAxang2tform([0 1 0 pi]);
T3_q = T3 * myAxang2tform([0 0 1 q3]);

%% J4
T4 = T3_q * myTrvec2tform([L4x L4y L4z]) ...
            * myAxang2tform([0 0 1 pi]) ...
            * myAxang2tform([0 1 0 pi]);
T4_q = T4 * myAxang2tform([0 0 1 q4]);

%% J5
T5 = T4_q * myTrvec2tform([L5x L5y L5z]) ...
            * myAxang2tform([0 1 0 pi/2]);
T5_q = T5 * myAxang2tform([0 0 1 q5]);

%% J6
T6 = T5_q * myTrvec2tform([L6x L6y L6z]) ...
            * myAxang2tform([0 1 0 -pi/2]);
T6_q = T6 * myAxang2tform([0 0 1 q6]);

%% End-Effector
T7 = T6_q * myTrvec2tform([L7x L7y L7z]) ...
            * myAxang2tform([0 0 1 pi]);

% --- 최종 엔드이펙터 변환행렬 ---
disp(T7);


%% ---- 유틸 함수들 ----

function T = myTrvec2tform(trvec)
    dx = trvec(1); dy = trvec(2); dz = trvec(3);
    T = [1 0 0 dx;
         0 1 0 dy;
         0 0 1 dz;
         0 0 0 1];
end

function T = myAxang2tform(axang)
    ux = axang(1); uy = axang(2); uz = axang(3); theta = axang(4);
    c = cos(theta); s = sin(theta); v = 1-c;
    R = [ux*ux*v+c,     ux*uy*v-uz*s, ux*uz*v+uy*s;
         uy*ux*v+uz*s,  uy*uy*v+c,    uy*uz*v-ux*s;
         uz*ux*v-uy*s,  uz*uy*v+ux*s, uz*uz*v+c];
    T = [R [0;0;0]; 0 0 0 1];
end
