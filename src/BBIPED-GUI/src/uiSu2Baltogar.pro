#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T10:21:30
#
#    BBIPED GUI is free software: you can redistribute it and/or modify
#    it under the terms of the Lesser GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    Lesser GNU General Public License (LGPL) for more details.
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://opensource.org/licenses/LGPL-3.0>.
#
#-------------------------------------------------

QT += core gui xml widgets

TARGET = BBIPED_platform
# uiSu2Baltogar
TEMPLATE = app
ICON = logo.gif
CONFIG += console qwt help debug

#-------------------------------------
# INSTALLS
install_it_debug.path=$$OUT_PWD/debug/.tools/Automatization
install_it_release.path=$$OUT_PWD/release/.tools/Automatization
install_it_debug.files=Automatization/*
install_it_release.files=Automatization/*

INSTALLS += install_it_debug install_it_release


#--------------------------------------------------------------------------------------
# Added for doing the desktop laucher
# sol: http://qttutorial.blogspot.com.es/2013/03/creating-debian-folder-part-2.html
#target.path=/usr/bin/BBIPED/
#shortcutfiles.files+=BBIPED_platform.desktop
#shortcutfiles.path=/usr/share/applications
#data.files=logo/BBiped6.gif
#data.path=/usr/share/BBIPED/

#INSTALLS+=target
#INSTALLS+=shortcutfiles
#INSTALLS+=data
#--------------------------------------------------------------------------------------
SOURCES += main.cpp\
        mainwindow.cpp \
    # Utility source
    Utility/utilfiles.cpp \
    Utility/qtableutil.cpp \
    Utility/treeviewutil.cpp \
    Utility/Model/DomModel/dommodel.cpp \
    Utility/Model/DomModel/domitem.cpp \
    Utility/qhashhelper.cpp \
    Utility/listhelper.cpp \
    Utility/Xml/xmlutil.cpp \
    Utility/showmessagehelper.cpp \
    # Solver source
    Solver/solverconfigmanager.cpp \
    Solver/solverconfigwriter.cpp \
    Solver/solverconfigreader.cpp \
    Solver/solverutil.cpp \
    Solver/markermenumanager.cpp \
    Solver/markermenureader.cpp \
    # Data source
    Data/solversectiondao.cpp \
    Data/ProjectData/VO/projectdata.cpp \
    Data/Solver/VO/solversectionvo.cpp \
    Data/Solver/VO/solvervariablevo.cpp \
    Data/Solver/VO/solvervo.cpp \
    Data/Solver/VO/solverresidualvo.cpp \
    Data/Solver/VO/solvermarkermenu.cpp \
    Data/Solver/VO/solvervarhelpvo.cpp \
    Data/SalomeTemplateData/salomevariabledata.cpp \
    Data/SalomeTemplateData/salometemplatedata.cpp \
    Data/SalomeTemplateData/salomepythonfileutil.cpp \
    Data/ProjectData/VO/multizoneprojectdata.cpp \
    Data/PreferencesData/userpreferencesdata.cpp \
    Data/ProjectData/VO/projectdetails.cpp \
    # Parser source
    Parser/parserfiles.cpp \
    Parser/Solver/configurationfileparser.cpp \
    Parser/Solver/meshfileparser.cpp \
    Parser/Solver/outputsolverparser.cpp \
    # Plotter source
    Plotter/plotterhelper.cpp \
    Plotter/solverplotter.cpp \
    Plotter/plottercurve.cpp \
    Plotter/plotterbackground.cpp \
    # Helper source
    WindowHelper/projectmenutreehelper.cpp \
    Help/helpmanager.cpp \
    # Searcher source
    Searcher/searchedpair.cpp \
    Searcher/searchpairhelper.cpp \
    # HTML source
    Html/htmlformatter.cpp \
    # Rule source
    Rule/abstractrule.cpp \
    Rule/variablerule.cpp \
    Rule/ruleengine.cpp \
    # Preferences source
    Preferences/preferencesmanager.cpp \
    # Multizone source
    Multizone/multizonemanager.cpp \
    # Template source
    Templates/templatemanager.cpp \
    Templates/salometemplatemanager.cpp \
    # Forms source
    Forms/helpdialog.cpp \
    Forms/summarydialog.cpp \
    Forms/aboutusdialog.cpp \
    Forms/ProjectDialog/multizoneprojectdialog.cpp \
    Forms/EditMenuDialog/projectsettingsdialog.cpp \
    Forms/EditMenuDialog/preferencedialog.cpp \
    Forms/SolverMenuDialog/filecomparisondialog.cpp \
    Forms/SolverMenuDialog/meshdialog.cpp \
    Forms/SolverMenuDialog/solverbasicdialog.cpp \
    Forms/SolverMenuDialog/specificboundaryvariabledialog.cpp \
    Forms/SolverMenuDialog/templatedialog.cpp \
    Forms/ToolsDialog/salometemplatedialog.cpp \
    Forms/ToolsDialog/salometoolcreatemeshdialog.cpp \
    Forms/ToolsDialog/salomeoptimizationdialog.cpp\
    Forms/ToolsDialog/askfortemplatenamedialog.cpp \
    Forms/WarningDialog/warningmultizonedialog.cpp \
    Forms/WarningDialog/warningclosingdialog.cpp \
    Forms/ToolsDialog/salomeautomaticmeshprogressdialog.cpp \
    Forms/SolverMenuDialog/clipdomaindialog.cpp \
    Forms/VirtualRotatingDialog/virtualrotatingdialog.cpp \
    Forms/VirtualRotatingDialog/virtualcurvedialog.cpp \
    Data/Solver/VirtualMRF/virtualmrfvo.cpp \
    Data/Solver/usermeshvo.cpp \
    WindowHelper/Multizone/MultizoneTabHelper.cpp \
    Forms/ProjectDialog/multizoneselectfilesdialog.cpp \
    WindowHelper/Multizone/multizoneindividualtabform.cpp \
    Forms/SummaryDialog/multizonesummarydialog.cpp \
    WindowHelper/Menu/FileMenuHelper.cpp \
    WindowHelper/Menu/HelpMenuHelper.cpp \
    WindowHelper/Menu/SolverMenuHelper.cpp \
    WindowHelper/Menu/EditMenuHelper.cpp \
    WindowHelper/Menu/ViewMenuHelper.cpp \
    WindowHelper/Menu/ToolsMenuHelper.cpp \
    WindowHelper/MainWindow/MainWindowHelper.cpp \
    WindowHelper/Menu/MenuHelpers/edithelper.cpp \
    WindowHelper/Menu/MenuHelpers/solverhelper.cpp \
    WindowHelper/Menu/MenuHelpers/toolshelper.cpp \
    WindowHelper/MainWindow/inputdatatabhelper.cpp \
    WindowHelper/MainWindow/residualtabhelper.cpp \
    WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.cpp \
    WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.cpp \
    Forms/WarningDialog/warningrestartdialog.cpp \
    Forms/WarningDialog/meshingtoolconfigurationwarningdialog.cpp \
    Data/Solver/MeshInfo/meshinfovo.cpp \
    Data/Solver/MeshInfo/meshinfofilewriter.cpp \
    Data/Solver/MeshInfo/meshinfofilereader.cpp \
    Forms/PlotTab/plottabform.cpp \
    Plotter/plottermanager.cpp \
    Parser/Solver/historyfileparser.cpp




HEADERS  += mainwindow.h \
    # Utility headers
    Utility/utilfiles.h \
    Utility/qtableutil.h \
    Utility/treeviewutil.h \
    Utility/Model/DomModel/dommodel.h \
    Utility/Model/DomModel/domitem.h \
    Utility/Xml/xmlutil.h \
    Utility/showmessagehelper.h \
    Utility/qhashhelper.h \
    Utility/listhelper.h \
    # Defines Headers
    Defines/General/GeneralDefines.h \
    Defines/Tool/Solver/SolverDefines.h \
    Defines/Tool/Solver/MarkerDefines.h \
    Defines/General/MultizoneDefines.h \
    Defines/General/UserPreferencesDefines.h \
    # Solver Headers
    Solver/solverconfigmanager.h \
    Solver/solverconfigwriter.h \
    Solver/solverconfigreader.h \
    Solver/solverutil.h \
    Solver/markermenumanager.h \
    Solver/markermenureader.h \
    # Data Headers
    Data/solversectiondao.h \
    Data/ProjectData/VO/projectdata.h \
    Data/Solver/VO/solversectionvo.h \
    Data/Solver/VO/solvervariablevo.h \
    Data/Solver/VO/solvervo.h \
    Data/Solver/VO/solverresidualvo.h \
    Data/Solver/VO/solvermarkermenu.h \
    Data/ProjectData/VO/multizoneprojectdata.h \
    Data/PreferencesData/userpreferencesdata.h \
    Data/SalomeTemplateData/salomevariabledata.h \
    Data/SalomeTemplateData/salometemplatedata.h \
    Data/SalomeTemplateData/salomepythonfileutil.h \
    Data/Solver/VO/solvervarhelpvo.h \
    Data/ProjectData/VO/projectdetails.h \
    # Parser Headers
    Parser/parserfiles.h \
    Parser/Solver/configurationfileparser.h \
    Parser/Solver/meshfileparser.h \    
    Parser/Solver/outputsolverparser.h \
    # WindowHelper Headers
    WindowHelper/projectmenutreehelper.h \
    Help/helpmanager.h \
    # Plotter Headers
    Plotter/plotterhelper.h \
    Plotter/solverplotter.h \
    Plotter/plottercurve.h \
    Plotter/plotterbackground.h \
    # Searcher Headers
    Searcher/searchedpair.h \
    Searcher/searchpairhelper.h \
    # HTML formater Headers
    Html/htmlformatter.h \
    # Rule Headers
    Rule/abstractrule.h \
    Rule/variablerule.h \
    Rule/ruleengine.h \
    # Templates Headers
    Templates/templatemanager.h \
    Templates/salometemplatemanager.h \
    Templates/SalomeTemplate_Defines.h \
    # Preferences Headers
    Preferences/preferencesmanager.h \
    # Multizone Headers
    Multizone/multizonemanager.h \
    # FORMS Headers
    Forms/summarydialog.h \
    Forms/helpdialog.h \
    Forms/aboutusdialog.h \
    Forms/ProjectDialog/multizoneprojectdialog.h \
    Forms/EditMenuDialog/preferencedialog.h \
    Forms/EditMenuDialog/projectsettingsdialog.h \
    Forms/SolverMenuDialog/solverbasicdialog.h \
    Forms/SolverMenuDialog/meshdialog.h \
    Forms/SolverMenuDialog/specificboundaryvariabledialog.h \
    Forms/SolverMenuDialog/templatedialog.h \
    Forms/SolverMenuDialog/filecomparisondialog.h \
    Forms/ToolsDialog/salometoolcreatemeshdialog.h \
    Forms/ToolsDialog/salomeoptimizationdialog.h \
    Forms/ToolsDialog/salometemplatedialog.h \
    Forms/ToolsDialog/askfortemplatenamedialog.h \
    Forms/WarningDialog/warningmultizonedialog.h \
    Forms/WarningDialog/warningclosingdialog.h \
    Forms/ToolsDialog/salomeautomaticmeshprogressdialog.h \
    Forms/SolverMenuDialog/clipdomaindialog.h \
    Forms/VirtualRotatingDialog/virtualrotatingdialog.h \
    Forms/VirtualRotatingDialog/virtualcurvedialog.h \
    Data/Solver/VirtualMRF/virtualmrfvo.h \
    Data/Solver/usermeshvo.h \
    Forms/ProjectDialog/multizoneselectfilesdialog.h \
    WindowHelper/Multizone/multizoneindividualtabform.h \
    Forms/SummaryDialog/multizonesummarydialog.h \
    WindowHelper/Menu/MenuHelpers/edithelper.h \
    WindowHelper/Menu/MenuHelpers/solverhelper.h \
    WindowHelper/Menu/MenuHelpers/toolshelper.h \
    WindowHelper/MainWindow/inputdatatabhelper.h \
    WindowHelper/MainWindow/residualtabhelper.h \
    WindowHelper/Dialog/SolverDialog/solverconfigmenuhelper.h \
    WindowHelper/Dialog/SolverDialog/solvermarkermenuhelper.h \
    Forms/WarningDialog/warningrestartdialog.h \
    Forms/WarningDialog/meshingtoolconfigurationwarningdialog.h \
    Data/Solver/MeshInfo/meshinfovo.h \
    Data/Solver/MeshInfo/meshinfofilewriter.h \
    Defines/General/MeshInfoDefines.h \
    Data/Solver/MeshInfo/meshinfofilereader.h \
    Forms/PlotTab/plottabform.h \
    Plotter/plottermanager.h \
    Parser/Solver/historyfileparser.h

FORMS  += \
    Forms/helpdialog.ui \
    mainwindow.ui \
    Forms/summarydialog.ui \   
    Forms/aboutusdialog.ui \
    Forms/ProjectDialog/multizoneprojectdialog.ui \
    Forms/EditMenuDialog/preferencedialog.ui \
    Forms/EditMenuDialog/projectsettingsdialog.ui \
    Forms/SolverMenuDialog/solverbasicdialog.ui \
    Forms/SolverMenuDialog/meshdialog.ui \
    Forms/SolverMenuDialog/specificboundaryvariabledialog.ui \
    Forms/SolverMenuDialog/filecomparisondialog.ui \
    Forms/SolverMenuDialog/templatedialog.ui \
    Forms/ToolsDialog/salometemplatedialog.ui \
    Forms/ToolsDialog/salometoolcreatemeshdialog.ui \
    Forms/ToolsDialog/salomeoptimizationdialog.ui\
    Forms/ToolsDialog/askfortemplatenamedialog.ui \
    Forms/WarningDialog/warningmultizonedialog.ui \
    Forms/WarningDialog/warningclosingdialog.ui \
    Forms/ToolsDialog/salomeautomaticmeshprogressdialog.ui \
    Forms/SolverMenuDialog/clipdomaindialog.ui \
    Forms/VirtualRotatingDialog/virtualrotatingdialog.ui \
    Forms/VirtualRotatingDialog/virtualcurvedialog.ui \
    Forms/ProjectDialog/multizoneselectfilesdialog.ui \
    WindowHelper/Multizone/multizoneindividualtabform.ui \
    Forms/SummaryDialog/multizonesummarydialog.ui \
    Forms/WarningDialog/warningrestartdialog.ui \
    Forms/WarningDialog/meshingtoolconfigurationwarningdialog.ui \
    Forms/PlotTab/plottabform.ui

RESOURCES += \
    toolbarIcons.qrc
#\
#    Forms/toolbarIcons.qrc

#Opcion Buena 2
unix:!macx:!symbian: LIBS += -L/usr/lib/ -lqwt
#-lqwt-qt4
INCLUDEPATH += /usr/include/qwt/
#INCLUDEPATH += /usr/include/qwt-qt4

##################
#release:DESTDIR = release
#release:OBJECTS_DIR = release/.o
#release:MOC_DIR = release/.moc
#release:RCC_DIR = release/.rcc
#release:UI_DIR = release/.ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/.o
debug:MOC_DIR = debug/.moc
debug:RCC_DIR = debug/.rcc
debug:UI_DIR = debug/.ui



#release:DESTDIR = release
#release:OBJECTS_DIR = release/.o
#release:MOC_DIR = release/.moc
#release:RCC_DIR = release/.rcc
#release:UI_DIR = release/.ui

##unix:configfiles.extra = chmod +x data/config/WallpaperChanger/Scripts/*; make clean; mv -f wallch-1 wallch
#binfile.files += BBIPED_platform
#binfile.path = /usr/bin/
#configfiles.files += files/*
#configfiles.path = /usr/share/bbiped/
##docfiles.files += data/doc/*
##docfiles.path = /usr/share/doc/
##manfiles.files += data/man/*
##manfiles.path = /usr/share/man/man1/
#logo.files=logo/BBiped6.gif
#logo.path=/usr/share/pixmaps/bbiped/
#shortcutfiles.files += files/bbiped.desktop
#shortcutfiles.path = /usr/share/applications/bbiped/
#INSTALLS += configfiles
##INSTALLS += docfiles
##INSTALLS += manfiles
#INSTALLS += shortcutfiles
#INSTALLS += logo
