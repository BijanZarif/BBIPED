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
** ** Modified and adapted version (BBIPED GUI - 2015)
****************************************************************************/
#include <QtXml>

#include "domitem.h"


/**
 * @brief Cosntructor Item element of the DOM tree
 *
 * @param node
 * @param row
 * @param parent
 */
DomItem::DomItem(QDomNode &node, int row, DomItem *parent)
{
    domNode = node;

    // Record the item's location within its parent.
    rowNumber = row;
    parentItem = parent;
}



/**
 * @brief Destructor
 *
 */
DomItem::~DomItem()
{
    QHash<int,DomItem*>::iterator it;
    for (it = childItems.begin(); it != childItems.end(); ++it)
        delete it.value();
}



/**
 * @brief Node of the DOM item
 *
 * @return QDomNode
 */
QDomNode DomItem::node() const
{
    return domNode;
}



/**
 * @brief Parent of the DOM item
 *
 * @return DomItem
 */
DomItem *DomItem::parent()
{
    return parentItem;
}



/**
 * @brief Child of the DOM item
 *
 * @param i
 * @return DomItem
 */
DomItem *DomItem::child(int i)
{
    if (childItems.contains(i))
        return childItems[i];

    if (i >= 0 && i < domNode.childNodes().count()) {
        QDomNode childNode = domNode.childNodes().item(i);
        DomItem *childItem = new DomItem(childNode, i, this);
        childItems[i] = childItem;
        return childItem;
    }
    return 0;
}



/**
 * @brief Row number of the DOM item
 *
 * @return int
 */
int DomItem::row()
{
    return rowNumber;
}

