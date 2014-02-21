#ifndef SCREENIESCENESERIALISER_H
#define SCREENIESCENESERIALISER_H

#include <QByteArray>

class ScreenieScene;

class ScreenieSceneSerializer
{
public:
    enum class Mode {
        FullScene,
        SelectedItems
    };

    virtual ~ScreenieSceneSerializer() {}

    virtual QByteArray serialize(const ScreenieScene &screenieScene, Mode mode) = 0;

    virtual ScreenieScene *deserialize(QByteArray &data) const = 0;
};

#endif // SCREENIESCENESERIALISER_H
