/**
 * @licence app begin@
 * Copyright (C) 2021 Alexander Wenzel
 *
 * This file is part of the DLT Multimeter project.
 *
 * \copyright This code is licensed under GPLv3.
 *
 * \author Alexander Wenzel <alex@eli2.de>
 *
 * \file dltcan.h
 * @licence end@
 */

#ifndef DLT_TEST_H
#define DLT_TEST_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTcpSocket>
#include <QTimer>

class DLTTest
{
public:
    DLTTest();
    ~DLTTest();

    DLTTest& operator=(const DLTTest &test);

    void clear();

    QString getId() const;
    void setId(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    int size() { return commands.size(); }

    void append(const QString &text) {  commands.append(text); }
    QString at(int num) { return commands[num]; }

    int getRepeat() const;
    void setRepeat(int value);

    const QStringList &getCommands() const;

    const QString &getFail() const;
    void setFail(const QString &newFail);

private:
    QString id;
    QString fail;
    QString description;
    QStringList commands;
    int repeat;
};


#endif // DLT_TEST_H
