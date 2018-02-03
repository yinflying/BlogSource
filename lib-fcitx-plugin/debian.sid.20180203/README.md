System info:	Debian sid  
Date:		2017.02.03  
Fcitx version:	4.2.9.4-3   

Compiled using QT and [fcitx-qt5](https://github.com/fcitx/fcitx-qt5.git).

`libfcitxplatforminputcontextplugin.so.for.RStudio` might be used to fix Fcitx Chinese input issue under RStudio 1.1.414. It was compiled using QT 5.4.2 binary version from RStudio team, and dev version of fcitx-qt5 from GitHub.  
Put the file in `/usr/lib/rstudio/bin/plugins/platforminputcontexts/` and 
rename it to `libfcitxplatforminputcontextplugin.so`.