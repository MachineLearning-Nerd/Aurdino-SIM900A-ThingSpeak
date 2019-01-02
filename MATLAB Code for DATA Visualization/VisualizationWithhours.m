% MATLAB code for visualizing data from a channel as a filled area
% 2D plot using the AREA function.

% Prior to running this MATLAB code template, assign the channel ID to read
% data from to the 'readChannelID' variable. Also, assign the field IDs
% within the channel that you want to read data from to 'fieldID1', and
% 'fieldID2'. 

% TODO - Replace the [] with channel ID to read data from:
readChannelID = 629298;
% TODO - Replace the [] with the Field ID to read data from:
fieldID1 = [1];
field = 2;
% TODO - Replace the [] with the Field ID to read data from:
% fieldID2 = [];

% Channel Read API Key 
% If your channel is private, then enter the read API
% Key between the '' below: 
readAPIKey = 'P12WOODDOFSDBO7L';

%% Read Data

% Read first data variable
% [data,timestamps] = thingSpeakRead(readChannelID, 'Field', fieldID1, 'ReadKey', readAPIKey);

[data,timestamps] = thingSpeakRead(629298,'NumPoints',200,'ReadKey', readAPIKey);

% timestamps.TimeZone = 'Asia/Kolkata';
% Read second data variable
% data2 = thingSpeakRead(readChannelID, 'Field', fieldID2, 'NumPoints', 30, 'ReadKey', readAPIKey);

%% Process Data

% Concatenate the two data variables
% data = [data1];

[fieldata fieldindex]= rmmissing(data(:,field));
fieldindex = find(fieldindex==0);

time = timestamps(fieldindex);

dayoftime = day(time);
days = unique(dayoftime);
mindays = [];
for i = 1:length(days)
[ind] = find(dayoftime==days(i));
mindays = [mindays sum(fieldata(ind))];
end

hourdays = mindays'/60; % 60 converting hour from minute 

time = (string(days));

[m , n] = size(hourdays);
C = 15*ones(m,1);
D = 10*ones(m,1);
background = [C D];

%% Plotting Area
if (m==1)
    figure, 
    hold on

    h1 = area([15 ; 15 ;15 ]);
    grid on
    h1.FaceColor = [0 1 0];
    xticks(1:3)
    xticklabels([0 time 0])
    xtickangle(0)
    xlabel("Days")
    ylabel("Sitting Hours in a Day")
    h2 = area([background(:,2); background(:,2); background(:,2)]);
    grid on
    h2.FaceColor = [1 0 0];
    
    %% Visualize Data
    plot([0 hourdays 0],'ks','LineWidth',2,'MarkerSize',15,'MarkerFaceColor','k');
    grid on
    % ylim([0 15])
    hold off
else
    figure, 
    hold on

    h1 = area(background(:,1));
    grid on
    h1.FaceColor = [0 1 0];
    xticks(1:m)
    xticklabels(time)
    xtickangle(0)
    
    yticks(0:15)
    yticklabels(0:15)
    ytickangle(0)
    
    xlabel("Days")
    ylabel("Sitting Hours in a Day")
    h2 = area(background(:,2));
    grid on
    h2.FaceColor = [1 0 0];
    ylim([0 15])


    %% Visualize Data
    plot(hourdays,'ks-','LineWidth',2,'MarkerSize',5,'MarkerFaceColor','k');
    grid on
    % ylim([0 15])
    hold off
end
