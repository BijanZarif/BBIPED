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
#include "projectdetails.h"

/*****************************************************************************************/
/**
 * @brief
 *
 */
ProjectDetails::ProjectDetails()
{
    // Project Details
    _projectTile = "";
    _author = "";
    _startingDate  = "";
    _otherDetails  = "";
}

/*********************************** GETTER/SETTER/IS *******************************/
/**
 * @brief This function gets the project title
 *
 * @return QString
 */
QString ProjectDetails::getProjectTile()
{
    return _projectTile;
}

/**
 * @brief This function sets the project title
 *
 * @param _title
 */
void ProjectDetails::setProjectTile(QString _title)
{
    _projectTile = _title;
}

/**
 * @brief This function gets the author name
 *
 * @return QString
 */
QString ProjectDetails::getAuthor()
{
    return _author;
}

/**
 * @brief This function sets the author name
 *
 * @param _newAuthor
 */
void ProjectDetails::setAuthor(QString _newAuthor)
{
    _author = _newAuthor;
}

/**
 * @brief This function gets the starting data added by the user
 *
 * @return QString
 */
QString ProjectDetails::getStartingDate()
{
    return _startingDate;
}

/**
 * @brief This function sets the starting data added by the user
 *
 * @param _newDate
 */
void ProjectDetails::setStartingDate(QString _newDate)
{
    _startingDate = _newDate;
}

/**
 * @brief This function get the other details field
 *
 * @return QString
 */
QString ProjectDetails::getOtherDetails()
{
    return _otherDetails;
}

/**
 * @brief This function set the other details field
 *
 * @param _newDetails
 */
void ProjectDetails::setOtherDetails(QString _newDetails)
{
    _otherDetails = _newDetails;

}


/********************************** CLEAR ******************************************/
/**
 * @brief This function clear all the values of the object
 *
 */
void ProjectDetails::clear()
{
    _projectTile = "";
    _author = "";
    _startingDate  = "";
    _otherDetails  = "";
}


/**
 * @brief This function is to update the data from another object
 *
 * @param _newProjectDetails
 */
void ProjectDetails::updateProjectDetails(ProjectDetails _newProjectDetails)
{
    setProjectTile(_newProjectDetails.getProjectTile());
    setAuthor(_newProjectDetails.getAuthor());
    setStartingDate(_newProjectDetails.getStartingDate());
    setOtherDetails(_newProjectDetails.getOtherDetails());
}
