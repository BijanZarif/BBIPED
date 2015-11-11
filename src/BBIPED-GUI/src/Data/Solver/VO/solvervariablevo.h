#ifndef SOLVERVARIABLEVO_H
#define SOLVERVARIABLEVO_H
#include <QString>
#include <QStringList>
#include "Data/Solver/VO/solvervarhelpvo.h"
#include "Rule/variablerule.h"
#include <QList>


class SolverVariableVO
{
public:
    SolverVariableVO();
    ~SolverVariableVO();

    /************************************ Getters/Setters************************************/

    void setVariableName(QString _newName);
    QString getVariableName();

    void setDefaultValue(QString _newDefaultValue);
    QString getDefaultValue();

    void setHelpContents(SolverVarHelpVO * _newHelpContents);
    QString getHelpContents(bool _withFormat);

    void setComboValuesList(QStringList _newComboValuesList);
    QStringList getComboValuesList();

    void setSelectableComboValues( QList<bool> _list);
    QList<bool> getSelectableComboValues();


    void setComboValueNamesList(QStringList _list);
    QStringList getComboValueNamesList();

    void setVariableFileName(QString _newName);
    QString getVariableFileName();

    void setCurrentValue(QString _newValue);
    QString getCurrentValue();

    void setUnitValue(QString _newValue);
    QString getUnitValue();

    bool isUniqueSelection();
    void setUniqueSelection(bool _val);

    bool isRepeatable();
    void setRepeatable(bool _val);

    bool isOptional();
    void setOptional(bool _val);

    bool isVisibleInMenu();
    void setVisibleInMenu(bool _val);

    bool isVariableSelectable();
    void setVariableSelectable(bool _val);

    QStringList getAssociatedVariables();
    void setAssociatedVariables(QStringList _list);

    void setSubVariableNames(QStringList _newValue);
    QStringList getSubVariableNames();

    SolverVarHelpVO getHelperContents();

    /************************************ Others************************************/

    bool isEmpty();
    bool isVisible();
    void setRule(VariableRule _newRule);
    VariableRule getRule();
    void setVisibility(bool _val);

private:

    QString _name;
    QString _defaultValue;
    SolverVarHelpVO * _helpContents;

    QStringList _comboValueNamesList;
    QStringList _comboValuesList;
    QList<bool> _selectableComboValues;

    QString _fileName;
    QString _currentValue;
    bool _isEmpty;
    bool _isVisible;
    VariableRule _rule;
    QString _unitTxt;

    bool _isUniqueSelection;
    QStringList _subVariableNames;

    bool _isRepeatable;
    bool _isOptional;
    bool _isVisibleInMenu;
    bool _isSelectable;

    QStringList _associatedVariables;



};

#endif // SOLVERVARIABLEVO_H
