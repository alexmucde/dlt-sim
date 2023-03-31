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
 * \file dltcan.cpp
 * @licence end@
 */

#include "DLTTest.h"

#include <QDebug>
#include <QFile>
#include <QRandomGenerator>

DLTTest::DLTTest()
{
    clear();
}

DLTTest::~DLTTest()
{

}

DLTTest& DLTTest::operator=(const DLTTest &test)
{
    commands = test.commands;
    id = test.id;
    description = test.description;

    return *this;
}

void DLTTest::clear()
{
    commands.clear();
    id="";
    description="";
    repeat = 1;
    fail = "next";
}

QString DLTTest::getId() const
{
    return id;
}

void DLTTest::setId(const QString &value)
{
    id = value;
}

QString DLTTest::getDescription() const
{
    return description;
}

void DLTTest::setDescription(const QString &value)
{
    description = value;
}

int DLTTest::getRepeat() const
{
    return repeat;
}

void DLTTest::setRepeat(int value)
{
    repeat = value;
}

const QStringList &DLTTest::getCommands() const
{
    return commands;
}

const QString &DLTTest::getFail() const
{
    return fail;
}

void DLTTest::setFail(const QString &newFail)
{
    fail = newFail;
}


