#include <QMessageBox>
#include <QFileDialog>
#include <QDBusReply>
#include <QToolButton>
#include <QLineEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include "alarmwidget.hpp"
#include "ui_alarmwidget.h"
#include "mainwindow.hpp"

AlarmWidget::AlarmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmWidget)
{
    ui->setupUi(this);

    connect(ui->alarmPathButton, &QToolButton::clicked, [this]{
        QString path = QFileDialog::getOpenFileName(
            this,
            tr("Open Image"),
            QDir::homePath(),
            tr("Audio Files (*.mp3 *wav *.ogg)")
        );

        this->ui->alarmPathInput->setText(path);
    });
}

void AlarmWidget::connectToDBus(QDBusInterface& dbus)
{
    // removed hook
    connect(ui->deleteButton, &QToolButton::clicked, [this, &dbus]{
        QDBusReply<void> reply = dbus.call("removeAlarm", qVariantFromValue(this->alarmData));

        if(!reply.isValid())
        {
            displayError(reply.error().message());
            return;
        }

        delete this;
    });


    // modified hooks
    connect(ui->enableButton, &QToolButton::clicked, [this, &dbus]{
        this->alarmData.enabled = this->ui->enableButton->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->labelInput, &QLineEdit::textChanged, [this, &dbus]{
        this->alarmData.label = this->ui->labelInput->text();
        this->sendModificationSignal(dbus);
    });

    connect(ui->alarmPathInput, &QLineEdit::textChanged, [this, &dbus]{
        this->alarmData.audioPath = this->ui->alarmPathInput->text();
        this->sendModificationSignal(dbus);
    });

    connect(ui->timeInput, &QTimeEdit::timeChanged, [this, &dbus]{
        this->alarmData.hour = this->ui->timeInput->time().hour();
        this->alarmData.minute = this->ui->timeInput->time().minute();
        this->sendModificationSignal(dbus);
    });

    connect(ui->sundayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.sunday = this->ui->sundayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->mondayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.monday = this->ui->mondayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->tuesdayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.tuesday = this->ui->tuesdayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->wednesdayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.wednesday = this->ui->wednesdayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->thursdayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.thursday = this->ui->thursdayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->fridayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.friday = this->ui->fridayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });

    connect(ui->saturdayCheckBox, &QCheckBox::toggled, [this, &dbus]{
        this->alarmData.saturday = this->ui->saturdayCheckBox->isChecked();
        this->sendModificationSignal(dbus);
    });
}

AlarmData AlarmWidget::getAlarmData()
{
    return alarmData;
}

void AlarmWidget::setAlarmData(AlarmData alarmData)
{
    this->alarmData = alarmData;

    ui->enableButton->setChecked(alarmData.enabled);

    ui->labelInput->setText(alarmData.label);
    ui->alarmPathInput->setText(alarmData.audioPath);

    QTime time(alarmData.hour, alarmData.minute);
    ui->timeInput->setTime(time);

    ui->sundayCheckBox->setChecked(alarmData.sunday);
    ui->mondayCheckBox->setChecked(alarmData.monday);
    ui->tuesdayCheckBox->setChecked(alarmData.tuesday);
    ui->wednesdayCheckBox->setChecked(alarmData.wednesday);
    ui->thursdayCheckBox->setChecked(alarmData.thursday);
    ui->fridayCheckBox->setChecked(alarmData.friday);
    ui->saturdayCheckBox->setChecked(alarmData.saturday);
}

void AlarmWidget::sendModificationSignal(QDBusInterface& dbus)
{
    QDBusReply<void> reply = dbus.call("modifyAlarm", qVariantFromValue(alarmData));

    if(!reply.isValid())
        displayError(reply.error().message());
}

void AlarmWidget::displayError(QString message)
{
    QMessageBox errorBox;
    errorBox.critical(this, "error", message);
}

AlarmWidget::~AlarmWidget()
{
    delete ui;
}
