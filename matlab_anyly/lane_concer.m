clc;clear all;
close all;
% fidin=fopen('dash_lane_anno.txt');                               % 打开test2.txt文件
% fidin.data
detect=importdata('lane_concer_det.txt','%s%*[^\n]');
annot=importdata('dash_lane_anno.txt','%s%*[^\n]');

% %test为待转换的cell
% for n=1:length(test)
%       x{n}=str2num(test{n}(34:end));
% end
% for m=1:length(x)
%       y(m)=x{m}(1);
% end



%annot为待转换的cell
for n=1:length(annot)
    x{n}=str2num(annot{n}(34:end))/4;   % /4
    imag_annot(n)=string(annot{n}(1:33));
end

for j=1:length(detect)
    x2{j}=str2num(detect{j}(34:end));
    imag_detect(j)=string(detect{j}(1:33));
end

% y=-ones(length(annot),48);
num=0;
error_frames=0;error1_sum_mean=0;error2_sum_mean=0;error3_sum_mean=0;error4_sum_mean=0;
for m=1:length(x)%2717
    for k=1:length(x2)
% for m=1:1%2717
%     for k=1:5
        if(imag_annot(m)==imag_detect(k))
            %             disp("++++++++++++");
%             num=num+1;
            for h=1:length(x{m})%   /8
                y(m,h)=x{m}(h);
            end
%             plot(y(1:2:length(x{m})),y(2:2:length(x{m})),'b*')
%             hold on;
            for g=1:length(x2{k})%   /8
                y2(k,g)=x2{k}(g);
            end
%             plot(y2(1:2:length(x2{k})),y2(2:2:length(x2{k})),'g+')
%             hold on;
            error_frame=0;
            for p=1:(length(x{m})/8)
                distmin=10000;error_object=0;
                for q=1:(length(x2{k})/8)
                    dist_annot=sqrt((y(m,8*(p-1)+1)-y(m,8*(p-1)+2)).^2+(y(m,8*(p-1)+1)-y(m,8*(p-1)+2)).^2);
                    dist_annot_detet=sqrt((y(m,8*(p-1)+1)-y2(k,8*(q-1)+1)).^2+(y(m,8*(p-1)+2)-y2(k,8*(q-1)+2)).^2);
                     if(dist_annot_detet<distmin)%if(dist_annot_detet<dist_annot/8)
                         distmin=dist_annot_detet;
                         qq=q;
                    end
                end
                if(distmin<dist_annot/8)
                    error1=sqrt((y(m,8*(p-1)+1)-y2(k,8*(qq-1)+1)).^2+(y(m,8*(p-1)+2)-y2(k,8*(qq-1)+2)).^2);error1_sum_mean=error1_sum_mean+error1;
                    error2=sqrt((y(m,8*(p-1)+3)-y2(k,8*(qq-1)+3)).^2+(y(m,8*(p-1)+4)-y2(k,8*(qq-1)+4)).^2);error2_sum_mean=error2_sum_mean+error2;
                    error3=sqrt((y(m,8*(p-1)+5)-y2(k,8*(qq-1)+5)).^2+(y(m,8*(p-1)+6)-y2(k,8*(qq-1)+6)).^2);error3_sum_mean=error3_sum_mean+error3;
                    error4=sqrt((y(m,8*(p-1)+7)-y2(k,8*(qq-1)+7)).^2+(y(m,8*(p-1)+8)-y2(k,8*(qq-1)+8)).^2);error4_sum_mean=error4_sum_mean+error4;   
                    error_object=(error1+error2+error3+error4)/4;% XXXXXXXXXXXXXxxx
%                     disp(imag_annot(m)+"++1024*540++"+p+"  "+qq);
                    num=num+1;
                end
                error_frame= error_frame+error_object;
            end
             error_frame= error_frame/(length(x{m})/8);
             error_frames=error_frames+error_frame;
            break
        end
    end
end
error_frames=error_frames/length(x); % 每一帧的误差

error1_sum_mean=error1_sum_mean/(length(x{m})/8)/length(x); %某个角点误差
error2_sum_mean=error2_sum_mean/(length(x{m})/8)/length(x); %某个角点误差
error3_sum_mean=error3_sum_mean/(length(x{m})/8)/length(x); %某个角点误差
error4_sum_mean=error4_sum_mean/(length(x{m})/8)/length(x); %某个角点误差