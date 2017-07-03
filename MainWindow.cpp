#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SettingsDialog.h"
#include "Midi/MidiFile.h"

#include <QTime>
#include <QMenu>
#include <QDebug>
#include <QDir>
#include <QDirIterator>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    lyrWidget = new LyricsWidget(this);

    ui->setupUi(this);

    settings = new QSettings();
    db = new SongDatabase();
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    positionTimer = new QTimer();
    positionTimer->setInterval(30);

    player = new MidiPlayer();

    locale = QLocale(QLocale::English, QLocale::UnitedStates);

    { // UI
        int sTime = settings->value("SearchTimeout", 5).toInt();
        int pTime = settings->value("PlaylistTimeout", 5).toInt();
        setSearchTimeout(sTime);
        setPlaylistTimeout(pTime);

        bool removeList  = settings->value("RemoveFromPlaylist", true).toBool();
        bool aPlayNext   = settings->value("AutoPlayNext", true).toBool();
        remove_playlist = removeList;
        auto_playnext = aPlayNext;

        int bg = settings->value("BackgroundType", 0).toInt();
        if (bg == 0) {
            QString color = settings->value("BackgroundColor", "#525252").toString();
            setBackgroundColor(color);
        } else {
            QString img = settings->value("BackgroundImage", "").toString();
            if (QFile::exists(img)) {
                setBackgroundImage(img);
            } else {
                QString color = settings->value("BackgroundColor", "#525252").toString();
                setBackgroundColor(color);
            }
        }

        int w     = settings->value("WindowWidth", this->minimumWidth()).toInt();
        int h     = settings->value("WindowHeight", this->minimumHeight()).toInt();
        bool max  = settings->value("WindowMaximized", false).toBool();
        bool full = settings->value("WindowFullScreen", false).toBool();

        this->resize(w, h);
        if (max) {
            this->showMaximized();
        } else {
            if (full) {
                this->showFullScreen();
            }
        }
    }


    { // Player
        int oPort   = settings->value("MidiOut", 0).toInt();
        int vl      = settings->value("MidiVolume", 50).toInt();
        bool lDrum  = settings->value("MidiLockDrum", false).toBool();
        bool lSnare = settings->value("MidiLockSnare", false).toBool();
        bool lBass  = settings->value("MidiLockBass", false).toBool();

        player->setMidiOut(oPort);
        player->setVolume(vl);

        if (lDrum) {
            int ldNum = settings->value("MidiLockDrumNumber", 0).toInt();
            player->setLockDrum(true, ldNum);
        }
        if (lSnare) {
            int lsNum = settings->value("MidiLockSnareNumber", 38).toInt();
            player->setLockSnare(true, lsNum);
        }
        if (lBass) {
            int lbNum = settings->value("MidiLockBassNumber", 32).toInt();
            player->setLockBass(true, lbNum);
        }

        std::vector<std::string> sfs;
        sfs.push_back("D:/eXtreme Karaoke/SoundFont/god.sf2");
        //sfs.push_back("/home/noob/SoundFont/SoundFont_2_Drum.sf2");

        player->midiSynthesizer()->setSoundFonts(sfs);

        connect(player, SIGNAL(finished()), this, SLOT(onPlayerThreadFinished()));
        connect(player, SIGNAL(playingEvents(MidiEvent*)), this, SLOT(onPlayerPlayingEvent(MidiEvent*)));
        connect(player, SIGNAL(bpmChanged(int)), ui->rhmWidget, SLOT(setBpm(int)));
        connect(player, SIGNAL(beatCountChanged(int)), ui->rhmWidget, SLOT(setBeatCount(int)));
        connect(player, SIGNAL(beatInBarChanged(int)), ui->rhmWidget, SLOT(setBeatInBar(int)));
    }


    //QStringList sfs;
    //sfs << "/home/noob/SOMSAK_2017_V1.SF2";
    //player->setSoundFonts(sfs);

    // Test
    //connect(player, SIGNAL(positionChanged(int)), ui->sliderPosition, SLOT(setValue(int)));
    //connect(player, SIGNAL(positionChanged(int)), lyrics, SLOT(setCursorPosition(int)));
    //connect(player, SIGNAL(durationChanged(int)), this, SLOT(onPlayerDurationTickChanged(int)));
    //connect(player, SIGNAL(durationMSChanged(qint64)), this, SLOT(onPlayerDurationMSChanged(qint64)));
    //connect(player, SIGNAL(positionMSChanged(qint64)), this, SLOT(onPlayerpositionMSChanged(qint64)));
    /*connect(player, SIGNAL(finished()), this, SLOT(onPlayerFinished()));
    connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pause()));
    connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(playNext()));
    connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(playPrevious()));
    //connect(ui->sliderPosition, SIGNAL(sliderMoved(int)), player, SLOT(setPosition(int)));
    connect(ui->sliderPosition, SIGNAL(sliderPressed()), this, SLOT(onSliderPositionPressed()));
    connect(ui->sliderPosition, SIGNAL(sliderReleased()), this, SLOT(onSliderPositionReleased()));

    connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(onSliderVolumeValueChanged(int)));
    */


    { // Lyrics
        QString family  = settings->value("LyricsFamily", font().family()).toString();
        int size        = settings->value("LyricsSize", 40).toInt();
        int weight      = settings->value("LyricsWeight", 75).toInt();
        bool italic     = settings->value("LyricsItalic", false).toBool();
        bool strikeOut  = settings->value("LyricsStrikeOut", false).toBool();
        bool underline  = settings->value("LyricsUnderline", false).toBool();

        QString tColor  = settings->value("LyricsTextColor", "#f3f378").toString();
        QString tbColor = settings->value("LyricsTextBorderColor", "#000000").toString();
        int     tbWidth = settings->value("LyricsTextBorderWidth", 2).toInt();

        QString cColor  = settings->value("LyricsCurColor", "#ff0000").toString();
        QString cbColor = settings->value("LyricsCurBorderColor", "#ffffff").toString();
        int     cbWidth = settings->value("LyricsCurBorderWidth", 3).toInt();

        int     line1Y  = settings->value("LyricsLine1Y", 320).toInt();
        int     line2Y  = settings->value("LyricsLine2Y", 170).toInt();
        int     aTime   = settings->value("LyricsAnimationTime", 300).toInt();

        QFont f;
        f.setFamily(family);
        f.setPointSize(size);
        f.setWeight(weight);
        f.setItalic(italic);
        f.setStrikeOut(strikeOut);
        f.setUnderline(underline);

        lyrWidget->setTextFont(f);
        lyrWidget->setTextColor(QColor(tColor));
        lyrWidget->setTextBorderColor(QColor(tbColor));
        lyrWidget->setTextBorderWidth(tbWidth);

        lyrWidget->setCurColor(QColor(cColor));
        lyrWidget->setCurBorderColor(QColor(cbColor));
        lyrWidget->setCurBorderWidth(cbWidth);

        lyrWidget->setLine1Y(line1Y);
        lyrWidget->setLine2Y(line2Y);
        lyrWidget->setAnimationTime(aTime);
    }


    { // Init UI
        ui->frameSearch->hide();
        ui->framePlaylist->hide();
        ui->sliderVolume->setValue(player->volume());

        connect(timer2, SIGNAL(timeout()), ui->frameSearch, SLOT(hide()));
        connect(timer2, SIGNAL(timeout()), ui->framePlaylist, SLOT(hide()));
        connect(timer1, SIGNAL(timeout()), this, SLOT(showCurrentTime()));
        connect(positionTimer, SIGNAL(timeout()), this, SLOT(onPositiomTimerTimeOut()));

        connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pause()));
        connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stop()));
        connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(playNext()));
        connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(playPrevious()));
        connect(ui->sliderPosition, SIGNAL(sliderPressed()), this, SLOT(onSliderPositionPressed()));
        connect(ui->sliderPosition, SIGNAL(sliderReleased()), this, SLOT(onSliderPositionReleased()));
        connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(onSliderVolumeValueChanged(int)));

        timer1->start(1000);
    }


    // Menu
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

}

MainWindow::~MainWindow()
{
    stop();

    settings->setValue("MidiVolume", ui->sliderVolume->value());
    if (this->isFullScreen()) {
        settings->setValue("WindowFullScreen", true);
        settings->setValue("WindowMaximized", false);
    } else if (this->isMaximized()) {
        settings->setValue("WindowFullScreen", false);
        settings->setValue("WindowMaximized", true);
    } else {
        settings->setValue("WindowFullScreen", false);
        settings->setValue("WindowMaximized", false);
        settings->setValue("WindowWidth", this->width());
        settings->setValue("WindowHeight", this->height());
    }

    foreach (Song *s, playlist) {
        delete s;
    }

    delete player;
    delete positionTimer;
    delete timer2;
    delete timer1;
    delete db;
    delete settings;
    delete ui;

    delete lyrWidget;
}

void MainWindow::setBackgroundColor(QString colorName)
{
    bgType = 0;
    this->setStyleSheet("#MainWindow {background-color: " + colorName + ";}");
}

void MainWindow::setBackgroundImage(QString img)
{
    if (QFile::exists(img)) {
        this->setStyleSheet("");
        bgType = 1;
        bgImg = img;
        QPixmap bg(img);
        bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bg); //set the pic to the background
        this->setPalette(palette); //show the background pic
    }
}

QList<long> readCurFile(QFile *curFile, uint32_t resolution) {
    QList<long> curs;
    if (!curFile->isOpen()) {
        curFile->open(QIODevice::ReadOnly);
    }
    QDataStream in(curFile);
    while (!in.atEnd()) {
        quint8 b1 = 0;
        quint8 b2 = 0;
        in >> b1;
        in >> b2;
        long cs = (b1 + (b2 << 8)) * resolution / 24;
        curs.append(cs);
    }
    return curs;
}

void MainWindow::play(int index)
{
    stop();
    if (index == -1 && playingSong.id() != "") {
        lyrWidget->reset();
        player->start();
        lyrWidget->show();
        positionTimer->start();
        return;
    }

    QString ncnPath = settings->value("NCNPath").toString();

    Song *s = playlist[index];
    QString p = QDir::toNativeSeparators(ncnPath + s->path());
    playingSong = *s;
    playingIndex = index;

    if (remove_playlist) {
        delete s; // delete playlist in "index"
        playlist.removeAt(index);
        ui->playlist->takeItem(index);
        playingIndex = -1;
    }


    QString curPath = "";
    {// Find Cursor file and path
        curPath = playingSong.path().replace("Song", "Cursor");
        curPath = QDir::toNativeSeparators(ncnPath + curPath);
        curPath = curPath.replace(playingSong.id(), "");
        curPath = curPath.replace(curPath.length() - 4, 4, "");
        QDirIterator ct(curPath ,QStringList() << playingSong.id() +".cur" ,QDir::Files);
        if (ct.hasNext()) {
            ct.next();
            curPath = ct.filePath();
        }
    }

    player->load(p.toStdString());

    QFile f(curPath);
    lyrWidget->setLyrics(playingSong.lyrics(), readCurFile(&f, player->midiFile()->resorution()));
    onPlayerDurationTickChanged(player->durationTick());
    onPlayerDurationMSChanged(player->durationMs());

    player->start();
    lyrWidget->show();
    positionTimer->start();
}

void MainWindow::pause()
{
    positionTimer->stop();
    lyrWidget->stopAnimation();
    player->stop();
}

void MainWindow::resume()
{
    player->start();
    positionTimer->start();
}

void MainWindow::stop()
{
    positionTimer->stop();
    player->stop(true);

    ui->sliderPosition->setValue(0);
    lyrWidget->hide();
    lyrWidget->reset();
    ui->sliderPosition->setValue(0);
    onPlayerPositionMSChanged(0);
}

void MainWindow::playNext()
{
    if (playlist.count() > 0 && playingIndex < playlist.count()-1) {
        play(playingIndex+1);
    } else {
        stop();
    }
}

void MainWindow::playPrevious()
{
    if (playingIndex > 0 && playlist.count() > 0) {
        play(playingIndex - 1);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (bgType == 1) {
        setBackgroundImage(bgImg);
        QMainWindow::resizeEvent(event);
    }
    lyrWidget->resize(ui->centralWidget->size());
    emit resized(event->size());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_PageUp:
        if (ui->playlist->isVisible()) {
            int i = ui->playlist->currentRow();
            if (i == 0)
                break;

            playlist.swap(i, i-1);
            QListWidgetItem *item = ui->playlist->takeItem(i);
            ui->playlist->insertItem(i-1, item);
            ui->playlist->setCurrentRow(i-1);
            ui->playlist->show();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_PageDown:
        if (ui->playlist->isVisible()) {
            int i = ui->playlist->currentRow();
            if (i >= ui->playlist->count() -1)
                break;

            playlist.swap(i, i+1);
            QListWidgetItem *item = ui->playlist->takeItem(i);
            ui->playlist->insertItem(i+1, item);
            ui->playlist->setCurrentRow(i+1);
            ui->playlist->show();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Home:
        if (playingSong.id() != "") {
            play(-1);
        }
        break;
    case Qt::Key_End:
        if (auto_playnext) {
            playNext();
        } else {
            stop();
        }
        break;
    case Qt::Key_Up:
        if (ui->framePlaylist->isVisible()) {
            if (ui->playlist->currentRow() > 0 )
                ui->playlist->setCurrentRow( ui->playlist->currentRow() - 1 );
            timer2->start(playlist_timeout);
        } else {
            ui->frameSearch->hide();
            if (ui->playlist->currentRow() == -1)
                ui->playlist->setCurrentRow(0);
            ui->framePlaylist->show();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Down:
        if (ui->framePlaylist->isVisible()) {
            if (ui->playlist->currentRow() < ui->playlist->count() - 1 )
                ui->playlist->setCurrentRow( ui->playlist->currentRow() + 1 );
            timer2->start(playlist_timeout);
        } else {
            ui->frameSearch->hide();
            if (playingIndex == -1)
                ui->playlist->setCurrentRow(0);
            else
                ui->playlist->setCurrentRow(playingIndex);
            ui->framePlaylist->show();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Right:
        if (ui->frameSearch->isVisible()) {
            setFrameSearch( db->searchNext() );
            timer2->start(search_timeout);
        } else {
            ui->framePlaylist->hide();
            if (ui->lbId->text() == "")
                setFrameSearch( db->search("") );
            ui->lbSearch->setText("_");
            ui->frameSearch->show();
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Left:
        if (ui->frameSearch->isVisible()) {
            setFrameSearch( db->searchPrevious() );
            timer2->start(search_timeout);
        } else {
            ui->framePlaylist->hide();
            if (ui->lbId->text() == "")
                setFrameSearch( db->search("") );
            ui->lbSearch->setText("_");
            ui->frameSearch->show();
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Tab:
        if (ui->frameSearch->isVisible()) {
            QString s = ui->lbSearch->text();
            s = s.replace(s.length() - 1, 1, "");
            setFrameSearch( db->nextType(s) );
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Backspace:
        if (ui->frameSearch->isVisible()) {
            if (ui->lbSearch->text() == "_") {
                timer2->start(search_timeout);
                break;
            }
            QString s = ui->lbSearch->text();
            s = s.replace(s.length() - 2, 2, "");
            setFrameSearch( db->search(s) );
            ui->lbSearch->setText(s + "_");
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (ui->frameSearch->isVisible()) {
            Song *s = db->currentSong();
            ui->playlist->addItem( " " + s->id() + " " + s->name() +
                                   " - " + s->artist());
            QSize size;
            size.setHeight(41);
            ui->playlist->item(ui->playlist->count() - 1)->setSizeHint(size);
            ui->frameSearch->hide();

            // Test
            Song *sToAdd = new Song();
            *sToAdd = *s;
            playlist.append(sToAdd);

            if (auto_playnext && playlist.count() == 1 && player->isPlayerStopped()) {
                play(0);
            }
        }
        if (ui->framePlaylist->isVisible()) {
            ui->framePlaylist->hide();
            play(ui->playlist->currentRow());
        }
        break;
    default:
        if (ui->frameSearch->isVisible()) {
            QString s = ui->lbSearch->text();
            s = s.replace(s.length() - 1, 1, "");
            ui->lbSearch->setText(s + event->text() + "_");
            setFrameSearch( db->search(s + event->text()) );
            timer2->start(search_timeout);
        } else {
            db->setSearchType(SongDatabase::ByName);
            ui->lbSearch->setText(event->text() + "_");
            setFrameSearch( db->search(event->text() ));
            ui->framePlaylist->hide();
            ui->frameSearch->show();
            timer2->start(search_timeout);
        }
        break;
    }
}

void MainWindow::showCurrentTime()
{
    QTime time = QTime::currentTime();
    QString currentTime = locale.toString(time, "hh:mm:ss");
    ui->lcdTime->display(currentTime);
}

void MainWindow::setFrameSearch(Song *s)
{
    ui->lbId->setText(s->id());
    ui->lbName->setText(" " + s->name());
    ui->lbBtw->setText("-");
    ui->lbArtist->setText(s->artist());
    if (s->key() != "")
        ui->lbTempoKey->setText(" (" + QString::number(s->tempo()) + "-" + s->key() + ")");
    else
        ui->lbTempoKey->setText(" (" + QString::number(s->tempo()) + ")");
    ui->lbType->setText("[" + s->songType() + "]");
    ui->lbLyrics->setText(s->lyrics().replace("\r\n", " "));
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QMenu menu(tr("Context menu"), this);

    QAction actionSettings("ตั้งค่า", this);
    QAction actionFullScreen("เต็มหน้าจอ (ย่อ/ขยาย)", this);
    QAction actionExit("ออกจากโปรแกรม", this);

    connect(&actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    connect(&actionFullScreen, SIGNAL(triggered()), this, SLOT(showFullScreenOrNormal()));
    connect(&actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    menu.addAction(&actionSettings);
    menu.addSeparator();
    menu.addAction(&actionFullScreen);
    menu.addSeparator();
    menu.addAction(&actionExit);

    menu.exec(mapToGlobal(pos));
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog d(this, this);
    d.setModal(true);
    d.exec();
}

void MainWindow::showFullScreenOrNormal()
{
    if (this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void MainWindow::onPositiomTimerTimeOut()
{
    int tick = player->positionTick();

    /*int b = player->midiFile()->beatFromTick(tick);
    if (b != beat) {
        qDebug() << "Beat  is : " << b;
        beat = b;
    }*/
    ui->rhmWidget->setCurrentBeat( player->midiFile()->beatFromTick(tick) );

    onPlayerPositionMSChanged(player->positionMs());
    ui->sliderPosition->setValue(tick);
    lyrWidget->setPositionCursor(tick - 40);

    /*if (player->isFinished()) {
        QThread::msleep(50);
        if (auto_playnext)
            playNext();
        else
            stop();
    }*/
}

void MainWindow::onPlayerDurationMSChanged(qint64 d)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(d);
    ui->lbDuration->setText( locale.toString(t, "mm:ss") );
}

void MainWindow::onPlayerPositionMSChanged(qint64 p)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(p);
    ui->lbPosition->setText( locale.toString(t, "mm:ss") );
}

void MainWindow::onPlayerDurationTickChanged(int d)
{
    ui->sliderPosition->setMaximum(d);
}

void MainWindow::onSliderPositionPressed()
{
    playAfterSeek = player->isPlayerPlaying();
    pause();
}

void MainWindow::onSliderPositionReleased()
{
    if (player->isPlayerStopped()) {
        ui->sliderPosition->setValue(0);
        return;
    }

    player->setPositionTick(ui->sliderPosition->value());
    lyrWidget->setSeekPositionCursor(ui->sliderPosition->value());
    onPlayerPositionMSChanged(player->positionMs());
    if (playAfterSeek) resume();
}

void MainWindow::on_btnVolumeMute_clicked()
{
    if (ui->sliderVolume->isEnabled()) {
        ui->sliderVolume->setEnabled(false);
        ui->btnVolumeMute->setIcon(QIcon(":/Icons/volume-adjustment-mute.png"));
        player->setVolume(0);
    } else {
        ui->sliderVolume->setEnabled(true);
        ui->btnVolumeMute->setIcon(QIcon(":/Icons/volume-up-interface-symbol.png"));
        player->setVolume(ui->sliderVolume->value());
    }
}

void MainWindow::on_btnPlay_clicked()
{
    if (player->isPlayerPaused()) {
        resume();
    } else {
        if (playingSong.id() != "")
            play(-1);
    }
}


void MainWindow::onSliderVolumeValueChanged(int value)
{
    player->setVolume(value);
}

void MainWindow::onPlayerThreadFinished()
{
    if (player->isPlayerFinished())
        playNext();
}

void MainWindow::onPlayerPlayingEvent(MidiEvent *e)
{
    /*if (e->eventType() == MidiEventType::Meta) {
        //qDebug() << "This is Meta";
        switch (e->metaEventType()) {
        case MidiMetaType::TimeSignature:
            qDebug() << "BPM is " << e->tempoBpm();
            //ui->rhmWidget->setBpm(e->tempoBpm());
            break;
        default:
            break;
        }
    }*/
}
