/****************************************************************************
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
** 
** This file is part of a Qt Solutions component.
**
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact Nokia at qt-info@nokia.com.
** 
****************************************************************************/


// Declaration of the QWinWidget classes

#ifndef QWINWIDGET_H
#define QWINWIDGET_H

#include <QtGui/QWidget>
#include "qmfcapp.h"

class CWnd;

#if defined(Q_WS_WIN)
#  if !defined(QT_QTWINMIGRATE_EXPORT) && !defined(QT_QTWINMIGRATE_IMPORT)
#    define QT_QTWINMIGRATE_EXPORT
#  elif defined(QT_QTWINMIGRATE_IMPORT)
#    if defined(QT_QTWINMIGRATE_EXPORT)
#      undef QT_QTWINMIGRATE_EXPORT
#    endif
#    define QT_QTWINMIGRATE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTWINMIGRATE_EXPORT)
#    undef QT_QTWINMIGRATE_EXPORT
#    define QT_QTWINMIGRATE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTWINMIGRATE_EXPORT
#endif

class QT_QTWINMIGRATE_EXPORT QWinWidget : public QWidget
{
    Q_OBJECT
public:
    QWinWidget( HWND hParentWnd, QObject *parent = 0, Qt::WFlags f = 0 );
#ifdef QTWINMIGRATE_WITHMFC
    QWinWidget( CWnd *parnetWnd, QObject *parent = 0, Qt::WFlags f = 0 );
#endif
    ~QWinWidget();

    void show();
    void center();
    void showCentered();

    HWND parentWindow() const;

protected:
    void childEvent( QChildEvent *e );
    bool eventFilter( QObject *o, QEvent *e );

    bool focusNextPrevChild(bool next);
    void focusInEvent(QFocusEvent *e);

    bool winEvent(MSG *msg, long *result);

private:
    void init();

    void saveFocus();
    void resetFocus();

    HWND hParent;
    HWND prevFocus;
    bool reenable_parent;
};

#endif // QWINWIDGET_H
