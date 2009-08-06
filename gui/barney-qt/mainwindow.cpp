#include "mainwindow.h"
#include "ui_mainwindow.h"

Sportanbieter::Sportanbieter(QString dateiname) : file(dateiname) {
    file.open(QFile::ReadOnly);
    xmlfile.setContent(&file);
}

Sportanbieter::~Sportanbieter() {
    file.close();
}

QStringList Sportanbieter::get_sports() {
    QStringList ret;
    QDomElement s = xmlfile.documentElement().firstChildElement("sport");
    while(s.attribute("name") != "") {
        ret << s.attribute("name");
        s = xmlfile.documentElement().nextSiblingElement("sport");
    }

    return ret;
}

QString Sportanbieter::get_name() {
    return xmlfile.documentElement().attribute("name");
}

QVector<Game> Sportanbieter::get_games(QString sport) {
    QVector<Game> gamelist;

    QDomElement s = xmlfile.documentElement().firstChildElement("sport");
    while(!s.isNull()) {
        if(s.attribute("name") == sport) {
            QDomElement s2 = s.firstChildElement("game");

            Game g;
            while(!s2.isNull()) {
                g.team1 = s2.firstChildElement("team1").text();
                g.team2 = s2.firstChildElement("team2").text();
                g.odd1 = s2.firstChildElement("odd1").text().toFloat();
                g.odd2 = s2.firstChildElement("odd2").text().toFloat();
                g.date = s2.firstChildElement("date").text();
                g.time = s2.firstChildElement("time").text();
                g.id = s2.attribute("id");
                g.anbieter1 = get_name();
                g.anbieter2 = get_name();
                g.sport = sport;
                gamelist.push_back(g);

                s2 = s.nextSiblingElement("game");
            }
        }
        s = xmlfile.documentElement().nextSiblingElement("sport");
    }

    return gamelist;
}

void MainWindow::optimize_gamelist(QVector<Game>& gamelist) {
}

void MainWindow::fill_grid(QVector<Game> gamelist) {
    ui->spielliste->setRowCount(gamelist.size());
    for(int i = 0; i < gamelist.size(); i++) {
        ui->spielliste->setItem(i, 0, new QTableWidgetItem(gamelist.at(i).id));
        ui->spielliste->setItem(i, 1, new QTableWidgetItem(gamelist.at(i).team1));
        ui->spielliste->setItem(i, 2, new QTableWidgetItem(gamelist.at(i).odd1));
        ui->spielliste->setItem(i, 3, new QTableWidgetItem(gamelist.at(i).anbieter1));
        ui->spielliste->setItem(i, 4, new QTableWidgetItem(gamelist.at(i).team2));
        ui->spielliste->setItem(i, 5, new QTableWidgetItem(gamelist.at(i).odd2));
        ui->spielliste->setItem(i, 6, new QTableWidgetItem(gamelist.at(i).anbieter2));
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);

    QStringList xmlfiles;
    xmlfiles << "../../output/pinnacle.xml" << "../../output/expekt.xml" << "../../output/intertops.xml" << "../../output/bwin.xml";

    for (int i = 0; i < xmlfiles.size(); ++i)
        anbieter.push_back(new Sportanbieter(xmlfiles.at(i)));

    ui->anbieterliste->setRowCount(anbieter.size());

    QStringList sportarten;

    for (int i = 0; i < anbieter.size(); i++) {
        ui->anbieterliste->setItem(i, 0, new QTableWidgetItem(anbieter.at(i)->get_name()));
        sportarten << anbieter.at(i)->get_sports();
    }

    sportarten.removeDuplicates();

    ui->sportartenliste->addItems(sportarten);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0; i < anbieter.size(); i++) {
        delete anbieter.at(i);
    }
}

void MainWindow::on_actionBeenden_triggered()
{
    close();
}


void MainWindow::on_sportartenliste_itemSelectionChanged()
{
    QList<QListWidgetItem*> auswahl = ui->sportartenliste->selectedItems();
    QVector<Game> gameslist;

    for(int i = 0; i < auswahl.size(); i++) {
        QString sport = auswahl.at(i)->text();
        for(int j = 0; j < anbieter.size(); j++) {
            gameslist << anbieter.at(j)->get_games(sport);
        }
    }
    optimize_gamelist(gameslist);
    fill_grid(gameslist);
}
