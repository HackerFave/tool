#ifndef ABNORMALCRUSH_H
#define ABNORMALCRUSH_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class AbnormalCrush;
}

class AbnormalCrush : public QWidget
{
    Q_OBJECT

public:
    explicit AbnormalCrush(QWidget *parent = NULL);
    ~AbnormalCrush();

    void clearChoice();

public:
    Ui::AbnormalCrush *ui;
    QButtonGroup* buttonGroup;
};

#endif // ABNORMALCRUSH_H
