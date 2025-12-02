#pragma once

#include <QFutureWatcher>
#include <QObject>
#include <QPointF>
#include <QUrl>
#include <QVariantList>

#include "geometry.hpp"


class HullController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList pointsModel READ pointsModel NOTIFY pointsModelChanged)
    Q_PROPERTY(QVariantList hullModel READ hullModel NOTIFY hullModelChanged)
    Q_PROPERTY(double gamma READ gamma WRITE setGamma NOTIFY gammaChanged)

public:
    explicit HullController(QObject *parent = nullptr);

    QVariantList pointsModel() const;
    QVariantList hullModel() const;
    double gamma() const;

public slots:
    void loadPointsFromFile(const QUrl &fileUrl);
    void saveHullToFile(const QUrl &fileUrl);
    void addPoint(double x, double y);
    void setGamma(double newGamma);
    void clear();

signals:
    void pointsModelChanged();
    void hullModelChanged();
    void gammaChanged();
    void computationFinished();

private slots:
    void handleComputationFinished();

private:
    void recomputeHull();

    std::vector<task3::geometry::Point> m_points;
    std::vector<task3::geometry::Point> m_hull;
    double m_gamma = 1.0;

    QFutureWatcher<std::vector<task3::geometry::Point>> m_hullWatcher;
};
