System info:	Debian sid  
Date:		2017.12.23  
Fcitx version:	4.2.9.4-3  

Compiled using QT and [fcitx-qt5](https://github.com/fcitx/fcitx-qt5.git).
`libfcitxplatforminputcontextplugin.so.for.MendeleyDesktop` might be used to fix Fcitx Chinese input issue under Mendeley Desktop 1.17.12. It was compiled using QT 5.5.1 and dev version of fcitx-qt5 from GitHub.  
Put the file in `/opt/mendeleydesktop/plugins/qt/plugins/platforminputcontexts/` and 
rename it to `libfcitxplatforminputcontextplugin.so`.  

`libfcitxplatforminputcontextplugin.so.for.RStudio` might be used to fix Fcitx Chinese input issue under RStudio 1.1.383. It was compiled using QT 5.4.0 binary version from RStudio team, and dev version of fcitx-qt5 from GitHub.  
Put the file in `/usr/lib/rstudio/bin/plugins/platforminputcontexts/` and 
rename it to `libfcitxplatforminputcontextplugin.so`.