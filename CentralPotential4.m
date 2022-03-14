



%------------------------------------PROBLEMA 4----------------------------









clear;
clc;
close all;
% K=9e9;

Lbox=10;
[X,Y]=meshgrid(-Lbox:0.1:Lbox,-Lbox:0.1:Lbox);



Strength=10;

Z=-Strength*(X.^2+Y.^2).^(-0.5);
R=(X.^2+Y.^2).^(0.5);

Uaux=-Strength*(X./R).*(R/10).*(1-(R./10).^2).^0.5;
Vaux=-Strength*(Y./R).*(R/10).*(1-(R./10).^2).^0.5;





v1 = VideoWriter('PotencialPendulo14.avi');
open(v1);






%--------------------CONDICIONES INICIALES-----------------------
 
           %OJO! SIMON, ESTOS SON LOS CONTROLES DEL JUEGO
%
 
%La posición inicial que te he dejado puesta está cerca del mínimo de
%potencial que me has pasado pero cámbialo y juega.
 
vx=3;       %VELOCIDAD INICIAL DE LA PARTÍCULA
vy=1;

xpart=-3.5; %POSICIÓN INICIAL DE LA PARTÍCULA 
ypart=2.5;

stepmax=200000; %NÚMERO DE PASOS EN LA SIMULACIÓN. CÓMO DE LARGA QUIERES QUE SEA?
SamplePeriod=200;
VideoPrev=0.3;
%-----------------------------------------------------------------   


deltat=0.0005;


Xstore=zeros(stepmax/SamplePeriod,1);
Ystore=zeros(stepmax/SamplePeriod,1);

tc=0;

internalstep=1;


for step=1:stepmax
    
cc=1;

t=deltat*step;
    
    
%Las siguientes dos lineas son es el gradiente de tu potencial multiplicado
%por (-1). Esto es lo que realmente da de comer al algoritmo. Lo he calculado a mano.
    
% U=-2*xpart;
% V=-2*ypart;

r=((xpart^2)+(ypart^2))^0.5;

U=-Strength*(xpart/r)*(r/10)*(1-(r/10).^2)^0.5;
V=-Strength*(ypart/r)*(r/10)*(1-(r/10).^2)^0.5;

% U=-(Strength*((xpart.^2+ypart.^2).^(-1))).*(xpart./((xpart.^2+ypart.^2).^(0.5)));
% V=-(Strength*((xpart.^2+ypart.^2).^(-1))).*(ypart./((xpart.^2+ypart.^2).^(0.5)));

    

%  U=0;
%  V=0;

fprintf('xpart = %d \n',xpart);
    fprintf('t = %d \n',t);
    fprintf('step = %d \n',step);
%     display(U);
%     display(V)
%     display(xpart);
%     display(ypart);           
              
        
        % este if procesa los choques con las paredes de las partículas con n≠1
        
    
        if (xpart>0.97*Lbox && vx>0) || (xpart<-0.97*Lbox && vx<0)
        
        vx=-vx;
        
        cc=0;
        
        tc=t;
        
        
        
        elseif  (ypart>0.97*Lbox && vy>0) || (ypart<-0.97*Lbox && vy<0)
        
        vy=-vy;
        
        cc=0;
        
        tc=t;

        end
        

  [xpart,vx,ypart,vy]=force(xpart,vx,ypart,cc,vy,U,V,deltat);
  
 
        
if  step/SamplePeriod==floor(step/SamplePeriod)      
 

Xstore(internalstep,1)=xpart;
Ystore(internalstep,1)=ypart; 
internalstep=internalstep+1;
            
end            
         
            
            
       
    
  
    
       
  




       
  

    
    % gráfica que muestra las partículas en movimiento
     
     
    if step/SamplePeriod==floor(step/SamplePeriod) 
     
    figure(3);
    

    
    if step<(stepmax*VideoPrev)
    
    scatter(xpart,ypart,100,'o','filled','b');
     hold on  
%     scatter(0,0,200,'o','filled','y');
    fprintf('xpartASD = %d \n',xpart);
    plot([xpart,0],[ypart,0],'b');% Aquí hay un bug

    
    
 
     
    xlim([-Lbox,Lbox]);
    ylim([-Lbox,Lbox]);
    

    set(gca,'XColor', 'none','YColor','none')
    rectangle('Position',[-Lbox -Lbox 2*Lbox 2*Lbox],'LineWidth',2)
 
    
  
    frame = getframe(gcf);
    writeVideo(v1,frame);
    
    hold off
    
    else

        
            scatter(xpart,ypart,100,'o','filled');
     hold on
%     scatter(0,0,200,'o','filled','y');
    
    

    
    
 
     
    xlim([-Lbox,Lbox]);
    ylim([-Lbox,Lbox]);
    

    set(gca,'XColor', 'none','YColor','none')
    rectangle('Position',[-Lbox -Lbox 2*Lbox 2*Lbox],'LineWidth',2)
 
    
  
    frame = getframe(gcf);
    writeVideo(v1,frame);
    
    hold on
    
    
    
    
    
    
    end
    

 
  
    end
    
    
    
    end
    

 
  



close(v1);

% figure(4);
% 
%     
% %     hold on
% %     scatter(xpart(2,1),ypart(2,1),'g','filled','r');
% 
% hold all
% for step=1:(stepmax/SamplePeriod)
%     
% scatter(Xstore(step),Ystore(step),40,'o','filled','r')
% % line(Xstore(step),Ystore(step))
% end
%  
%     
%  
%      
%     xlim([-Lbox,Lbox]);
%     ylim([-Lbox,Lbox]);
%     
% 
%     set(gca,'XColor', 'none','YColor','none')
%     rectangle('Position',[-Lbox -Lbox 2*Lbox 2*Lbox],'LineWidth',2)
%  
% hold off

% figure (5)
% 
% 
%     r1=pcolor(Z);
%  r1.EdgeColor = 'none';
%     set(gca,'xtick',[])
%     set(gca,'ytick',[])
%  
% %  xticks([0 20 40 60 80 100 120 140 160 180 200 250])
% %  xticklabels({'-10','-8','-6','-4','-2','0','2','4','6','8','10','15'})
% %  yticks([0 20 40 60 80 100 120 140 160 180 200 250])
% %  yticklabels({'-10','-8','-6','-4','-2','0','2','4','6','8','10','15'})
% %  hold on
% %  xlabel('x(cm)') 
% %  ylabel('y(cm)') 
%  
%  hold on
% 
% for step=1:stepmax/SamplePeriod
%     
% 
%    
% scatter(10*Xstore(step)+100,10*Ystore(step)+100,40,'o','filled','r')
% hold on
% 
% 
%      xlim([0,200]);
%      ylim([0,200]);
%   
%     
%     hold on
% end




figure (6)


for step=2:(stepmax/SamplePeriod)
% line(10*Xstore(step)+100,10*Ystore(step)+100);
plot([Xstore(step),Xstore(step-1)],[Ystore(step),Ystore(step-1)],'LineWidth',3,'color','r');
disp(step);
     hold on
     xlim([-10,10]);
     ylim([-10,10]);
     hold on
end   

% figure (7)
% 
%   r1=pcolor(Z);
%  r1.EdgeColor = 'none';
%     set(gca,'xtick',[])
%     set(gca,'ytick',[])
%     
%     hold on;
% 
% for step=2:(stepmax/SamplePeriod)
% % line(10*Xstore(step)+100,10*Ystore(step)+100);
% plot([10*Xstore(step)+100,10*Xstore(step-1)+100],[10*Ystore(step)+100,10*Ystore(step-1)+100],'LineWidth',3,'color','b');
% disp(step);
%      hold on
%      xlim([0,200]);
%      ylim([0,200]);
%      hold on
% end   






% set(r, 'EdgeColor', 'none');



saveas(gcf,'Trayectoria de la Partícula12.jpg')


        
function [xpart,vx,ypart,vy]=force(xpart,vx,ypart,cc,vy,U,V,deltat)

Fric=1;% Con Fric=1, no hay fricción

% Fric=0.99995;
        
    if cc>0
            
    xpart=xpart+vx*deltat;
    
    vx=Fric*vx+U*deltat;
    
    ypart=ypart+vy*deltat;
    
    vy=Fric*vy+V*deltat;
    
    end
    
end


