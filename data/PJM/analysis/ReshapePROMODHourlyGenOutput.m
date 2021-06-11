%Reshape hourly gen by gen
close all;
clear all;
clc;
file= ['C:\D3x044\EOM\TestApril4th2013\East 9_0_2006_Release2Cons_2010_EOMMultiZone_1year_HourlyGen.xlsx'];
[data, header, tot]=xlsread(file);
[m,n]=size(tot);
k=1;
for i=2:24*7:m-4
    genname{1,k}=tot{i,2};
    gen(:,k)=data(i-1:i+24*7-2,end);
    k=k+1;
end
xlswrite('PROMOD_HourlyGenbyGen.xlsx', genname, 'Sheet1', 'C1');
xlswrite('PROMOD_HourlyGenbyGen.xlsx', gen, 'Sheet1', 'C2');
