%Compar EOM and PROMOD results
close all;
clear all;
clc;
EOMdir= ['\\pnl\projects\EOM\TestCases\TestApril4th2013\'];
genfile=[EOMdir 'Generators-sorted-deduped.csv'];
[gendata, genhead, gentot]=xlsread(genfile);
[m,n]=size(gentot);
for i=2:m
    gencat{i-1,1}=gentot{i,19};
end
 gencatlist=unique(gencat);

GenbyCat=cell(39,length(gencatlist));
for i=1:1
    filename=[EOMdir 'results\ramp-' num2str(i) '.csv'];
    data=xlsread(filename);
    %[p,q]=size(data);
    for j=2:4625 %the total number of generator is 4624
        unitcat=gentot{j,19};
        unitzone=gentot{j,8};
        for catindex=1:length(gencatlist)
            if strcmpi(gencatlist{catindex}, unitcat)
                if isempty(GenbyCat{unitzone+1, catindex})
                    GenbyCat{unitzone+1,catindex}=data(2:end,j);
                else
                    GenbyCat{unitzone+1,catindex}=GenbyCat{unitzone+1,catindex}+data(2:end,j); % zone by generation category
                end
            end
        end
    end
end

totalgenbycat=zeros(24,11);
totalgenbyzone=zeros(24,39);
for i=1:39
    for j=1:11
        if isempty(GenbyCat{i,j})
        else
            totalgenbycat(:,j)=totalgenbycat(:,j)+GenbyCat{i,j};
            totalgenbyzone(:,i)=totalgenbyzone(:,i)+GenbyCat{i,j};
        end
    end
end