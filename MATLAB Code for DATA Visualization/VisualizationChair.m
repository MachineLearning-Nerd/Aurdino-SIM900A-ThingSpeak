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

[data,timestamps] = thingSpeakRead(629298,'NumPoints',1000,'ReadKey', readAPIKey);
% timestamps.TimeZone = 'Asia/Kolkata';
% Read second data variable
% data2 = thingSpeakRead(readChannelID, 'Field', fieldID2, 'NumPoints', 30, 'ReadKey', readAPIKey);

%% Process Data

% Concatenate the two data variables
% data = [data1];

[fieldata fieldindex]= rmmissing(data(:,field));
fieldindex = find(fieldindex==0);

time = timestamps(fieldindex);
time = (string(time))';

[m , n] = size(fieldata);
C = max(fieldata)*ones(m,1);
D = 10*ones(m,1);
background = [C D];

%% Plotting Area
figure, 
hold on

h1 = area(background(:,1));
grid on
h1.FaceColor = [0 1 0];
% xticks(1:m)
% xticklabels(1:m)
% xtickangle(45)
xlim([1 m])
xlabel("Date & time")
ylabel("Sitting minutes")

h2 = area(background(:,2));
grid on
h2.FaceColor = [1 0 0];
ylim([0 max(fieldata)])


%% Visualize Data
plot(fieldata,'ks-','LineWidth',2,'MarkerSize',2,'MarkerFaceColor','k');
grid on
% ylim([0 15])
hold off