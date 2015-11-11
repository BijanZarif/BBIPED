#ifndef SEARCHEDPAIR_H
#define SEARCHEDPAIR_H
#include <QList>
class SearchedPair
{
public:
    SearchedPair();

    /*************************** Getter/Setter ***************************/
    int getIdParent();
    void setIdParent(int _id);

    int getIdChild();
    void setIdChild(int _id);

    QList<int> getChildrenList();
    void setChildrenList(QList<int> _list);

    bool contains(int _nidParent, int _nidChild);

private:
    int _idParent;
    int _idChild;
    QList<int> _listChildren;
};

#endif // SEARCHEDPAIR_H
