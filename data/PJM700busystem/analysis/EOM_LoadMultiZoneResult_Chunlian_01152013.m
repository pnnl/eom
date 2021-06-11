close all
clear all
clc

ZoneName = {'AEP';
        'ALWSTTA';
        'APS';
        'CINERGY';
        'ENTRGY';
        'FEMISO';
        'FRCC';
        'GATEWAY';
        'HYQU';
        'IESO';
        'ISONEBOS';
        'ISONECTS';
        'ISONEEST';
        'ISONEMNE';
        'ISONEWST';
        'MARITIME';
        'MECS';
        'MHSP';
        'MINNESTA';
        'NEBRASKA';
        'NY CDE';
        'NY GHI';
        'NYWESTAB';
        'NYZ-F';
        'NYZ-J';
        'NYZ-K';
        'PJMCENTR';
        'PJMEAST';
        'PJMNICA';
        'SASK';
        'SOCO';
        'SPPCENCA';
        'SPPLOUIS';
        'SPPN';
        'TVACA';
        'VACAR';
        'VIEP';
        'WAPA';
        'WUMS';};

%% Get Zone and Category information for generators
% dir = ['\\pnl\projects\EOM\Multi-ZoneEIC\'];
% filename = [dir 'EICDataBase.xlsx'];
% [Enum,Etxt,Eraw] = xlsread(filename);
% dir = ['C:\Users\gong358\Documents\RIM_Cases\EID_MultiZone_12182012\'];
% filename = [dir 'GeneratorZoneCategory.xlsx'];
% [Gnum,Gtxt,Graw] = xlsread(filename);
% NumGen = length(Graw(:,1));
% NumGenZone = length(Eraw(:,1));
% Graw(1,19) = Eraw(1,4);
% for i = 2:1:NumGen
%     Flag = 0;
%     for j = 2:1:NumGenZone
%         if strcmp(Graw{i,1},Eraw{j,1})
%             Graw(i,19) = Eraw(j,4);
%             Flag = 1;
%             break;
%         end
%     end
%     if 0 == Flag
%         display(['A generator without category -- ' num2str(i)]);
%     end
% end
% xlswrite([dir 'GeneratorZoneCategory.xlsx'],Graw);

%% Get load data
% dir = ['\\pnl\projects\EOM\PROMOD_EOMMultiZoneCompare\'];
% filename = [dir 'East Ventyx Zonal NERC 9_0_org_2010Base_EOMNativeLoad.xlsx'];
% [Lnum,Ltxt,Lraw] = xlsread(filename);
% filename = [dir 'East Ventyx Zonal NERC 9_0_org_2010Base_EOMInterruptibleLoad.xlsx'];
% [Inum,Itxt,Iraw] = xlsread(filename);
% filename = [dir 'East Ventyx Zonal NERC 9_0_org_2010Base_EOMPumpingSchedule.xlsx'];
% [Pnum,Ptxt,Praw] = xlsread(filename);
% filename = [dir 'East Ventyx Zonal NERC 9_0_org_2010Base_EOMTransactionsbyZone_2010.xlsx'];
% [Wnum,Wtxt,Wraw] = xlsread(filename);
% M = length(Lraw(1,:));
% L = length(Lraw(:,1));
% N = length(Iraw(1,:));
% for i = 3:1:N
%     Flag = 0;
%     for j = 3:1:M
%         if strcmp(Iraw{1,i},Lraw(1,j))
%             Lnum(:,j-1) = Lnum(:,j-1) - Inum(:,i-1);
%             Flag = 1;
%             break;
%         end
%     end
%     if 0 == Flag
%         display(['A unmatched zone name -- ' num2str(i)]);
%     end
% end
% N = length(Wraw(1,:));
% for i = 3:1:N
%     Flag = 0;
%     for j = 3:1:M
%         if strcmp(Wraw{1,i},Lraw(1,j))
%             Lnum(:,j-1) = Lnum(:,j-1) - Wnum(:,i-1);
%             Flag = 1;
%             break;
%         end
%     end
%     if 0 == Flag
%         display(['A unmatched zone name -- ' num2str(i)]);
%     end
% end
% N = length(Praw(1,:));
% for i = 3:1:N
%     Flag = 0;
%     for j = 3:1:M
%         if strcmp(Praw{1,i},Lraw(1,j))
%             Lnum(:,j-1) = Lnum(:,j-1) - Pnum(:,i-1);
%             Flag = 1;
%             break;
%         end
%     end
%     if 0 == Flag
%         display(['A unmatched zone name -- ' num2str(i)]);
%     end
% end
% Lraw(2:L,2:M) = num2cell(Lnum);
% for i = 3:1:M
%     ZoneLoad = Lraw(2:L,i);
%     add = '';
%     if i<12
%         add = '0';
%     end
%     csvwrite([dir 'ZoneLoad\Zone_' add num2str(i-2) '.csv'],ZoneLoad);
% end

%% Format and get generation from PORMOD
% dir = ['\\pnl\projects\EOM\PROMOD_EOMMultiZoneCompare\'];
% filename = [dir '2010_noemissioncost_noFOR_EOMMultiZone_HourlyGenbyGen.xlsx'];
% [Gnum,Gtxt,Graw] = xlsread(filename);
% Day = 7;
% Step = Day*24;
% NumGenPromond = (length(Graw(:,1))-1)/Step;
% FormGen(Step+1,NumGenPromond+2) = {''};
% FormGen(:,1:2) = Graw(1:Step+1,2:3);
% for i = 1:1:NumGenPromond
%     FormGen(1,i+2) = Graw(Step*i,1);
%     FormGen(2:Step+1,i+2) = Graw((i-1)*Step+2:Step*i+1,4);
% end
% xlswrite([dir '2010_noemissioncost_PROMOD_HourlyGenbyGen.xlsx'],FormGen);

%% Load EOM results
day = 7;
window = 1;
t = (1:day*24)./24;

dir = ['C:\D3x044\EOM\TestApril4th2013\'];
filename = [dir 'GeneratorZoneCategory_JINC.xlsx'];
[Gnum,Gtxt,Graw] = xlsread(filename);
NumGen = length(Graw(:,1));
filename = [dir 'Generators-sorted-deduped.csv'];
[Znum,Ztxt,Zraw] = xlsread(filename);
NumZoneGen = length(Zraw(:,1));
load Color.mat;

Load = [];
Generation = [];
GeneratorSchedule = [];
eomresultsdir=['\\pnl\projects\EOM\TestCases\TestApril4th2013\results\'];
for k = 0:window:day-1
    filename = [eomresultsdir 'ramp-' num2str(k) '.csv'];
    [data,text,raw] = xlsread(filename);
    Load = [Load; data(2:24*window+1,NumZoneGen+1)];
    GeneratorSchedule = [GeneratorSchedule; data(2:24*window+1,2:NumZoneGen)];
end
Generation = sum(GeneratorSchedule,2);
TotalGeneration = sum(GeneratorSchedule,1);


filename = [dir 'PROMOD_HourlyGenbyGen.xlsx'];
[Hnum,Htxt,Hraw] = xlsread(filename);
NumPro = length(Hraw(1,:));

% figure(1);
% plot(t, Load, color(1,:));
% hold on;
% plot(t, Generation, color(2,:));
% legend('Load', 'Total Generation');
% xlabel('Time (day)');
% ylabel('Power (MW)');
% title('Power and Load Balance');
% grid on;
% hold off;

Category = [{'ST Coal'};
            {'Nuclear (Existing)'};
            {'Combined Cycle (Existing)'};
            {'CT Gas'};
            {'CT Other'};
            {'CT Oil'};];
Num = 1;
CText = Category{5};
GenEOM(1:day*24+1,39) = 0;
for i = 2:1:NumZoneGen
    for j = 2:1:NumGen
        if strcmp(Zraw{i,1},Graw{j,1})
            if strcmp(CText,Graw{j,19})||strcmp(Category{5},Graw{j,19})||strcmp(Category{6},Graw{j,19})
                num = Zraw{i,8}+1; %the index was j changed to be i
                GenEOM(2:day*24+1,num) = GenEOM(2:day*24+1,num) + GeneratorSchedule(:,i-1);
            end
            break;
        end
    end
end
GenPRO(1:day*24+1,39) = 0;
for i = 3:1:NumPro
    for j = 2:1:NumZoneGen
        if strcmp(Hraw{1,i},Zraw{j,1})
            num = Zraw{j,8}+1;
        end
    end
    for j = 2:1:NumGen
        if strcmp(Hraw{1,i},Graw{j,1})
            if strcmp(CText,Graw{j,19})||strcmp(Category{5},Graw{j,19})||strcmp(Category{6},Graw{j,19})
                GenPRO(2:day*24+1,num) = GenPRO(2:day*24+1,num) + Hnum(1:day*24,i-1);
            end
            break;
        end
    end
end

xlswrite([dir 'PROMOD_EOM_Results.xlsx'],GenEOM,[CText '_EOM']);
% xlswrite([dir 'PROMOD_EOM_Results.xlsx'],GenPRO,[CText '_PROMOD']);
xlswrite([dir 'PROMOD_EOM_Results.xlsx'],ZoneName',[CText '_EOM']);
% xlswrite([dir 'PROMOD_EOM_Results.xlsx'],ZoneName',[CText '_PROMOD']);

% figure(2);
% plot(t, GenEOM, color(1,:));
% hold on;
% plot(t, GenPRO, color(2,:));
% legend('EOM', 'PROMOD');
% xlabel('Time (day)');
% ylabel('Power (MW)');
% grid on;
% hold off;
% 
% %%
% 
% GenZoneEOM(1:day*24,39) = 0;
% GenZonePRO(1:day*24,39) = 0;
% for i = 2:1:NumPro
%     for j = 2:1:NumZoneGen
%         if strcmp(Hraw{1,i},Zraw{j,1})
%             num = Zraw{j,8}+1;
%             GenZoneEOM(:,num) = GenZoneEOM(:,num) + GeneratorSchedule(:,j-1);
%             GenZonePRO(:,num) = GenZonePRO(:,num) + Hnum(1:day*24,i-1);
%             break;
%         end
%     end
% end
% 
% figure(2);
% for i = 1:1:39
%     plot(t, GenZoneEOM(:,i), color(1,:));
%     hold on;
%     plot(t, GenZonePRO(:,i), color(2,:));
%     legend('EOM', 'PROMOD');
%     xlabel('Time (day)');
%     ylabel('Power (MW)');
%     title(ZoneName{i,:});
%     grid on;
%     hold off;
%     saveas(gcf,ZoneName{i,:},'emf');
%     input(['Show Zone: ' num2str(i)])
% end
