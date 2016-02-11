/***
 *    This file is part of BBIPED GUI.
 *
 *   BBIPED GUI is free software: you can redistribute it and/or modify
 *   it under the terms of the Lesser GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   BBIPED GUI is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   Lesser GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Foobar. If not, see <http://opensource.org/licenses/LGPL-3.0>.
 *
 * * Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "salomepythonfileutil.h"
#include <QDir>
#include <QList>
#include <QCoreApplication>
#include "../../Defines/General/GeneralDefines.h"
/**
 * @brief  Constructor
 *
 */
SalomePythonFileUtil::SalomePythonFileUtil()
{
}

/**
 * @brief Destructor
 *
 * @param _name
 */
void SalomePythonFileUtil::setFileName (QString _name)
{
    _fileName = _name;
}
/**
 * @brief This function stores the project path
 *
 * @param _path
 */
void SalomePythonFileUtil::setPathProject (QString _path)
{
    _pathProject = _path;
}

/**
 * @brief This function stores the template data of the Salome Template
 *
 * @param _data
 */
void SalomePythonFileUtil::setTemplateData(SalomeTemplateData _data)
{
    _templateData = _data;
}

/**
 * @brief This function stores the list of possible SalomeTemplateDAta
 *
 * @param _data
 */
void SalomePythonFileUtil::setTemplateDataList(QList<SalomeTemplateData> _data)
{
    _templateDataList = _data;
}

/**
 * @brief This funciton sotres the SalomeVariableData list associated to each template
 *
 * @param _list
 */
void SalomePythonFileUtil::setVariableList(QList< QList<SalomeVariableData> > _list)
{
    _variablesList = _list;
}

/**
 * @brief This funciton buillds the python file
 *
 */
void SalomePythonFileUtil::buildPythonFile()
{
    QString _pythonFilePath;
    QDir _phythonDir = QDir(_pathProject).filePath("Python");
    _pythonFilePath = _phythonDir.filePath(_fileName+".py");
    // Creating the folder structure
    if (!_phythonDir.exists())
        _phythonDir.mkpath(".");
    // Opening the file
    QFile * _file = new QFile(_pythonFilePath);
    _file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream  _textWriter(_file);

    buildHeader(&_textWriter);
    buildMain(&_textWriter);

    // Closing the file
    _file->close();
}

/**
 * @brief This funciton buillds the python file for the Optimization menu
 *
 * @param _iterations
 */
void SalomePythonFileUtil::buildPythonFileForOptimization(int _iterations)
{
    QString _pythonFilePath;
    QDir _phythonDir = QDir(_pathProject).filePath("Python");
    _pythonFilePath = _phythonDir.filePath(_fileName+"_opt.py");
    // Creating the folder structure
    if (!_phythonDir.exists())
        _phythonDir.mkpath(".");
    // Opening the file
    QFile * _file = new QFile(_pythonFilePath);
    _file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream  _textWriter(_file);

    buildHeader(&_textWriter);
    buildMainForOptimization(&_textWriter,_iterations);

    // Closing the file
    _file->close();
}



/**
 * @brief This funciton buillds the python file header
 *
 * @param _textWriter
 */
void SalomePythonFileUtil::buildHeader(QTextStream * _textWriter)
{

    QString _BBIPED_HOME = QCoreApplication::applicationDirPath();

    //----------------------------------------------
    // FOR TESTING REMOVE FOR RELEASE
    //_BBIPED_HOME = _BBIPED_HOME + "/debug";
    //----------------------------------------------
    (* _textWriter) << "# ------------------------------------------------------------" << endl;
    (* _textWriter) << "# HEADER  " << endl;
    (* _textWriter) << "# ------------------------------------------------------------" << endl;

    (* _textWriter) << "import os" << endl;
    (* _textWriter) << "import sys" << endl;

    (* _textWriter) << "_AutomatizationPath=\""+_BBIPED_HOME+"/.tools/Automatization/\"" << endl;
    (* _textWriter) << "sys.path.append(_AutomatizationPath)"<< endl;
    (* _textWriter) << "import JobManager" << endl;
    (* _textWriter) << "CurrentPath = os.getcwd()" << endl;

}
/**
 * @brief This funciton buillds the python file Main Body
 *
 * @param _textWriter
 */
void SalomePythonFileUtil::buildMain(QTextStream * _textWriter)
{
    QString _templateFullName = _templateData.getTemplateFileName();
    _templateFullName.remove("_Calculations");
    (* _textWriter) << "# ------------------------------------------------------------" << endl;
    (* _textWriter) << "# MAIN BODY  " << endl;
    (* _textWriter) << "# ------------------------------------------------------------" << endl;
    (* _textWriter) << "Geometry_Template = \""+_templateFullName+"\""<< endl;
    (* _textWriter) << "JobManager.Compute_Mesh(CurrentPath,Geometry_Template"+getVariablesFromTemplate()+")"<< endl;


}

/**
 * @brief This funciton buillds the python file Main Body for Optimization
 *
 * @param _textWriter
 * @param _iterations
 */
void SalomePythonFileUtil::buildMainForOptimization(QTextStream * _textWriter,int _iterations)
{
    QString _templateFullName = _templateData.getTemplateFileName();
    _templateFullName.remove("_Calculations");
    (* _textWriter) << "# ------------------------------------------------------------" << endl;
    (* _textWriter) << "# MAIN BODY  " << endl;
    (* _textWriter) << "# ------------------------------------------------------------" << endl;
    (* _textWriter) << "Geometry_Template = \""+_templateFullName+"\""<< endl;
    (* _textWriter) << "def CreateDirectory(WritePath):"<< endl;
    (* _textWriter) << "	if not os.path.exists(WritePath):"<< endl;
    (* _textWriter) << "		os.makedirs(WritePath)"<< endl;

    QString _basePath = QDir(_pathProject).filePath("Python");
    for (int i = 0; i< _iterations; i++)
    {
        QString _newFolder;
        _newFolder = QDir(_basePath).filePath("Iter_"+QString::number(i));

        (* _textWriter) << "CreateDirectory(\""+_newFolder+"\")"<< endl;
        (* _textWriter) << "WritePath =\""+_newFolder +"\""<< endl;
        (* _textWriter) << "JobManager.Compute_Mesh(WritePath,Geometry_Template"+getVariables(i)+")"<< endl;
    }


}

/**
 * @brief This funciton buillds get the variable Name in a specific position
 *
 * @param _pos
 * @return QString
 */
QString SalomePythonFileUtil::getVariables(int _pos)
{
    QString _variables = "";
    QList<SalomeVariableData> _varList;

    if (_pos < _variablesList.size())
        _varList = _variablesList.at(_pos);


    for (int i = 0; i < _varList.size();i++)
    {
        SalomeVariableData _var = _varList.at(i);
        _variables = _variables + ",\""+_var.getFileName()+"\","+_var.getCurrentValue();
    }

    return _variables;

}




/**
 * @brief This function gets the variables names in a QString format from a specifi template (variable name + current value)
 *
 * @return QString
 */
QString SalomePythonFileUtil::getVariablesFromTemplate()
{
    QString _variables = "";
    QList<SalomeVariableData> _varList = _templateData.getVariableList();
    for (int i = 0; i < _varList.size();i++)
    {
        SalomeVariableData _var = _varList.at(i);
        if (QString::compare(_var.getDefaultValue(),_var.getCurrentValue()) != StrEqual)
            _variables = _variables + ",\""+_var.getFileName()+"\","+_var.getCurrentValue();
    }


    return _variables;

}
