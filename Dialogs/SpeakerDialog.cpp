#include "SpeakerDialog.h"
#include "ui_SpeakerDialog.h"

#include <QMenu>
#include <QSettings>

#include <Config.h>
#include "Dialogs/DialogHelper.h"

SpeakerDialog::SpeakerDialog(QWidget *parent, QMap<InstrumentType, InstCh *> *chInstMap, MainWindow *mainWindow) :
    QDialog(parent),
    ui(new Ui::SpeakerDialog)
{
    ui->setupUi(this);

    this->chInstMap = chInstMap;
    this->mainWindow = mainWindow;
    this->player = mainWindow->midiPlayer();
    this->synth = player->midiSynthesizer();

    for (const QString &n : MidiSynthesizer::audioDevices())
        deviceNames.append(n);

    // setup speaker table
    for (int i=0; i<chInstMap->keys().count(); i++)
    {
        InstrumentType t = chInstMap->keys()[i];

        if (t >= InstrumentType::BusGroup1)
            break;

        QString deviceName;
        if (synth->speaker(t) == SpeakerType::Default)
            deviceName = "Default Audio Device";
        else
            deviceName = deviceNames[synth->device(t)];

        QString speakerName = DialogHelper::getSpeakerName(synth->speaker(t));

        QTableWidgetItem *nameItem = new QTableWidgetItem(chInstMap->value(t)->fullInstrumentName());
        QTableWidgetItem *deviceItem = new QTableWidgetItem(deviceName);
        QTableWidgetItem *speakerItem = new QTableWidgetItem(speakerName);

        deviceItem->setIcon(QIcon(":/Icons/SoundCard/sound-card.png"));
        speakerItem->setIcon(QIcon(DialogHelper::getSpeakerIconName(synth->speaker(t))));

        ui->tableSpeaker->insertRow(i);
        ui->tableSpeaker->setItem(i, 0, nameItem);
        ui->tableSpeaker->setItem(i, 1, deviceItem);
        ui->tableSpeaker->setItem(i, 2, speakerItem);
    }

    QHeaderView *header = ui->tableSpeaker->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(2, QHeaderView::Stretch);

    connect(ui->tableSpeaker, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showSpeakerMenu(QPoint)));
}

SpeakerDialog::~SpeakerDialog()
{
    delete ui;
}

void SpeakerDialog::on_btnClose_clicked()
{
    this->close();
}

void SpeakerDialog::showSpeakerMenu(const QPoint &pos)
{
    QMenu menu;
    QSignalMapper signalMapper;
    QIcon iconCard(":/Icons/SoundCard/sound-card.png");

    QAction *act = menu.addAction(iconCard, "Default Device / Speaker");
    connect(act, SIGNAL(triggered()), &signalMapper, SLOT(map()));
    signalMapper.setMapping(act, 0);

    menu.addSeparator();

    for (int i=1; i<MidiSynthesizer::audioDevices().size(); i++)
    {
        QMenu *m = menu.addMenu(iconCard, deviceNames[i]);
        DialogHelper::createSpeakerMenu(m, &signalMapper, i);
    }

    connect(&signalMapper, SIGNAL(mapped(int)), this, SLOT(setSpeaker(int)));

    menu.exec(ui->tableSpeaker->mapToGlobal(pos));
}

void SpeakerDialog::setSpeaker(int number)
{
    int device = (number & 0xFFFF);
    SpeakerType speaker = static_cast<SpeakerType>((number>>16) & 0xFFFF);

    PlayerState state = player->playerState();
    int position = player->positionTick();
    if (synth->isOpened() && player->isPlayerPlaying())
        mainWindow->stop();

    QSettings settings(CONFIG_SYNTH_FILE_PATH, QSettings::IniFormat);
    settings.beginWriteArray("SynthMixer");

    QModelIndexList selection = ui->tableSpeaker->selectionModel()->selectedRows();
    for (int i=0; i<selection.count(); i++)
    {
        int row = selection.at(i).row();

        InstrumentType instType = static_cast<InstrumentType>(row);
        synth->setDevice(instType, device);
        synth->setSpeaker(instType, speaker);

        if (speaker == SpeakerType::Default)
        {
            QTableWidgetItem *item =  ui->tableSpeaker->item(row, 1);
            item->setIcon(QIcon(":/Icons/SoundCard/sound-card.png"));
            item->setText("Default Audio Device");

            item =  ui->tableSpeaker->item(row, 2);
            item->setIcon(QIcon(DialogHelper::getSpeakerIconName(SpeakerType::Default)));
            item->setText(DialogHelper::getSpeakerName(SpeakerType::Default));
        }
        else
        {
            QTableWidgetItem *item =  ui->tableSpeaker->item(row, 1);
            item->setIcon(QIcon(":/Icons/SoundCard/sound-card.png"));
            item->setText(deviceNames[device]);

            item =  ui->tableSpeaker->item(row, 2);
            item->setIcon(QIcon(DialogHelper::getSpeakerIconName(speaker)));
            item->setText(DialogHelper::getSpeakerName(speaker));
        }

        settings.setArrayIndex(row);
        settings.setValue("Device", synth->device(instType));
        settings.setValue("Speaker", static_cast<int>(synth->speaker(instType)));
    }

    settings.endArray();

    if (synth->isOpened())
    {
        switch (state) {
        case PlayerState::Playing:
            mainWindow->play(-1, position);
            break;
        case PlayerState::Paused:
            mainWindow->midiPlayer()->setPositionTick(position);
            mainWindow->lyricsWidget()->setSeekPositionCursor(position);
            if (mainWindow->secondLyrics() != nullptr)
                mainWindow->secondLyrics()->setSeekPositionCursor(position);
            break;
        default:
            break;
        }
    }
}
