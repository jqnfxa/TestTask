#include <QFile>
#include <QTextStream>
#include <QtConcurrent/QtConcurrent>
#include "hull_controller.h"
#include "concave_hull.hpp"


HullController::HullController(QObject *parent)
    : QObject(parent)
{
    connect(&m_hullWatcher, &QFutureWatcher<std::vector<task3::geometry::Point>>::finished,
            this, &HullController::handleComputationFinished);
}

QVariantList HullController::pointsModel() const
{
    QVariantList model;
    for (const auto &p : m_points)
    {
        model.append(QPointF(p.x, p.y));
    }

    return model;
}

QVariantList HullController::hullModel() const
{
    QVariantList model;
    for (const auto &p : m_hull)
    {
        model.append(QPointF(p.x, p.y));
    }

    return model;
}

double HullController::gamma() const
{
    return m_gamma;
}

void HullController::loadPointsFromFile(const QUrl &fileUrl)
{
    QFile file(fileUrl.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Could not open file for reading");
        return;
    }

    m_points.clear();
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ", QString::SkipEmptyParts);
        if (parts.size() == 2)
        {
            m_points.push_back({parts[0].toDouble(), parts[1].toDouble()});
        }
    }

    emit pointsModelChanged();
    recomputeHull();
}

void HullController::saveHullToFile(const QUrl &fileUrl)
{
    QFile file(fileUrl.toLocalFile());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning("Could not open file for writing");
        return;
    }

    QTextStream out(&file);
    out.setRealNumberPrecision(6);
    for (const auto &p : m_hull)
    {
        out << p.x << " " << p.y << "\n";
    }
}

void HullController::addPoint(double x, double y)
{
    m_points.push_back({x, y});
    emit pointsModelChanged();
    recomputeHull();
}

void HullController::setGamma(double newGamma)
{
    if (qFuzzyCompare(m_gamma, newGamma))
    {
        return;
    }

    m_gamma = newGamma;
    emit gammaChanged();
    recomputeHull();
}

void HullController::clear()
{
    m_points.clear();
    m_hull.clear();
    emit pointsModelChanged();
    emit hullModelChanged();
}

void HullController::recomputeHull()
{
    if (m_points.size() < 3)
    {
        m_hull.clear();
        emit hullModelChanged();
        return;
    }

    QFuture<std::vector<task3::geometry::Point>> future = QtConcurrent::run(
        task3::algorithm::compute_concave_hull,
        m_points,
        m_gamma
    );
    m_hullWatcher.setFuture(future);
}

void HullController::handleComputationFinished()
{
    m_hull = m_hullWatcher.result();
    emit hullModelChanged();
    emit computationFinished();
}
