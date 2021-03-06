#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtXml>
#include <QDomDocument>

namespace Ui
{
    class MainWindowClass;
}

struct Game {
    QString team1, team2, anbieter1, anbieter2, date, time, sport, id;
    float odd1, odd2;
};

class Sportanbieter {
    private:
        QFile file;
        QDomDocument xmlfile;
    public:
        Sportanbieter(QString dateiname);
        ~Sportanbieter();
        QStringList get_sports();
        QString get_name();
        QVector<Game> get_games(QString sport);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
    QVector<Sportanbieter*> anbieter;
    QStringList refresh_cmd;
    QString xml_path;

    void optimize_gamelist(QVector<Game>& gamelist);
    void fill_grid(QVector<Game> gamelist);
    void set_xml_path(QString path);
    void set_refresh_command(QStringList command);
    void updateAnbieterliste();

private slots:
    void on_actionErneuern_triggered();
    void on_action_ber_Qt_triggered();
    void on_actionEinstellungen_triggered();
    void on_sportartenliste_itemSelectionChanged();
    void on_actionBeenden_triggered();
    void on_anbieterCheckBox_clicked();
};

#endif // MAINWINDOW_H
