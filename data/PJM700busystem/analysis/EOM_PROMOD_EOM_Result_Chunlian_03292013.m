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
day = 7;
t = (1:day*24)./24;
Category = [{'ST Coal'};
            {'Nuclear'};
            {'Combined Cycle'};
            {'CT Gas'};];
    
dir = ['\\pnl\projects\EOM\PROMOD_EOMMultiZoneCompare\'];
filename = [dir 'East NERC 9_0_2006_Release2Cons_2010_EOMMultiZone_GenbyZonebyCategory.xlsx'];
[P1num,P1txt,P1raw] = xlsread(filename, 'ST Coal');
[P2num,P2txt,P2raw] = xlsread(filename, 'Nuclear');
[P3num,P3txt,P3raw] = xlsread(filename, 'Combined Cycle');
[P4num,P4txt,P4raw] = xlsread(filename, 'CT Gas');
[P5num,P5txt,P5raw] = xlsread(filename, 'CT Oil');
[P6num,P6txt,P6raw] = xlsread(filename, 'CT Other');
filename = [dir 'PROMOD_EOM_Results.xlsx'];
[E1num,E1txt,E1raw] = xlsread(filename, 'ST Coal_EOM');
[E2num,E2txt,E2raw] = xlsread(filename, 'Nuclear (Existing)_EOM');
[E3num,E3txt,E3raw] = xlsread(filename, 'Combined Cycle (Existing)_EOM');
[E4num,E4txt,E4raw] = xlsread(filename, 'CT Gas_EOM');
load Color.mat;

% figure(1);
P4num = P4num + P5num + P6num;
% for i = 1:1:39
%     plot(t, E4num(:,i), color(1,:));
%     hold on;
%     plot(t, P4num(:,i+1), color(2,:));
%     legend('EOM', 'PROMOD');
%     xlabel('Time (day)');
%     ylabel('Power (MW)');
%     title([Category{4} ' --- ' ZoneName{i,:}]);
%     grid on;
%     hold off;
%     saveas(gcf,[Category{4} ' - ' ZoneName{i,:}],'emf');
% %     input(['Show Zone: ' num2str(i)])
% end

figure(2);
plot(t, sum(E3num'), color(1,:));
hold on;
plot(t, sum(P3num(:,2:40)'), color(2,:));
legend('EOM', 'PROMOD');
xlabel('Time (day)');
ylabel('Power (MW)');
title([Category{3}]);
grid on;
hold off;
saveas(gcf,[Category{3}],'emf');




