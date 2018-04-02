#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDirIterator>
#include <QRegularExpression>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("RegexReplacer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt application to replace strings in files using regular expressions");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("path", QCoreApplication::translate("main", "Path to file or directory to replace strings."));
    parser.addPositionalArgument("regexSource", QCoreApplication::translate("main", "Regex to capture strings from file(s)."));
    parser.addPositionalArgument("regexDest", QCoreApplication::translate("main", "Regex with new string to replace in file(s)."));

    QCommandLineOption v("verbose", QCoreApplication::translate("main", "Run application in verbose mode."));
    parser.addOption(v);

    parser.process(a);

    bool verbose = parser.isSet(v);
    const QStringList args = parser.positionalArguments();

    if(args.length() != 3)
    {
        parser.showHelp();
    }

    qDebug() << "Regex source: " << args.at(1);
    QFileInfo path(args.at(0));
    QRegularExpression regexSource(args.at(1));
    QString regexDest(args.at(2));

    if(path.isDir()) // If it's a directory, enter and do the regex replacing in all the files.
    {
        QDirIterator dirIt(path.path(), QDirIterator::NoIteratorFlags);

        while(dirIt.hasNext())
        {
            QString filePathStr = dirIt.next();
            QFileInfo filePath(filePathStr);
            if(!filePath.isFile())
            {
                break;
            }

            QFile f(filePathStr);
            if(!f.open(QIODevice::ReadWrite))
            {
                qDebug() << f.fileName() << ": Unable to open.";
                break;
            }

            qDebug() << f.fileName();
            QString content = QString(f.readAll());
            content.replace(regexSource, regexDest);

            f.resize(0);
            f.write(content.toUtf8());
        }
    }
    else if(path.isFile())
    {
        QFile f(path.filePath());

        if(!f.open(QIODevice::ReadWrite))
        {
            qDebug() << f.fileName() << ": Unable to open.";
        }
        else
        {
            qDebug() << f.fileName();
            QString content = QString(f.readAll());
            content.replace(regexSource, regexDest);

            f.resize(0);
            f.write(content.toUtf8());
        }
    }

    return 0;
}
