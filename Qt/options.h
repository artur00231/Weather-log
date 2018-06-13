#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

#include <state.h>

class Options : public State
{
    Q_OBJECT

public:
    Options();


    void set(QWidget *centralWidget, QStatusBar *qStatusBar) override;
    void unset() override;

private slots:
    void setPort(int index);
    void setAutoReconnect(bool auto_reconnect);
    void setDir();
    void setTime(int time);

public slots:
    void reconnect();
    void refresh();

signals:
    void portChanged(QSerialPortInfo);
    void settingsChanged();

private:
    void setupUI();
    void loadAvailablePorts();

    QComboBox *qComboBox;
    QCheckBox *qCheckBox;
    QVBoxLayout *qVBoxLayout;
    QLineEdit *qLineEdit;
    QPushButton *qPushButton;
    QSpinBox *qSpinBox;

    QWidget *centralWidget;
    QStatusBar *qStatusBar;

    QSerialPortInfo qSerialPortInfo;

    bool auto_reconnect { false };
    QString path;
    int time;
};

#endif // OPTIONS_H
