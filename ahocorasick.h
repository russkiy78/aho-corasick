#ifndef AHOCORASICK_H
#define AHOCORASICK_H

#include <queue>

#include <QVector>
#include <QMap>
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QFileInfo>
#include <QDir>

class AhoCorasick
{


private:

    QMap<int, QVector<int>> outLinks; // state, <ids>
    QMap<int, int> failureTree;
    QMap<int, int> wordLength;
    QMap<int, QMap<int, int>> machineTree;

    int findNextState(int currentState, int nextInput);
    bool saveToFile(QString valueName, QString fileName);


public:
    AhoCorasick();

    bool buildMachine(QMap<int, QString> &input, bool calculateWordLength = false);
    bool loadMachineFromFile(QString dirName, bool getWordLength = false);
    bool saveMachineToFile(QString dirName, bool loadWordLength = false);
    QVector<QPair<int,QVector<int>>> findInText(QString text);
    int findExactly(QString text);
    int getWordLength(int word);

};

#endif // AHOCORASICK_H
