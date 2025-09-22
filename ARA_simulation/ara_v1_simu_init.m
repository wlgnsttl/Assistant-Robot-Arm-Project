clear; 
close all; 
clc;

%%
EndTime = 4;
AccelTime = 1;


%%
start_point = [0 -40 -100 25 90 90]';  % deg
transit_point = [
    -20 -30 -130 45 45 10;
    ]';
end_point = [0 -40 -100 25 90 90]';
way_point = [start_point, transit_point, end_point]; 

