clear; close all; clc;

figure; hold on; grid on; axis equal;
xlabel('X'); ylabel('Y'); zlabel('Z'); view(3);

deg2rad = pi/180;

q1 = 0 * deg2rad;
q2 = 90 * deg2rad;
q3 = 0 * deg2rad;
q4 = 0 * deg2rad;
q5 = 0 * deg2rad;
q6 = 0 * deg2rad;

% Base 프레임 (원점)
drawFrame(eye(4),'0',5,'-');

% J1
T1 = myTrvec2tform([0 7.5 0]) * myAxang2tform([1 0 0 pi/2]);
T1_q = T1 * myAxang2tform([0 0 1 q1]);
drawFrame(T1_q,'1', 5);

% J2
T2 = T1_q * myTrvec2tform([-2 0 -7.5]) * myAxang2tform([0 0 1 pi]) * myAxang2tform([1 0 0 -pi/2]);
T2_q = T2 * myAxang2tform([0 0 1 q2]);
drawFrame(T2_q,'2', 5);

% J3
T3 = T2_q * myTrvec2tform([-3.5014 19.6897 0]) * myAxang2tform([0 0 1 pi]) * myAxang2tform([0 1 0 pi]);
T3_q = T3 * myAxang2tform([0 0 1 q3]);
drawFrame(T3_q,'3', 5);

% J4
T4 = T3_q * myTrvec2tform([0 -17.8140 0]) * myAxang2tform([0 0 1 pi]) * myAxang2tform([0 1 0 pi]);
T4_q = T4 * myAxang2tform([0 0 1 q4]);
drawFrame(T4_q,'4', 5);

% J5
T5 = T4_q * myTrvec2tform([0 7.3 0]) * myAxang2tform([0 1 0 pi/2]);
T5_q = T5 * myAxang2tform([0 0 1 q5]);
drawFrame(T5_q,'5', 5);

% J6
T6 = T5_q * myTrvec2tform([5.8937 0 0]) * myAxang2tform([0 1 0 -pi/2]);
T6_q = T6 * myAxang2tform([0 0 1 q6]);
drawFrame(T6_q,'6', 6);

% end-effector
T7 = T6_q * myTrvec2tform([-9.9246 -0.3533 -1.3497]) * myAxang2tform([0 0 1 pi]);
drawFrame(T7,'7', 7);



%% --- 사용자 정의 함수들 ---

function T = myTrvec2tform(trvec)
% myTrvec2tform : Translation vector → homogeneous transform
% 입력: trvec = [dx dy dz]
% 출력: 4x4 homogeneous transform
    dx = trvec(1); dy = trvec(2); dz = trvec(3);
    T = [1 0 0 dx;
         0 1 0 dy;
         0 0 1 dz;
         0 0 0 1];
end


function T = myAxang2tform(axang)
% myAxang2tform : Axis-angle → homogeneous transform
% 입력: axang = [ux uy uz θ]
% 출력: 4x4 homogeneous transform
    ux = axang(1); uy = axang(2); uz = axang(3); theta = axang(4);

    c = cos(theta); s = sin(theta); v = 1-c;
    R = [ux*ux*v+c,     ux*uy*v-uz*s, ux*uz*v+uy*s;
         uy*ux*v+uz*s,  uy*uy*v+c,    uy*uz*v-ux*s;
         uz*ux*v-uy*s,  uz*uy*v+ux*s, uz*uz*v+c];
    T = [R [0;0;0]; 0 0 0 1];
end







function drawFrame(T, frameName, scale, style)
% drawFrame : 동차변환행렬 T를 받아 좌표축 시각화
%
% 입력:
%   T         : 4x4 homogeneous transform
%   frameName : 좌표축 이름 (문자열)
%   scale     : 축 길이 (기본값 = 0.5)
%   style     : 라인스타일 ('-' 실선, '--' 점선 등)
%
% 예시:
%   drawFrame(eye(4),'Base',0.5,'-')   % 원점 프레임
%   T = trvec2tform([1 0 0.5])*axang2tform([0 0 1 pi/4]);
%   drawFrame(T,'Frame1',0.5,'--')

    if nargin < 3, scale = 0.5; end
    if nargin < 4, style = '-'; end

    % --- 원점 (T의 translation 부분)
    origin = T(1:3,4)';

    % --- 축 방향 (T의 rotation 부분)
    R = T(1:3,1:3);
    X = scale * R(:,1)';
    Y = scale * R(:,2)';
    Z = scale * R(:,3)';

    % --- 좌표축 그리기
    quiver3(origin(1),origin(2),origin(3),X(1),X(2),X(3), ...
        'r','LineWidth',2,'LineStyle',style,'MaxHeadSize',0.5);
    quiver3(origin(1),origin(2),origin(3),Y(1),Y(2),Y(3), ...
        'g','LineWidth',2,'LineStyle',style,'MaxHeadSize',0.5);
    quiver3(origin(1),origin(2),origin(3),Z(1),Z(2),Z(3), ...
        'b','LineWidth',2,'LineStyle',style,'MaxHeadSize',0.5);

    % --- 텍스트 라벨
    text(origin(1)+X(1),origin(2)+X(2),origin(3)+X(3),['X_',frameName]);
    text(origin(1)+Y(1),origin(2)+Y(2),origin(3)+Y(3),['Y_',frameName]);
    text(origin(1)+Z(1),origin(2)+Z(2),origin(3)+Z(3),['Z_',frameName]);

end
