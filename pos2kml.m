function pos2kml(lat,lon,hgt,att,out_file,opt)
    % brief Generate google earth kml file
    % author yinflying(yinflying@foxmail.com)
    % date 2018-10-07
    % Input:    lat (nx1,double) : latitude vector
    %           lon (nx1,double) : longitude vector
    %           hgt (nx1,double or []) : height vector
    %           att (nx1,integer or []) : attributes vector, stand for
    %               different color points, only 7 color supported.
    %           out_file (string) : out kml file path and name
    %           opt (struct, optional): more options
    %               .line(0 or 1): show or hide line between points
    %                               defalut 1
    %               .line_color(string): line color, html color, e.g.
    %                               '#ff0000ff'
    %               .line_width(double): line width, e.g. 5 , default 2

    % Call Example:
    %   1.  opt.line = 1; opt.line_color = '#FF00FFFF'; opt.line_width = 5;
    %       pos2kml(lat,lon,[],[],'out.kml',opt);
    %   2.  pos2kml(lat,lon,[],ones(length(lat),1),'out.kml');
    %
    % history: 2018/10/07   created
    %          2018/10/13   add line support

    % Default 7 color points(corresponding to 1~7 in att)
    all_color = {'FF7093DB','FF00FF00','FF00FFFF','FFFFFF00','FFE47833',...
        'FF0000FF','FF8E236B'};

    if isempty(att)
        att = zeros(length(lat),1);
    end
    if isempty(hgt)
        hgt = zeros(length(lat),1);
    end
    if length(lat) ~= length(lon) || length(lat) ~= length(att)
        fprintf('ERROR: vector length are not equal !\n');
    end
    [fdir,~,~] = fileparts(out_file);
    if ~isempty(fdir) && ~exist(fdir,'dir')
        mkdir(fdir);
    end

    if ~exist('opt','var') || ~isfield(opt,'line')
        opt.line = 1;
    end
    if  ~exist('opt','var') || ~isfield(opt,'line_color')
        opt.line_color = '#FF00FFFF';
    end
    if  ~exist('opt','var') || ~isfield(opt,'line_width')
        opt.line_width = 2;
    end

    icon_url = 'http://maps.google.com/mapfiles/kml/pal2/icon18.png';

    fileID = fopen(out_file,'w');
    fprintf(fileID,'<?xml version="1.0" encoding="UTF-8"?>\n');
    fprintf(fileID,'<kml xmlns="http://earth.google.com/kml/2.1">\n');
    fprintf(fileID,'<Document>\n');
    % Define Style
    for i = 1:size(all_color,2)
        fprintf(fileID,'<Style id="P%.0f">\n',i-1);
        fprintf(fileID,'  <IconStyle>\n');
        fprintf(fileID,'    <color>%s</color>\n',all_color{i});
        fprintf(fileID,'    <scale>0.2</scale>\n');
        fprintf(fileID,'    <Icon><href>%s</href></Icon>\n',icon_url);
        fprintf(fileID,'  </IconStyle>\n');
        fprintf(fileID,'</Style>\n');
    end

    fprintf(fileID,'<Folder>\n');
    fprintf(fileID,'  <name>Rover Position</name>\n');

    fprintf(fileID,'<Placemark>\n');
    fprintf(fileID,'<LineString>\n');
    fprintf(fileID,'<coordinates>\n');
    for i = 1:size(lat,1)
        fprintf(fileID,'%.9f,%.9f,%.9f\n',lon(i),lat(i),hgt(i));
    end
    fprintf(fileID,'</coordinates>\n');
    fprintf(fileID,'</LineString>\n');
    if opt.line == 1
        fprintf(fileID,'<Style>\n');
        fprintf(fileID,'    <LineStyle>\n');
        fprintf(fileID,'         <color>%s</color>\n',opt.line_color);
        fprintf(fileID,'         <width>%f</width>\n',opt.line_width);
        fprintf(fileID,'    </LineStyle>\n');
        fprintf(fileID,'</Style>\n');
    end
    fprintf(fileID,'</Placemark>\n');

    for i = 1:size(lat,1)
        fprintf(fileID,'<Placemark>\n');
        fprintf(fileID,...
            '<styleUrl>#P%.0f</styleUrl>\n',mod(att(i),length(all_color)));
        fprintf(fileID,'<Point>\n');
        fprintf(fileID,...
            '<coordinates>%.9f,%.9f,%.9f</coordinates>\n',lon(i),lat(i),hgt(i));
        fprintf(fileID,'</Point>\n');
        fprintf(fileID,'</Placemark>\n');
    end

    fprintf(fileID,'</Folder>\n');
    fprintf(fileID,'</Document>\n');
    fprintf(fileID,'</kml>\n');
    fclose(fileID);
end
