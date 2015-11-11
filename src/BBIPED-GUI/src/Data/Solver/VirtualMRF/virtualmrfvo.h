#ifndef VIRTUALMRFVO_H
#define VIRTUALMRFVO_H
#include <QString>
#include <QStringList>

class VirtualMrfVO
{
public:
    VirtualMrfVO();

    QString getZoneName();
    QString getOmega();
    QStringList getAxisCenter();
    QStringList getAxisNormal();
    QStringList getInnerCurvePoints();
    QStringList getOuterCurvePoints();
    bool isCoordinatesIn3D();


   void  setZoneName(QString  _val);
   void  setOmega(QString  _val);
   void  setAxisCenter(QStringList  _list);
   void  setAxisNormal(QStringList  _list);
   void  setInnerCurvePoints(QStringList  _list);
   void  setOuterCurvePoints(QStringList  _list);
   void setCoordinatesIn3D(bool _val);

   bool isEmpty();

private:


    QString _zoneName;
    QString _omega;
    QStringList _axisCenter;
    QStringList _axisNormal;
    QStringList _innerCurvePoints;
    QStringList _outerCurvePoints;

    bool _coordinatesIn3D;


};

#endif // VIRTUALMRFVO_H
