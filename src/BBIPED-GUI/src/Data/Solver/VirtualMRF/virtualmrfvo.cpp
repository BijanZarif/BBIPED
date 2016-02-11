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
 *  Author: Dec 2016: Carmen Alonso Montes calonso@bcamath.org
***/
#include "virtualmrfvo.h"
#include "Defines/General/GeneralDefines.h"

/**
 * @brief Constructor of the class Virtual MRF
 *
 */
VirtualMrfVO::VirtualMrfVO()
{
    _coordinatesIn3D = false; // By default, coordinates of inner and outer curves will be in 2D
    _zoneName = "";
    _omega = "";
    _axisCenter.clear();
    _axisNormal.clear();
    _innerCurvePoints.clear();
    _outerCurvePoints.clear();
}


/**
 * @brief Get tbe name of the zone
 *
 * @return QString
 */
QString VirtualMrfVO::getZoneName()
{
    return _zoneName;
}

/**
 * @brief Get the omega
 *
 * @return QString
 */
QString VirtualMrfVO::getOmega()
{
     return _omega;
}

/**
 * @brief Get the axis center
 *
 * @return QStringList
 */
QStringList VirtualMrfVO::getAxisCenter()
{
     return _axisCenter;
}

/**
 * @brief get the Axis Normal
 *
 * @return QStringList
 */
QStringList VirtualMrfVO::getAxisNormal()
{
     return _axisNormal;
}

/**
 * @brief get Inner curve points (list)
 *
 * @return QStringList
 */
QStringList VirtualMrfVO::getInnerCurvePoints()
{
     return _innerCurvePoints;
}

/**
 * @brief get Outer curve points (list)
 *
 * @return QStringList
 */
QStringList VirtualMrfVO::getOuterCurvePoints()
{
     return _outerCurvePoints;
}


/**
 * @brief Set Zone name
 *
 * @param _val
 */
void  VirtualMrfVO::setZoneName(QString _val)
{
    _zoneName = _val;
}

/**
 * @brief Set omega
 *
 * @param _val
 */
void  VirtualMrfVO::setOmega(QString _val)
{
    _omega = _val;
}

/**
 * @brief Set Axis Center
 *
 * @param _list
 */
void  VirtualMrfVO::setAxisCenter(QStringList _list)
{
    _axisCenter = _list;
}

/**
 * @brief Set Axis Normal
 *
 * @param _list
 */
void  VirtualMrfVO::setAxisNormal(QStringList _list)
{
    _axisNormal = _list;
}

/**
 * @brief Set the list of inner curve points
 *
 * @param _list
 */
void  VirtualMrfVO::setInnerCurvePoints(QStringList _list)
{
    _innerCurvePoints = _list;
}

/**
 * @brief Set the list of outer curve points
 *
 * @param _list
 */
void  VirtualMrfVO::setOuterCurvePoints(QStringList _list)
{
    _outerCurvePoints = _list;
}



/**
 * @brief Check if all the elements are empty
 *
 * @return bool
 */
bool  VirtualMrfVO::isEmpty()
{
    bool _isEmpty = true;
    if (!_axisCenter.isEmpty()) _isEmpty = false;
    if (!_axisNormal.isEmpty()) _isEmpty = false;
    if (!_innerCurvePoints.isEmpty()) _isEmpty = false;
    if (!_outerCurvePoints.isEmpty()) _isEmpty = false;
    if (!_zoneName.isEmpty()) _isEmpty = false;
    if (!_omega.isEmpty()) _isEmpty = false;
    return _isEmpty;
}



/**
 * @brief True if the coordinates are in 3D, false otherwise
 *
 * @param _val
 */
void VirtualMrfVO::setCoordinatesIn3D(bool _val)
{
    _coordinatesIn3D = _val;
}

/**
 * @brief Returns if the coordinates are set in 3D (true), otherwise is false.
 *
 * @return bool
 */
bool VirtualMrfVO::isCoordinatesIn3D()
{
    return _coordinatesIn3D ;
}
