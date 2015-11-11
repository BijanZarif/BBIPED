#ifndef PROJECTDETAILS_H
#define PROJECTDETAILS_H
#include <QString>
class ProjectDetails
{
public:
    ProjectDetails();

    QString getProjectTile();
    void setProjectTile(QString _title);
    QString getAuthor();
    void setAuthor(QString _newAuthor);

    QString getStartingDate();
    void setStartingDate(QString _newDate);

    QString getOtherDetails();
    void setOtherDetails(QString _newDetails);
    void clear();

    void updateProjectDetails(ProjectDetails _newProjectDetails);

private:
    QString _projectTile;
    QString _author;
    QString _startingDate;
    QString _otherDetails;

};

#endif // PROJECTDETAILS_H
