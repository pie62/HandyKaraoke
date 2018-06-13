#include "SpeakerDialog.h"
#include "ui_SpeakerDialog.h"

#include <QMenu>
#include <QSettings>

#include <Config.h>
#include "Dialogs/DialogHelper.h"

#define ICON_SOUNDCARD  ":/Icons/SoundCard/sound-card.png"
#define ICON_MIXER      ":/Icons/SoundCard/mixer.png"

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
        QString name = chInstMap->values()[i]->fullInstrumentName();
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);

        ui->tableSpeaker->insertRow(i);
        ui->tableSpeaker->setItem(i, 0, nameItem);
        ui->tableSpeaker->setItem(i, 1, new QTableWidgetItem());
        ui->tableSpeaker->setItem(i, 2, new QTableWidgetItem());

        setItems(i);
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

        setItems(row);

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

void SpeakerDialog::setItems(int row)
{
    QTableWidgetItem *itemDevice = ui->tableSpeaker->item(row, 1);
    QTableWidgetItem *itemSpeaker = ui->tableSpeaker->item(row, 2);

    auto t = static_cast<InstrumentType>(row);


    // set device icon, name
    QIcon iconDevice;
    QString nameDevice;
    if (t < InstrumentType::BusGroup1 && synth->busGroup(t) != -1)
    {
        auto busType = static_cast<InstrumentType>(synth->busGroup(t) + synth->HANDLE_BUS_START);
        iconDevice.addFile(ICON_MIXER);
        nameDevice = (*chInstMap)[busType]->fullInstrumentName();
    }
    else
    {
        int device = synth->device(t);
        iconDevice.addFile(ICON_SOUNDCARD);
        nameDevice = (device == 0) ? "Default Audio Device" : deviceNames[device];
    }

    itemDevice->setIcon(iconDevice);
    itemDevice->setText(nameDevice);


    // set speaker icon, name
    QIcon iconSpeaker;
    QString nameSpeaker;
    iconSpeaker.addFile(DialogHelper::getSpeakerIconName(synth->speaker(t)));
    nameSpeaker = DialogHelper::getSpeakerName(synth->speaker(t));

    itemSpeaker->setIcon(iconSpeaker);
    itemSpeaker->setText(nameSpeaker);

    if (t >= InstrumentType::BusGroup1)
        itemSpeaker->setFlags(itemSpeaker->flags() & ~Qt::ItemIsEnabled);
}
