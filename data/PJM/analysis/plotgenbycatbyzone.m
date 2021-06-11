close all;
clear all;
clc;
dir=['C:\D3x044\EOM\TestApril4th2013\'];
eomfile=[dir 'EOM_Results by cat.xlsx'];
[eomdata,eomheader, eomtot]=xlsread(eomfile,'CT Gas_EOM');
promodfile=[dir 'East 9_0_2006_Release2Cons_2010_EOMMultiZone_1year_HourlyGenbyCat_all.xlsx'];
[promoddata, promodheader,promodtot]=xlsread(promodfile);
totpromod=zeros(168,1);
toteom=zeros(168,1);
for i=1:39
    plotdata= promoddata((i-1)*24*7+1:i*24*7,4);
    for j=1:168
        if isnan(plotdata(j))
            plotdata(j)=0;
        end
    end
    
    totpromod=totpromod+plotdata;
    toteom=toteom+eomdata(:,i);
    ymin1=min(plotdata);
    ymax1=max(plotdata);
    ymin2=min(eomdata(:,i));
    ymax2=max(eomdata(:,i));
    ymax=max([ymax1 ymax2]);
    ymin=min([ymin1 ymin2]);

    figure(i);
    plot(eomdata(:,i));
    hold on
    plot(plotdata,'r');
    if ymax/ymin<1.1
        ymax=1.1*ymin;
        ymin=0.9*ymin;
        axis([1 168 ymin ymax]);
    else
        axis auto;
        
    end
    

    saveas(gcf,['CT Gas_' eomtot{1,i}],'emf');
end
figure(i+1);
ymin1=min(totpromod);
ymax1=max(totpromod);
ymin2=min(toteom);
ymax2=max(toteom);
ymax=max([ymax1 ymax2]);
ymin=min([ymin1 ymin2]);
plot(toteom);
hold on
plot(totpromod,'r');

if ymax/ymin<1.1
    ymax=1.1*ymin;
    ymin=0.9*ymin;
    axis([1 168 ymin ymax]);
else
    axis auto;

end


    saveas(gcf,['CT Gas_total' ],'emf');