#ifndef TEMPLATEORGANIZER_H
#define TEMPLATEORGANIZER_H

#include <QObject>
#include <QList>

class ScreenieScene;
class ScreenieModelInterface;
class ScreenieTemplateModel;
class TemplateOrganizerPrivate;

/*!
 * Organizes the ScreenieTemplateModel instances by ordering them according
 * to their \em order.
 *
 * \todo Implement re-ordering (much like changing Tab order in Qt Creator)
 * \sa ScreenieTemplateModel#getOrder
 */
class TemplateOrganizer : public QObject
{
    Q_OBJECT
public:
    TemplateOrganizer(const ScreenieScene &screenieScene);
    virtual ~TemplateOrganizer();

    QList<ScreenieTemplateModel *> getOrderedTemplates() const;

private:
    TemplateOrganizerPrivate *d;

    void frenchConnection();

private slots:
    void handleModelAdded(ScreenieModelInterface &screenieModel);
    void handleModelRemoved(ScreenieModelInterface &screenieModel);
};

#endif // TEMPLATEORGANIZER_H
