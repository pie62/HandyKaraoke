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
    ui->setupUi(this);

    settings = new QSettings();
    db = new SongDatabase();
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    positionTimer = new QTimer();
    positionTimer->setInterval(10);

    player = new MidiPlayer();
    lyrics = new LyricsWidget(this);

    ui->verticalLayout_4->insertWidget(3, lyrics, 1);

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

        this->setGeometry(this->x(), this->y(), w, h);
        if (max) {
            this->showMaximized();
        } else {
            if (full) {
                this->showFullScreen();
            }
        }
    }


    int oPort = settings->value("MidiOut", 0).toInt();
    //player->setMidiOut(oPort);
    player->setMidiOut(-1);
    player->setVolume(settings->value("MidiVolume", 50).toInt());

    std::vector<std::string> sfs;
    sfs.push_back("/home/noob/SOMSAK_2017_V1.SF2");
    sfs.push_back("/home/noob/SoundFont_2_Drum.sf2");

    player->midiSynthesizer()->setSoundFonts(sfs);


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
        int     tbWidth = settings->value("LyricsTextBorderWidth", 1).toInt();

        QString cColor  = settings->value("LyricsCurColor", "#ff0000").toString();
        QString cbColor = settings->value("LyricsCurBorderColor", "#ffffff").toString();
        int     cbWidth = settings->value("LyricsCurBorderWidth", 2).toInt();

        QFont f;
        f.setFamily(family);
        f.setPointSize(size);
        f.setWeight(weight);
        f.setItalic(italic);
        f.setStrikeOut(strikeOut);
        f.setUnderline(underline);

        lyrics->setTextFont(f);

        lyrics->setTextColor(QColor(tColor));
        lyrics->setTextBorderColor(QColor(tbColor));
        lyrics->setTextBorderWidth(tbWidth);

        lyrics->setCurColor(QColor(cColor));
        lyrics->setCurBorderColor(QColor(cbColor));
        lyrics->setCurBorderWidth(cbWidth);
    }


    { // Init UI
        ui->frameSearch->hide();
        ui->framePlaylist->hide();
        ui->sliderVolume->setValue(player->volume());

        connect(timer2, SIGNAL(timeout()), ui->frameSearch, SLOT(hide()));
        connect(timer2, SIGNAL(timeout()), ui->framePlaylist, SLOT(hide()));
        connect(timer1, SIGNAL(timeout()), this, SLOT(showCurrentTime()));
        connect(positionTimer, SIGNAL(timeout()), this, SLOT(onPositiomTimerTimeOut()));

        connect(player, SIGNAL(finished()), this, SLOT(onPlayerFinished()));
        connect(player, SIGNAL(playingEvent(MidiEvent*)), this, SLOT(onPlayerPlayingEvent(MidiEvent*)));
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

    delete lyrics;
    delete player;
    delete positionTimer;
    delete timer2;
    delete timer1;
    delete db;
    delete settings;
    delete ui;
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

void MainWindow::play(int index)
{
    stop();
    if (index == -1 && playingSong.id() != "") {
        lyrics->reset();
        player->play();
        lyrics->show();
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

    player->load(p);

    lyrics->setLyrics(playingSong.lyrics(), curPath, player->resolution());
    onPlayerDurationTickChanged(player->duration());
    onPlayerDurationMSChanged(player->durationMS());
    player->play();
    lyrics->show();
    positionTimer->start();
}

void MainWindow::pause()
{
    positionTimer->stop();
    player->pause();
}

void MainWindow::resume()
{
    player->resume();
    positionTimer->start();
}

void MainWindow::stop()
{
    positionTimer->stop();
    player->stop();
    ui->sliderPosition->setValue(0);
    lyrics->hide();
    lyrics->reset();
    ui->sliderPosition->setValue(0);
    onPlayerpositionMSChanged(0);
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

void MainWindow::onPlayerFinished()
{
    if (auto_playnext)
        playNext();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (bgType == 1) {
        setBackgroundImage(bgImg);
        QMainWindow::resizeEvent(event);
    }
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

            if (auto_playnext && playlist.count() == 1 && player->isStoped()) {
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
    QString currentTime = time.toString("hh:mm:ss");
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
    onPlayerpositionMSChanged(player->positionMS());
    ui->sliderPosition->setValue(player->position());
    lyrics->setCursorPosition(player->position() - 100);
}

void MainWindow::onPlayerDurationMSChanged(qint64 d)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(d);
    ui->lbDuration->setText(t.toString("mm:ss"));
}

void MainWindow::onPlayerpositionMSChanged(qint64 p)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(p);
    ui->lbPosition->setText(t.toString("mm:ss"));
}

void MainWindow::onPlayerDurationTickChanged(int d)
{
    ui->sliderPosition->setMaximum(d);
}

void MainWindow::onSliderPositionPressed()
{
    playAfterSeek = player->isPlaying();
    pause();
}

void MainWindow::onSliderPositionReleased()
{
    if (player->isStoped()) {
        ui->sliderPosition->setValue(0);
        return;
    }
    player->setPosition(ui->sliderPosition->value());
    lyrics->setSeekPosition(ui->sliderPosition->value());
    onPlayerpositionMSChanged(player->positionMS());
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
    if (player->isPaused()) {
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

void MainWindow::onPlayerPlayingEvent(MidiEvent *e)
{
    switch (e->eventType()) {
    case MidiEventType::Meta: {

        switch (e->metaEventType()) {
        case MidiMetaType::TimeSignature:
            qDebug() << e->data()[0];
            qDebug() << e->data()[1];
            qDebug() << e->data()[2];
            qDebug() << e->data()[3];
            qDebug() << "=====================";
            break;
        default:
            break;
        }

        break;
    }
    default:
        break;
    }
}
