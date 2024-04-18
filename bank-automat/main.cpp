#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


#ifdef Q_OS_WIN
    // Windows path handling
    QDir dir("..\\styles");
    QString stylesFolder = dir.absolutePath();
    QString cssFilePath = stylesFolder + "\\myStyle.qss";
#elif defined(Q_OS_MACOS)
    // macOS path handling
    QString stylesFolder;
    stylesFolder = QCoreApplication::applicationDirPath() + "/../../../../styles";
    QString cssFilePath = stylesFolder + "/myStyle.qss";

#endif

// #ifdef Q_OS_WIN
//         // Windows path handling
//     stylesFolder = QCoreApplication::applicationDirPath() + "/styles";
// #elif defined(Q_OS_MACOS)
//         // macOS path handling
//     stylesFolder = QCoreApplication::applicationDirPath() + "/../../../../styles";
// #endif


    QFile file(cssFilePath);

    if (!file.open(QFile::ReadOnly)) {
        // Tiedoston avaaminen epäonnistui
        qDebug()<<"Tyylitiedoston avaus epäonnistui: " << file.errorString();
        //qDebug() << "Current folder: " << QDir::currentPath();
            return -1;
    }

    QString styleSheet = QLatin1String(file.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return a.exec();
}
