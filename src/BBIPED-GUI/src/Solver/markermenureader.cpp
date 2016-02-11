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
 *  *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org

***/
#include "markermenureader.h"
#include <QFile>
#include <QDomNode>
#include <QDomElement>
#include <QDomNodeList>

#include "Defines/Tool/Solver/MarkerDefines.h"

/**
 * @brief Constructors
 *
 */
MarkerMenuReader::MarkerMenuReader()
{
    _isInitialized = false;
}

MarkerMenuReader::~MarkerMenuReader()
{
    if (_isInitialized)
        delete _domtree;

}
/*************************************************** GETTER/ SETTER ***********************************/
/**
 * @brief Get the Boundary/Marker menu list
 *
 * @return QList<SolverMarkerMenu>
 */
QList<SolverMarkerMenu> MarkerMenuReader::getMarkerMenuList()
{
     return _markerMenuList;
}



/**
 * @brief Function that gets a Marker node name from the DOM XML file
 *
 * @param _element
 * @param tagName
 * @return QString
 */
QString MarkerMenuReader::getMarkerName(QDomElement _element, QString tagName)
{
    QDomNodeList _listName = _element.elementsByTagName(tagName);
    if (!_listName.isEmpty())
        return (_listName.at(0).toElement().text());
    return QString("");
}


/*************************************************** Build ***********************************/

/**
 * @brief Function that builds the Marker Menu Nodes to be shown in the Tree view
 *
 */
void MarkerMenuReader::buildAllMarkerMenuNodes()
{
    QFile * _templateConfigFile = new QFile(MARKER_XML_TEMPLATE);
    _domtree = new QDomDocument();

    if( !_templateConfigFile->open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug( "Failed to open file for reading." );
    }
    else
    {
        if( !_domtree->setContent( _templateConfigFile ) )
        {
            qDebug( "Failed to parse the file into a DOM tree." );
            _templateConfigFile->close();
        }
          _templateConfigFile->close();
           loadAllFile();
    }
    _templateConfigFile->close();
    delete _templateConfigFile;
    _isInitialized = true;
}

/*************************************************** Load ***********************************/

/**
 * @brief Function that loads the xml file and gets all the nodes of the menu
 *
 */
void MarkerMenuReader::loadAllFile()
{
    QDomElement _rootElement = _domtree->documentElement();
    if (!_rootElement.isNull())
    {
        QDomNodeList _list = _rootElement.elementsByTagName(XML_MARKER_NODE_TAG_NAME);
        if (!_list.isEmpty())
            for (int i = 0; i < _list.count();i++)
            {
                SolverMarkerMenu _node = createMarkerNode(_list.at(i));
                _markerMenuList.append(_node);
            }

    }
}

/*************************************************** Create ***********************************/

/**
 * @brief Function that creates a Marker node from the DOM XML file
 *
 * @param _node
 * @return SolverMarkerMenu
 */
SolverMarkerMenu MarkerMenuReader::createMarkerNode(QDomNode _node)
{
    SolverMarkerMenu _markerVO;
    QDomElement _markerEl = _node.toElement();
    if (!_markerEl.isNull())
    {
        _markerVO.setMarkerTitle(getMarkerName(_markerEl,XML_MARKER_NAME_TAG_NAME));
        _markerVO.setMarkerFileName(getMarkerName(_markerEl,XML_MARKER_FILENAME_TAG_NAME));
        _markerVO.setParametersList(createParameterList(_markerEl));
        _markerVO.setTypeList(createTypeList(_markerEl));
        _markerVO.setUnitList(createUnitList(_markerEl));
        _markerVO.setDefaultValueList(createDefaultValueList(_markerEl));
    }
    return _markerVO;
}

QStringList MarkerMenuReader::createDefaultValueList(QDomElement _element)
{
    QDomNodeList _listParameter =
            _element.elementsByTagName(XML_MARKER_PARAMETER_LIST_TAG_NAME);
    QStringList _result;
    if (!_listParameter.isEmpty())
    {
        QDomNodeList _childNodes = _listParameter.at(0).childNodes();
        for (int i = 0; i < _childNodes.count();i++)
        {
            QDomElement _nodeEl = _childNodes.at(i).toElement();
            if (_nodeEl.hasAttribute(XML_MARKER_DEFAULT_VALUE_ATT))
                _result.append(_nodeEl.attribute(XML_MARKER_DEFAULT_VALUE_ATT));
        }
    }
    return _result;

}

/**
 * @brief Function that creates the Parameter List from the DOM XML file
 *
 * @param _element
 * @return QStringList
 */
QStringList MarkerMenuReader::createParameterList(QDomElement _element)
{
    QDomNodeList _listParameter =
            _element.elementsByTagName(XML_MARKER_PARAMETER_LIST_TAG_NAME);
    QStringList _result;
    if (!_listParameter.isEmpty())
    {
        QDomNodeList _childNodes = _listParameter.at(0).childNodes();
        for (int i = 0; i < _childNodes.count();i++)
            _result.append(_childNodes.at(i).toElement().text());
    }
    return _result;
}



/**
 * @brief  Function that creates the Unit type List from the DOM XML file
 *
 * @param _element
 * @return QStringList
 */
QStringList MarkerMenuReader::createUnitList(QDomElement _element)
{
    QDomNodeList _listParameter =
            _element.elementsByTagName(XML_MARKER_PARAMETER_LIST_TAG_NAME);
    QStringList _result;
    if (!_listParameter.isEmpty())
    {
        QDomNodeList _childNodes = _listParameter.at(0).childNodes();
        for (int i = 0; i < _childNodes.count();i++)
        {
            QDomElement _nodeEl = _childNodes.at(i).toElement();
            if (_nodeEl.hasAttribute(XML_MARKER_UNIT_ATT))
                _result.append(_nodeEl.attribute(XML_MARKER_UNIT_ATT));
        }
    }
    return _result;
}


/**
 * @brief Function that creates a the type list from the DOM XML file
 *
 * @param _element
 * @return QStringList
 */
QStringList MarkerMenuReader::createTypeList(QDomElement _element)
{
    QDomNodeList _listParameter =
            _element.elementsByTagName(XML_MARKER_PARAMETER_LIST_TAG_NAME);
    QStringList _result;
    if (!_listParameter.isEmpty())
    {
        QDomNodeList _childNodes = _listParameter.at(0).childNodes();
        for (int i = 0; i < _childNodes.count();i++)
        {
            QDomElement _nodeEl = _childNodes.at(i).toElement();
            if (_nodeEl.hasAttribute(XML_MARKER_TYPE_ATT))
                _result.append(_nodeEl.attribute(XML_MARKER_TYPE_ATT));
        }
    }
    return _result;
}
