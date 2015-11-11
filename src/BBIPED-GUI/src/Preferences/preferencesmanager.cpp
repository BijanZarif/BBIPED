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
***/
#include "preferencesmanager.h"

#include "../Defines/General/GeneralDefines.h"
#include "../Defines/General/UserPreferencesDefines.h"
#include <QFile>
#include <QString>
#include <QDir>
#include <QCoreApplication>

/**
 * @brief
 *
 */
PreferencesManager::PreferencesManager()
{
}



/**
 * @brief
 *
 * @return UserPreferencesData
 */
UserPreferencesData PreferencesManager::getUserPreferences()
{
    UserPreferencesData _userData;
    readPreferencesFile(&_userData);
    return _userData;
}

/************************************** WRITE ****************************************/

/**
 * @brief
 *
 * @param _newPreferences
 */
void PreferencesManager::saveUserPreferences(UserPreferencesData _newPreferences)
{
    QString _cPath = QDir(QCoreApplication::applicationDirPath()).filePath(PREFERENCE_FILE_NAME);
    QFile _file(_cPath);
    bool _fileCreated = false;
    if (_file.exists())
        _file.remove();

     _fileCreated = _file.open(QIODevice::ReadWrite);
    if (_fileCreated)
    {
        QXmlStreamWriter xmlWriter(&_file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        // Writing the root element
        xmlWriter.writeStartElement(XML_PREFERENCES_BBIPED_SESSION_ROOT_TAG);

        // Writing the recent projects
        writeRecentProjects(&xmlWriter,_newPreferences);
        // Writing the user preferences
        writeUserPreferences(&xmlWriter,_newPreferences);

        // Writing the Warning Preferences
        writeWarningPreferences(&xmlWriter,_newPreferences);

        // Writing the Template Preferences
        writeTemplatePreferences(&xmlWriter,_newPreferences);

        // Ending the root element
        xmlWriter.writeEndElement();

        // Ending the document
        xmlWriter.writeEndDocument();

    }
    _file.close();
}

/**
 * @brief
 *
 * @param _xmlWriter
 * @param _userData
 */
void PreferencesManager::writeWarningPreferences(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData)
{
    _xmlWriter->writeStartElement(XML_PREFERENCES_WARNING_TAG);
        QString _value("YES");
        if (!_userData.showCloseWarning())
            _value = "NO";
        _xmlWriter->writeStartElement(XML_PREFERENCES_WARNING_CLOSE_APP_TAG);
        _xmlWriter->writeAttribute(XML_PREFERENCES_WARNING_CLOSE_APP_SHOW_ATT,_value);
        _xmlWriter->writeEndElement();

        _value = "YES";
        if (!_userData.showCloseWarning())
            _value = "NO";
        _xmlWriter->writeStartElement(XML_PREFERENCES_WARNING_CLOSE_PROJECT_TAG);
        _xmlWriter->writeAttribute(XML_PREFERENCES_WARNING_CLOSE_PROJECT_SHOW_ATT,_value);
        _xmlWriter->writeEndElement();
    _xmlWriter->writeEndElement();
}

/**
 * @brief
 *
 * @param _xmlWriter
 * @param _userData
 */
void PreferencesManager::writeTemplatePreferences(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData)
{
    _xmlWriter->writeStartElement(XML_PREFERENCES_TEMPLATE_TAG);
        _xmlWriter->writeAttribute(XML_PREFERENCES_TEMPLATE_PATH_ATT,_userData.getTemplateConfigPath());
        _xmlWriter->writeEndElement();
    _xmlWriter->writeEndElement();
}


/**
 * @brief
 *
 * @param _xmlWriter
 * @param _userData
 */
void PreferencesManager::writeRecentProjects(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData)
{
    _xmlWriter->writeStartElement(XML_PREFERENCES_RECENTPROJECTS_TAG);
    QStringList _listProjects = _userData.getRecentProjectsList();
    if (!_listProjects.isEmpty())
        for (int i = 0; i < _listProjects.size();i++)
        {
            _xmlWriter->writeStartElement(XML_PREFERENCES_PROJECT_TAG);
            _xmlWriter->writeAttribute(XML_PREFERENCES_PROJECT_PATH_ATT_TAG,_listProjects.at(i));
            _xmlWriter->writeEndElement();

        }
    _xmlWriter->writeEndElement();
}

/**
 * @brief
 *
 * @param _xmlWriter
 * @param _userData
 */
void PreferencesManager::writeUserPreferences(QXmlStreamWriter * _xmlWriter, UserPreferencesData  _userData)
{
    _xmlWriter->writeStartElement(XML_PREFERENCES_USER_PREFERENCES_TAG);

    // Writing the Mesh Viewer Information
        _xmlWriter->writeStartElement(XML_PREFERENCES_USER_PREFERENCES_MESH_VIEWER_TAG);
         _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_MESH_VIEWER_PATH_ATT_TAG,_userData.getMeshViewerPath());
        _xmlWriter->writeEndElement();

    // Writing the PostProcessor Viewer Information
        _xmlWriter->writeStartElement(XML_PREFERENCES_USER_PREFERENCES_POSTPROCESSOR_VIEWER_TAG);
         _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_POSTPROCESSOR_VIEWER_PATH_ATT_TAG,_userData.getPostProcessorViewerPath());
        _xmlWriter->writeEndElement();

    // Writing the Solver Engine Information
        _xmlWriter->writeStartElement(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_TAG);
            _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PATH_ATT_TAG,_userData.getUserDefinedSolverPath());
            QString _virtualValue = "NO";
            if (_userData.isVirtualSolverEngineSelected())
                _virtualValue = "YES";
            _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_VIRTUAL_ATT_TAG,_virtualValue);

            QString _multizoneValue = "NO";
            if (_userData.isMultizoneSolverEngineSelected())
                _multizoneValue = "YES";
            _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_MULTIZONE_ATT_TAG,_multizoneValue);

            if (!_userData.isDefaultParallelEngineSelected())
                _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PARALLEL_ATT_TAG,_userData.getCustomizedParallelEnginePath());
            if (!_userData.isDefaultPartitionEngineSelected())
                _xmlWriter->writeAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PARTITION_ATT_TAG,_userData.getCustomizedPartitionEnginePath());

        _xmlWriter->writeEndElement();

    _xmlWriter->writeEndElement();
}


/************************************** READ ****************************************/
/**
 * @brief
 *
 * @param _userData
 */
void PreferencesManager::readPreferencesFile(UserPreferencesData * _userData)
{
    QString _cPath = QDir(QCoreApplication::applicationDirPath()). filePath(PREFERENCE_FILE_NAME);
    QFile _file(_cPath);
    bool _fileExist = true;

    _fileExist = _file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (_fileExist)
    {
        QXmlStreamReader _xmlReader(&_file);
        getRecentProjects(&_xmlReader,_userData);
        getUserToolPreferences(&_xmlReader,_userData);
        getUserWarningPreferences(&_xmlReader,_userData);
        getUserTemplatePreferences(&_xmlReader,_userData);
    }else
        saveUserPreferences(*_userData);

    _file.close();
}

/**
 * @brief
 *
 * @param _xmlReader
 * @param _userData
 */
void PreferencesManager::getUserTemplatePreferences(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData)
{
    bool _finish = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_PREFERENCES_TEMPLATE_TAG ) == StrEqual))
        {
            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_TEMPLATE_PATH_ATT))
                _userData->setTemplateConfigPath(_xmlReader->attributes().value(XML_PREFERENCES_TEMPLATE_PATH_ATT).toString());

        }else{
            if ((QString::compare(_xmlReader->name().toString(),XML_PREFERENCES_TEMPLATE_TAG) == StrEqual) &&
                    (token == QXmlStreamReader::EndElement))
                _finish = true;
        }
    }
}

/**
 * @brief
 *
 * @param _xmlReader
 * @param _userData
 */
void PreferencesManager::getUserWarningPreferences(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData)
{
    bool _finish = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_PREFERENCES_WARNING_TAG ) == StrEqual))
        {
            _userData->setShowCloseWarning(getWarningPreferences(_xmlReader,XML_PREFERENCES_WARNING_CLOSE_APP_TAG,
                                                              XML_PREFERENCES_WARNING_CLOSE_APP_SHOW_ATT) );
            _userData->setShowCloseProjectWarning(getWarningPreferences(_xmlReader,XML_PREFERENCES_WARNING_CLOSE_PROJECT_TAG,
                                                                     XML_PREFERENCES_WARNING_CLOSE_PROJECT_SHOW_ATT));

        }else{
            if ((QString::compare(_xmlReader->name().toString(),XML_PREFERENCES_WARNING_TAG) == StrEqual) &&
                    (token == QXmlStreamReader::EndElement))
                _finish = true;
        }
    }
}


/**
 * @brief
 *
 * @param _xmlReader
 * @param _warningType
 * @param _warningAtt
 * @return bool
 */
bool PreferencesManager::getWarningPreferences(QXmlStreamReader *_xmlReader, QString _warningType,
                                               QString _warningAtt)
{
    bool _finish = false;
    bool _showWarning = true;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), _warningType ) == StrEqual))
        {
            if (_xmlReader->attributes().hasAttribute(_warningAtt))
            {
                if (QString::compare(_xmlReader->attributes().value(_warningAtt).toString(), "NO") == StrEqual)
                    _showWarning = false;
            }
        }else
        {
            if ((QString::compare(_xmlReader->name().toString(),_warningType) == StrEqual) &&
                    (token == QXmlStreamReader::EndElement))
                _finish = true;
        }

    }
    return _showWarning;
}

/**
 * @brief
 *
 * @param _xmlReader
 * @param _userData
 */
void PreferencesManager::getRecentProjects(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData)
{
    bool _finish = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument) continue;
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_PREFERENCES_RECENTPROJECTS_TAG ) == StrEqual))
        {
            _userData->setRecentProjectsList(getProjectList(_xmlReader));
            _finish = true;
        }

    }
}

/**
 * @brief
 *
 * @param _xmlReader
 * @return QStringList
 */
QStringList PreferencesManager::getProjectList(QXmlStreamReader *_xmlReader)
{
    bool _finish = false;
    QStringList _resultList;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_PREFERENCES_PROJECT_TAG ) == StrEqual))
        {
            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_PROJECT_PATH_ATT_TAG))
                _resultList.append(_xmlReader->attributes().value(XML_PREFERENCES_PROJECT_PATH_ATT_TAG).toString());
        }else
        {
            if  ((QString::compare(_xmlReader->name().toString(),XML_PREFERENCES_RECENTPROJECTS_TAG) == StrEqual)
                 && (token == QXmlStreamReader::EndElement))
                _finish = true;
        }

    }

    return _resultList;
}


/**
 * @brief
 *
 * @param _xmlReader
 * @param _userData
 */
void PreferencesManager::getUserToolPreferences(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData)
{
    bool _finish = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_PREFERENCES_USER_PREFERENCES_TAG ) == StrEqual))
        {
            _userData->setMeshViewerPath(getViewerInformation(_xmlReader,XML_PREFERENCES_USER_PREFERENCES_MESH_VIEWER_TAG,
                                                              XML_PREFERENCES_USER_PREFERENCES_MESH_VIEWER_PATH_ATT_TAG));
            _userData->setPostProcessorViewerPath(getViewerInformation(_xmlReader,
                                                                       XML_PREFERENCES_USER_PREFERENCES_POSTPROCESSOR_VIEWER_TAG,
                                                                       XML_PREFERENCES_USER_PREFERENCES_POSTPROCESSOR_VIEWER_PATH_ATT_TAG));
            setSolverEngineInformation(_xmlReader,_userData);
        }else
        {
            if ((QString::compare(_xmlReader->name().toString(),XML_PREFERENCES_USER_PREFERENCES_TAG) == StrEqual) &&
                    (token == QXmlStreamReader::EndElement))
                _finish = true;
        }

    }
}

/**
 * @brief
 *
 * @param _xmlReader
 * @param _keyName
 * @param _attName
 * @return QString
 */
QString PreferencesManager::getViewerInformation(QXmlStreamReader *_xmlReader, QString _keyName, QString _attName)
{
    QString _result;
    bool _finish = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), _keyName ) == StrEqual))
        {
            if (_xmlReader->attributes().hasAttribute(_attName))
                _result = _xmlReader->attributes().value(_attName).toString();
        }else
        {
            if ( (QString::compare(_xmlReader->name().toString(),_keyName) == StrEqual) &&
                    (token == QXmlStreamReader::EndElement))
                _finish = true;
        }

    }
    return _result;

}

/**
 * @brief
 *
 * @param _xmlReader
 * @param _userData
 */
void PreferencesManager::setSolverEngineInformation(QXmlStreamReader *_xmlReader, UserPreferencesData * _userData)
{
    bool _finish = false;
    while ( (!_xmlReader->atEnd()) && (!_xmlReader->hasError()) && (!_finish))
    {
        QXmlStreamReader::TokenType token = _xmlReader->readNext();
        if ( (token == QXmlStreamReader::StartElement) &&
            (QString::compare(_xmlReader->name().toString(), XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_TAG ) == StrEqual))
        {

            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PATH_ATT_TAG))
            {
                 QString _solverEnginePath =  _xmlReader->attributes().value(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PATH_ATT_TAG).toString();
                 if ( (!_solverEnginePath.isEmpty()) && (QString::compare(_solverEnginePath,"") != StrEqual) )
                    _userData->setUserDefinedSolverPath(_solverEnginePath);
            }
            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_VIRTUAL_ATT_TAG) )
            {
                QString  _valueSolverVirtualEngine =  _xmlReader->attributes().value(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_VIRTUAL_ATT_TAG).toString();
                if (QString::compare(_valueSolverVirtualEngine,"YES") == StrEqual)
                    _userData->setVirtualSolverEngineSelected(true);
            }
            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_MULTIZONE_ATT_TAG) )
            {
                QString  _valueSolverMultizoneEngine =  _xmlReader->attributes().value(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_MULTIZONE_ATT_TAG).toString();
                if (QString::compare(_valueSolverMultizoneEngine,"YES") == StrEqual)
                    _userData->setMultizoneSolverEngineSelected(true);
            }
            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PARALLEL_ATT_TAG) )
            {
                QString  _valueSolverParallelEngine =  _xmlReader->attributes().value(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PARALLEL_ATT_TAG).toString();
               if ( (!_valueSolverParallelEngine.isEmpty()) && (QString::compare(_valueSolverParallelEngine,"") != StrEqual) )
               {
                    _userData->setCustomizedParallelEnginePath(_valueSolverParallelEngine);
                    _userData->setDefaultParallelEngineSelected(false);
               }else
                   _userData->setDefaultParallelEngineSelected(true);
            }
            if (_xmlReader->attributes().hasAttribute(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PARTITION_ATT_TAG) )
            {
                QString  _valueSolverPartitionEngine =  _xmlReader->attributes().value(XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_PARTITION_ATT_TAG).toString();
               if ( (!_valueSolverPartitionEngine.isEmpty()) && (QString::compare(_valueSolverPartitionEngine,"") != StrEqual) )
               {
                    _userData->setCustomizedPartitionEnginePath(_valueSolverPartitionEngine);
                    _userData->setDefaultPartitionEngineSelected(false);
               }else
                   _userData->setDefaultPartitionEngineSelected(true);
            }

        }else
        {
            if ( (QString::compare(_xmlReader->name().toString(),XML_PREFERENCES_USER_PREFERENCES_SOLVER_ENGINE_TAG) == StrEqual) &&
                    (token == QXmlStreamReader::EndElement))
                _finish = true;
        }

    }
}
