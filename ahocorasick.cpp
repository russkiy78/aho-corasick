#include "ahocorasick.h"

AhoCorasick::AhoCorasick(){

}

bool AhoCorasick::buildMachine(QMap<int, QString> &input, bool calculateWordLength) {

    int states = 1; // Initially, we just have the 0 state

    machineTree.clear();
    outLinks.clear();
    failureTree.clear();
    wordLength.clear();

    QMap<int, QString>::iterator _mainItSet;

    for(_mainItSet = input.begin(); _mainItSet!= input.end(); _mainItSet++){
        const int wordId = _mainItSet.key();
        QString keyword = _mainItSet.value();
        int kewordLength = int(keyword.size());

        // create machine tree
        int currentState = 0;
        for (int j = 0; j < kewordLength; ++j) {
            int c = keyword[j].unicode();
            if (!machineTree.contains(currentState))
                machineTree[currentState] = QMap<int, int>();

            if (!machineTree[currentState].contains(c))
                machineTree[currentState][c] = states++;

            currentState = machineTree[currentState][c];

        }
        outLinks[currentState].append(wordId);

        // add word length if calculateWordLength = true
        if (calculateWordLength)
            wordLength[wordId] = kewordLength;
    }

    // Now, let's build the failure function
    std::queue<int> q;
    QMap<int, int>::iterator _itSet;
    for(_itSet = machineTree[0].begin(); _itSet!= machineTree[0].end(); _itSet++){
        int iterValue = _itSet.value();
        failureTree[iterValue] = 0;
        q.push(iterValue);
    }

    while (q.size())
    {
        int state = q.front();
        q.pop();
        if (machineTree.contains(state)) {

            for(_itSet = machineTree[state].begin(); _itSet!= machineTree[state].end(); _itSet++){
                int iterKey = _itSet.key();
                int iterVal = _itSet.value();

                int failure = 0;
                if (failureTree.contains(state))
                    failure = failureTree[state];

                while (!machineTree.contains(failure) || !machineTree[failure].contains(iterKey)) {
                    if (failure == 0)
                        break;
                    failure = failureTree[failure];
                }

                if (machineTree.contains(failure) && machineTree[failure].contains(iterKey))
                    failure = machineTree[failure][iterKey];


                if (!machineTree.contains(failure))
                    machineTree[failure] = QMap<int, int>();

                failureTree[iterVal] = failure;

                if (outLinks.contains(failure)) {
                    // add founded words to outLinks
                    outLinks[iterVal].append(outLinks[failure]);
                }


                q.push(iterVal);
            }
        }
    }
    return true;
}

int AhoCorasick::findNextState(int currentState, int nextInput)
{
    int answer = currentState;
    while (!machineTree.contains(answer) || !machineTree[answer].contains(nextInput)){
        if (answer == 0 )
            break;
        answer = failureTree[answer];
    }
    if (machineTree[answer].contains(nextInput))
        return machineTree[answer][nextInput];
    return 0;
}

bool AhoCorasick::saveToFile(QString valueName, QString fileName)
{
    QFile saveFile (fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QDataStream in (&saveFile);
    in.setVersion(QDataStream::Qt_5_3);
    if (valueName == QString("outLinks"))
        in << outLinks;
    else if (valueName == QString("failureTree"))
        in << failureTree;
    else if (valueName == QString("machineTree"))
        in << machineTree;
    else if (valueName == QString("wordLength"))
        in << wordLength;
    else
        return false;

    saveFile.flush();
    saveFile.close();
    return true;
}

QVector<QPair<int,QVector<int>>> AhoCorasick::findInText(QString text) {
    int currentState = 0;
    QVector<QPair<int,QVector<int>>> ret;
    int textSize = text.size();
    for (int i=0; i<textSize; ++i) {
        int intChar = text[i].unicode();
        currentState = findNextState(currentState,  intChar);

        if (!outLinks.contains(currentState))
            continue; // Nothing new, let's move on to the next character.

        ret.append(QPair<int,QVector<int>>(i, outLinks[currentState]));
    }

    return ret;
}

int AhoCorasick::findExactly(QString text) {
    int currentState = 0;
    int textSize = text.size();
    for (int i=0; i<textSize; ++i) {
        int intChar = text[i].unicode();
        currentState = findNextState(currentState,  intChar);
        if (i < textSize - 1 || !outLinks.contains(currentState))
            continue;
        return outLinks[currentState][0];
    }
    return -1;
}

int AhoCorasick::getWordLength(int word)
{
    if (wordLength.contains(word))
        return wordLength[word];
    return 0;
}

bool AhoCorasick::loadMachineFromFile(QString dirName, bool getWordLength)
{

    QVector<QString> valueNames;
    valueNames << "outLinks" << "failureTree" << "machineTree";

    if (getWordLength)
        valueNames << "wordLength";

    if (!QDir(dirName).exists()) {
        return false;
    }


    machineTree.clear();
    outLinks.clear();
    failureTree.clear();

    foreach (QString valueName, valueNames) {
        if(!QFileInfo::exists(dirName+"/"+valueName+".dat")) {
            return false;
        }
        QFile outFile (dirName+"/"+valueName+".dat");
        if (!outFile.open(QIODevice::ReadOnly)) {
            return false;
        }
        QDataStream out (&outFile);
        out.setVersion(QDataStream::Qt_5_3);
        if (valueName == QString("outLinks"))
            out >> outLinks;
        else if (valueName == QString("failureTree"))
            out >> failureTree;
        else if (valueName == QString("machineTree"))
            out >> machineTree;
        else if (valueName == QString("wordLength"))
            out >> wordLength;

        outFile.close();
    }

    return true;

}

bool AhoCorasick::saveMachineToFile(QString dirName, bool loadWordLength)
{
    QVector<QString> valueNames;
    valueNames << "outLinks" << "failureTree" << "machineTree";

    if (loadWordLength)
        valueNames << "wordLength";

    if (QDir(dirName).exists()) {
        return false;
    }
    if (!QDir().mkdir(dirName)) {
        return false;
    }

    foreach (QString valueName, valueNames) {
        if (!saveToFile(valueName,dirName+"/"+valueName+".dat"))
            return false;
    }

    return true;
}



