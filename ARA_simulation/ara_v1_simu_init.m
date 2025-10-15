clear; 
close all; 
clc;

%%
EndTime = 2;
AccelTime = 0.5;


%%
theta1 = [-50 20 0]';
theta2 = [-30 -50 0]';
theta3 = theta1 - theta2 - 90;

tmp = zeros(3,1);

start_point = [0 0 0 0 0 0]';  % deg
transit_point = [tmp theta1 theta2 theta3 tmp tmp]';
way_point = [start_point, transit_point]; 


