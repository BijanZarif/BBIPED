/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
** Modified and adapted version (BBIPED GUI - 2015)
****************************************************************************/
#include "dommodel.h"
#include <QtGui>
#include <QtXml>

#include "domitem.h"

#include "Defines/Tool/Solver/SolverDefines.h"
#include "Defines/General/GeneralDefines.h"

/**
 * @brief Specific DOM model constructor
 *
 * @param document
 * @param parent
 */
DomModel::DomModel(QDomDocument document, QObject *parent)
    : QAbstractItemModel(parent), domDocument(document)
{
    rootItem = new DomItem(domDocument, 0);
}



/**
 * @brief Destructor
 *
 */
DomModel::~DomModel()
{
    delete rootItem;
}


/**
 * @brief Number of columns
 *
 * @param
 * @return int
 */
int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}



/**
 * @brief Data contained in the DOM Tree
 *
 * @param index
 * @param role
 * @return QVariant
 */
QVariant DomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();



    DomItem *item = static_cast<DomItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
        return QVariant(item->node().nodeName());

    QDomNode node = item->node();
    QString _nodeName;

    _nodeName = node.nodeName();
    QStringList attributes;
    QDomNamedNodeMap attributeMap = node.attributes();
    // TODO ... change what we show
    switch (index.column()) {
        case 0:
            return node.nodeName();
       case 1:
            for (int i = 0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + "=\""
                              +attribute.nodeValue() + "\"";
            }
            return attributes.join(" ");
        case 2:
            return node.nodeValue().split("\n").join(" ");

        default:
            return QVariant();
    }
}



/**
 * @brief Activated flags
 *
 * @param index
 * @return Qt::ItemFlags
 */
Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/**
 * @brief Header Data
 *
 * @param section
 * @param orientation
 * @param role
 * @return QVariant
 */
QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Attributes");
            case 2:
                return tr("Value");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

/**
 * @brief Index within the DOM tree
 *
 * @param row
 * @param column
 * @param parent
 * @return QModelIndex
 */
QModelIndex DomModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

/**
 * @brief Parent within DOM tree
 *
 * @param child
 * @return QModelIndex
 */
QModelIndex DomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

/**
 * @brief Row count function
 *
 * @param parent
 * @return int
 */
int DomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

