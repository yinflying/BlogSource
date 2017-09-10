% ionospheric pierce point position ---------------------------------------
% azel(1) : azimuth  of beacon to satellite (unit in radian)
% azel(2) : elevation of beacon to satellite (unit in radian)
% posr(1) : beacon latitude (unit in radian)
% posr(2) : beacon longitude (unit in radian)
% posp(1) : IPP latitude (unit in radian)
% posp(2) : IPP longitude (unit in radian)
function [posp]=pppos(azel,posr)
RE=6371000; H=450000; D2R=pi/180; 
zr=pi/2-azel(2); 
z=asin(RE*sin(zr)/(RE+H));
posp(1)=asin(cos(zr-z)*sin(posr(1))+sin(zr-z)*cos(posr(1))*cos(azel(1)));
if ( (posp(1) > 70*D2R) && (tan(zr-z)*cos(azel(1)) > tan(pi/2-posr(1))) ) ...
    || ( (posp(1) < -70*D2R) && (-tan(zr-z)*cos(azel(1)) > tan(pi/2+posr(1))) )
    posp(2)=posr(2)+pi-asin(sin(zr-z)*sin(azel(1))/cos(posp(1)));
else
    posp(2)=posr(2)+asin(sin(zr-z)*sin(azel(1))/cos(posp(1)));
end
if posp(2)>pi
    posp(2)=posp(2)-2*pi;
end
if posp(2)<-pi
    posp(2)=posp(2)+2*pi;
end
