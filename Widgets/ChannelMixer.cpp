#include "ChannelMixer.h"
#include "ui_ChannelMixer.h"

#include "Midi/MidiHelper.h"
#include "Dialogs/SettingVuDialog.h"

#include <QPainter>
#include <QToolTip>
#include <QCursor>
#include <QSettings>

void BackgroundChMx::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBrush(palette().color(QPalette::Text));
    p.drawRect(rect());
    p.end();
}

ChannelMixer::ChannelMixer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelMixer)
{
    ui->setupUi(this);

    player = nullptr;

    setAutoFillBackground(true);

    chs.append(ui->ch1);
    chs.append(ui->ch2);
    chs.append(ui->ch3);
    chs.append(ui->ch4);
    chs.append(ui->ch5);
    chs.append(ui->ch6);
    chs.append(ui->ch7);
    chs.append(ui->ch8);
    chs.append(ui->ch9);
    chs.append(ui->ch10);
    chs.append(ui->ch11);
    chs.append(ui->ch12);
    chs.append(ui->ch13);
    chs.append(ui->ch14);
    chs.append(ui->ch15);
    chs.append(ui->ch16);

    // vu color ==================
    QSettings st;
    LEDVu *vu = ui->ch1->vuBar();

    QString vBg     = st.value("ChMixVuBgColor", vu->backgroundColor().name()).toString();
    QString vOn1    = st.value("ChMixVuColorOn1", vu->ledColorOn1().name()).toString();
    QString vOn2    = st.value("ChMixVuColorOn2", vu->ledColorOn2().name()).toString();
    QString vOn3    = st.value("ChMixVuColorOn3", vu->ledColorOn3().name()).toString();
    QString vOff1   = st.value("ChMixVuColorOff1", vu->ledColorOff1().name()).toString();
    QString vOff2   = st.value("ChMixVuColorOff2", vu->ledColorOff2().name()).toString();
    QString vOff3   = st.value("ChMixVuColorOff3", vu->ledColorOff3().name()).toString();
    bool vShowPh    = st.value("ChMixVuShowPeakHold", true).toBool();
    int vPhTime     = st.value("ChMixVuPeakHoldMs", 500).toInt();
    // ===========================

    for (int i=0; i<16; i++) {
        chs[i]->setChannelNumber(i);
        connect(chs[i], SIGNAL(mouseHover(int)), ui->cbCh, SLOT(setCurrentIndex(int)));
        connect(chs[i], SIGNAL(sliderValueChanged(int,int)),
                this, SLOT(onChSliderValueChanged(int,int)));
        connect(chs[i], SIGNAL(muteChanged(int,bool)),
                this, SLOT(onChMuteChanged(int,bool)));
        connect(chs[i], SIGNAL(soloChanged(int,bool)),
                this, SLOT(onChSoloChanged(int,bool)));

        // set vu Color
        LEDVu *vb = chs[i]->vuBar();
        vb->setBackGroundColor(QColor(vBg));
        vb->setLedColorOn1(QColor(vOn1));
        vb->setLedColorOn2(QColor(vOn2));
        vb->setLedColorOn3(QColor(vOn3));
        vb->setLedColorOff1(QColor(vOff1));
        vb->setLedColorOff2(QColor(vOff2));
        vb->setLedColorOff3(QColor(vOff3));
        vb->setShowPeakHold(vShowPh);
        vb->setPeakHoldMs(vPhTime);
    }

    ui->cbInts->addItems(MidiHelper::GMInstrumentNumberNames());

    connect(ui->cbCh, SIGNAL(currentIndexChanged(int)), this, SLOT(showDeTail(int)));
    connect(ui->chbMuteVoice, SIGNAL(toggled(bool)), this, SLOT(onChbMuteVoiceToggled(bool)));
    connect(ui->chbLock, SIGNAL(toggled(bool)), this, SLOT(onChbLockToggled(bool)));

    connect(ui->cbInts, SIGNAL(activated(int)), this, SLOT(onCbIntsActivated(int)));
    connect(ui->dialPan, SIGNAL(valueChanged(int)), this, SLOT(onDialPanValueChanged(int)));
    connect(ui->dialReverb, SIGNAL(valueChanged(int)), this, SLOT(onDialReverbValueChanged(int)));
    connect(ui->dialChorus, SIGNAL(valueChanged(int)), this, SLOT(onDialChorusValueChanged(int)));

    connect(ui->btnSettingVu, SIGNAL(clicked()), this, SLOT(onBtnSettingVuClicked()));
}

ChannelMixer::~ChannelMixer()
{
    { // setting vu
        QSettings st;
        LEDVu *vu = ui->ch1->vuBar();

        st.setValue("ChMixVuBgColor", vu->backgroundColor().name());
        st.setValue("ChMixVuColorOn1", vu->ledColorOn1().name());
        st.setValue("ChMixVuColorOn2", vu->ledColorOn2().name());
        st.setValue("ChMixVuColorOn3", vu->ledColorOn3().name());
        st.setValue("ChMixVuColorOff1", vu->ledColorOff1().name());
        st.setValue("ChMixVuColorOff2", vu->ledColorOff2().name());
        st.setValue("ChMixVuColorOff3", vu->ledColorOff3().name());
        st.setValue("ChMixVuShowPeakHold", vu->isShowPeakHold());
        st.setValue("ChMixVuPeakHoldMs", vu->peakHoldMs());
    }

    chs.clear();
    delete ui;
}

void ChannelMixer::setLock(bool lock)
{
    if (this->lock == lock)
        return;

    this->lock = lock;

    disconnect(ui->chbLock, SIGNAL(toggled(bool)), this, SLOT(onChbLockToggled(bool)));
    ui->chbLock->setChecked(lock);
    connect(ui->chbLock, SIGNAL(toggled(bool)), this, SLOT(onChbLockToggled(bool)));
}

void ChannelMixer::setPlayer(MidiPlayer *p)
{
    if (player != nullptr) {
        disconnect(player, SIGNAL(loaded()), this, SLOT(onPlayerLoaded()));
        disconnect(player, SIGNAL(sendedEvent(MidiEvent*)),
                   this, SLOT(onPlayerPlayingEvent(MidiEvent*)));
    }

    player = p;

    connect(player, SIGNAL(loaded()), this, SLOT(onPlayerLoaded()));
    connect(player, SIGNAL(sendedEvent(MidiEvent*)),
            this, SLOT(onPlayerPlayingEvent(MidiEvent*)));
}

void ChannelMixer::peak(int ch, int value)
{
    chs[ch]->peak(value);
}

void ChannelMixer::showDeTail(int ch)
{
    disconnect(ui->cbInts, SIGNAL(activated(int)), this, SLOT(onCbIntsActivated(int)));
    disconnect(ui->dialPan, SIGNAL(valueChanged(int)), this, SLOT(onDialPanValueChanged(int)));
    disconnect(ui->dialReverb, SIGNAL(valueChanged(int)), this, SLOT(onDialReverbValueChanged(int)));
    disconnect(ui->dialChorus, SIGNAL(valueChanged(int)), this, SLOT(onDialChorusValueChanged(int)));

    if (ch == 9) {
        ui->cbInts->clear();
        ui->cbInts->addItems(MidiHelper::drumKitNumberNames());
    }
    else {
        ui->cbInts->clear();
        ui->cbInts->addItems(MidiHelper::GMInstrumentNumberNames());
    } 

    int i = player->midiChannel()[ch].instrument();
    int p = player->midiChannel()[ch].pan();
    int r = player->midiChannel()[ch].reverb();
    int c = player->midiChannel()[ch].chorus();

    ui->cbInts->setCurrentIndex(i);
    ui->dialPan->setValue(127-p);
    ui->dialReverb->setValue(r);
    ui->dialChorus->setValue(c);

    setPanToolTip(p);
    ui->dialReverb->setToolTip("เสียงก้อง  " + QString::number(r));
    ui->dialChorus->setToolTip("เสียงประสาน  " + QString::number(c));

    connect(ui->cbInts, SIGNAL(activated(int)), this, SLOT(onCbIntsActivated(int)));
    connect(ui->dialPan, SIGNAL(valueChanged(int)), this, SLOT(onDialPanValueChanged(int)));
    connect(ui->dialReverb, SIGNAL(valueChanged(int)), this, SLOT(onDialReverbValueChanged(int)));
    connect(ui->dialChorus, SIGNAL(valueChanged(int)), this, SLOT(onDialChorusValueChanged(int)));
}

void ChannelMixer::onPlayerLoaded()
{
    for (ChMx *c : chs) {
        c->setSliderValue(100);
    }
    showDeTail(ui->cbCh->currentIndex());
}

void ChannelMixer::onPlayerPlayingEvent(MidiEvent *e)
{
    switch (e->eventType()) {
    case MidiEventType::NoteOn:
        chs[e->channel()]->peak(e->data2());
        break;
    case MidiEventType::Controller:
        if (e->data1() == 7) {
            chs[e->channel()]->setSliderValue(e->data2());
        }
        if (e->channel() == ui->cbCh->currentIndex()) {
            switch (e->data1()) {
            case 10:
            case 91:
            case 93:
                showDeTail(ui->cbCh->currentIndex());
                break;
            default:
                break;
            }
        }
        break;
    case MidiEventType::ProgramChange:
        if (e->channel() == ui->cbCh->currentIndex())
            showDeTail(ui->cbCh->currentIndex());
        break;
    default:
        break;
    }
}

void ChannelMixer::leaveEvent(QEvent *event)
{
    if (ui->cbCh->isPopupVisible() || ui->cbInts->isPopupVisible())
        return;

    emit mouseLeaved();
}

void ChannelMixer::onChSliderValueChanged(int ch, int v)
{
    if (player == nullptr)
        return;

    player->setVolume(ch, v);
}

void ChannelMixer::onChMuteChanged(int ch, bool m)
{
    if (player == nullptr)
        return;

    player->setMute(ch, m);

    if (ch == 8) {
        disconnect(ui->chbMuteVoice, SIGNAL(toggled(bool)), this, SLOT(onChbMuteVoiceToggled(bool)));
        ui->chbMuteVoice->setChecked(m);
        connect(ui->chbMuteVoice, SIGNAL(toggled(bool)), this, SLOT(onChbMuteVoiceToggled(bool)));
    }
}

void ChannelMixer::onChSoloChanged(int ch, bool s)
{
    if (player == nullptr)
        return;

    player->setSolo(ch, s);
}

void ChannelMixer::onCbIntsActivated(int index)
{
    if (player == nullptr)
        return;

    player->setInstrument(ui->cbCh->currentIndex(), index);
}

void ChannelMixer::onDialPanValueChanged(int value)
{
    if (player == nullptr)
        return;

    player->setPan(ui->cbCh->currentIndex(), 127-value);
    setPanToolTip(player->midiChannel()[ui->cbCh->currentIndex()].pan());
    QToolTip::showText(QCursor::pos(), ui->dialPan->toolTip());
}

void ChannelMixer::onDialReverbValueChanged(int value)
{
    if (player == nullptr)
        return;

    player->setReverb(ui->cbCh->currentIndex(), value);
    ui->dialReverb->setToolTip("เสียงก้อง  " + QString::number(value));
    QToolTip::showText(QCursor::pos(), ui->dialReverb->toolTip());
}

void ChannelMixer::onDialChorusValueChanged(int value)
{
    if (player == nullptr)
        return;

    player->setChorus(ui->cbCh->currentIndex(), value);
    ui->dialChorus->setToolTip("เสียงประสาน  " + QString::number(value));
    QToolTip::showText(QCursor::pos(), ui->dialChorus->toolTip());
}

void ChannelMixer::setPanToolTip(int value)
{
    if (value > 64) { // left
        int v = value - 64;
        ui->dialPan->setToolTip("ซ้าย  " + QString::number(100 * v / 63) + "%");
    }
    else if (value < 64) {
        int v = 63 - value;
        ui->dialPan->setToolTip("ขวา  " + QString::number(100 * v / 63) + "%");
    }
    else {
        ui->dialPan->setToolTip("กึ่งกลาง");
    }
}

void ChannelMixer::onChbMuteVoiceToggled(bool checked)
{if (player == nullptr)
        return;

    player->setMute(8, checked);
    chs[8]->setMuteButton(checked);
}

void ChannelMixer::onChbLockToggled(bool checked)
{
    this->lock = checked;
    emit lockChanged(checked);
}

void ChannelMixer::onBtnSettingVuClicked()
{
    QList<LEDVu*> vus;
    for (ChMx *cm : chs)
        vus.append(cm->vuBar());

    SettingVuDialog dlg(this, vus);
    dlg.setWindowTitle("ตั้งค่า LED Meter (ช่องสัญญาณมิกเซอร์)");
    dlg.setModal(true);
    dlg.adjustSize();
    dlg.setFixedSize(dlg.size());
    dlg.exec();
}
