/****************************************************************************
**   nodecast-worker is a bot worker, part of the backend of nodecast.net
**   Copyright (C) 2010-2011  Frédéric Logier <frederic@logier.org>
**
**   http://gitorious.org/nodecast/nodecast-worker
**
**   This program is free software: you can redistribute it and/or modify
**   it under the terms of the GNU Affero General Public License as
**   published by the Free Software Foundation, either version 3 of the
**   License, or (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**   GNU Affero General Public License for more details.
**
**   You should have received a copy of the GNU Affero General Public License
**   along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/


#include "stats.h"

Stats::Stats(Nosql& a, QObject *parent) : nosql_(a), QObject(parent)
{}


Stats::~Stats()
{}


void Stats::s_job_receive(std::string data)
{
    qDebug() << "Stats::s_mother_job_receive";
}

