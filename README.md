# aho-corasick
Qt C++ implementation of  Aho–Corasick algorithm with save/load to disk functionality.

# API functions

## Build the Machine

buildMachine (QMap<int, QString> &input, bool calculateWordLength)
* input - QMap of words dictionary (wird id, word)
* calculateWordLength - if  true, it calculates word's length and storage it in machine's save file

```
QMap<int, QString> input {{1,"ba"},{2,"aa"}};
AhoCorasick aho;
aho.buildMachine(input, true);
```
## Find in text
QVector<QPair<int,QVector<int>>> findInText(QString text);

* text - input text
* return: QVector of QPair(word id, QVector(founded positions))

```
QMap<int, QString> input {{1,"ba"},{2,"aa"}};
AhoCorasick aho;
aho.buildMachine(input, true);
QVector<QPair<int,QVector<int>>>matches =  aho.findInText(QString("baa"));
// return QVector((1,1),(1,2))
```

## Find for whole text compliance (similar to regex(^word$))
int AhoCorasick::findExactly(QString text)
* text - input text
* return:  id of founded word or -1 
```
QMap<int, QString> input {{1,"ba"},{2,"aa"}};
AhoCorasick aho;
aho.buildMachine(input, true);
int match = aho.findExactly(QString("ba"));
// return  1
```

## Save machine into the file
bool saveMachineToFile(QString dirName, bool loadWordLength = false);
* dirName - name of directory to save (create directory with follow files:
failureTree.dat,  machineTree.dat,  outLinks.dat, wordLength.dat)
* loadWordLength - save also words length
* return - true if success
```
QMap<int, QString> input {{1,"ba"},{2,"aa"}};
AhoCorasick aho;
aho.buildMachine(input, true);
aho.saveMachineToFile("/tmp/new_machine");
```

## Load machine from the file
bool loadMachineFromFile(QString dirName, bool loadWordLength = false);
* dirName - name of directory to save (create directory with follow files:
failureTree.dat,  machineTree.dat,  outLinks.dat, wordLength.dat)
* loadWordLength - load also words length
* return - true if success
```
QMap<int, QString> input {{1,"ba"},{2,"aa"}};
AhoCorasick aho;
aho.buildMachine(input, true);
aho.saveMachineToFile("/tmp/new_machine", true);
aho.loadMachineFromFile("/tmp/new_machine", true);
```
## Get word length from saved machine
int getWordLength(int word);
* word - id of word
* return: lenght or 0 if not found
_work only if calculateWordLength=true when build machine_
 ```
 QMap<int, QString> input {{1,"ba"},{2,"aa"}};
 AhoCorasick aho;
 aho.buildMachine(input, true);
 aho.getWordLength(1)
// return: 2
 ```


