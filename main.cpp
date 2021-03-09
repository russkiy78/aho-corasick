#include <QDebug>
#include "ahocorasick.h"

int main()
{
    QMap<int, QString> input;

    AhoCorasick aho;

    input[1] = QString("aasdasba");
    input[2] = QString("ba");
    input[3] = QString("nach");
    input[4] = QString("abbekommen");
    input[5] = QString("abbekosadasdasmmen");
    input[6] = QString("abur");
    input[7] = QString("ur");


    aho.buildMachine(input, true);

    QVector<QPair<int,QVector<int>>>matches = aho.findInText(QString("abur"));
    for (int i=0; i<matches.size(); ++i){
        for (int j=0; j<matches[i].second.size(); ++j){
            qDebug() << "matches id " <<  matches[i].second[j] << "length" << aho.getWordLength(matches[i].second[j]) ;
        }
    }

    return 0;
}
