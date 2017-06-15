#ifndef LYRMANAGER_H
#define LYRMANAGER_H

#include <QObject>

class LyrManager : public QObject
{
    Q_OBJECT
public:
    explicit LyrManager(QObject *parent = 0);

signals:

public slots:
};

#endif // LYRMANAGER_H