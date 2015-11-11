#ifndef SOLVERDEFINES_H
#define SOLVERDEFINES_H
#include <QString>
// PATHS
const QString CONFIG_TEMPLATE_PATH = ":/template/Templates/Menu/template_menus_v7_2.xml";


// COLUMN
enum ColumnPhysicsSolver
{
    ColumnCheck = 0,
    ColumnVarName = 1,
    ColumnSelVal = 2,
    ColumnUnit = 3,
    ColumnHelp = 4
};
// MARKER SPECIFIC DEFINES
enum ColumnMarkerSolver
{
    MarkerColumnHelp = 3,
    ColumnMarkerUnitMeasure = 3
};

enum ColumnTemplate{
    ColumnTemplateHelp = 2
};

// XML Defines
const QString ROTATING_FRAME_TITLE = "Rotating Frame";
const QString UNSTEADY_TITLE = "Unsteady";
const QString MARKER_TITLE = "Boundaries Definition"; // Tal y como aparece en el XML file
const QString MARKER_CHILD_TITLE=  "Boundaries General Definition";
const QString MARKER_PLOTTING_TITLE=  "Boundary Plotting Values";
const QString MARKER_MONITORING_TITLE=  "Boundary Monitoring Values";
const QString MARKER_MULTIZONE_TITLE=  "Boundary Multizone";
const QString MESH_TITLE = "Mesh Files";
const QString IO_TITLE = "Input / Output information";

const QString MARKER_PREFIX=  "MARKER";
const QString BC_PREFIX="BC_";

const QString XML_CUSTOMISED_TAG=  "Customised";

const QString XML_ROOT_NODE_NAME=  "solver-template";
const QString XML_SECTION_LIST_NAME = "sections";
const QString XML_SECTION_NODE_NAME=  "section";
const QString XML_TITLE_SECTION_NAME=  "title";
const QString XML_SECTION_ID_NAME=  "id";
const QString XML_SUBSECTION_LIST_NAME = "subsections";
const QString XML_SUBSECTION_NAME = "subsection";
const QString XML_SUBSECTION_TITLE_NAME=  "title";
const QString XML_SUBSECTION_ID_NAME=  "id";
const QString XML_VARIABLE_LIST_NAME = "variables";
const QString XML_VARIABLE_NAME = "variable";
const QString XML_VARIABLE_SELECTABLE_ATT = "isSelectable";
const QString XML_VARIABLE_NAME_NAME = "name";
const QString XML_VARIABLE_HELP_NAME = "help";
const QString XML_VARIABLE_COMBOVALUES_LIST_NAME=  "comboValues";
const QString XML_VARIABLE_COMBOVALUE_ELEMENT_NAME = "comboValue";
const QString XML_VARIABLE_VALUE_NAME=  "value";
const QString XML_VARIABLE_ISREPEATABLE_ATT="isRepeatable";
const QString XML_VARIABLE_ISOPTIONAL_ATT="isOptional";
const QString XML_VARIABLE_ISVISIBLE_ATT="isVisible";
const QString XML_VARIABLE_ISVISIBLEINMENU_ATT="isVisibleInMenu";
const QString XML_VARIABLE_ASSOCIATED_VAR_ATT="associatedTo";

// Help variables
const QString XML_VARIABLE_HELP_DEF_ATT ="definition";
const QString XML_VARIABLE_HELP_VAL_ATT ="values";
const QString XML_VARIABLE_HELP_OTHERDETAILS_ATT ="otherdetails";

// ATTRIBUTES
const QString XML_SECTION_TITLE_IN_BASIC_VIEW_ATT = "inBasicView";
const QString XML_SECTION_TITLE_VISIBLE_CHILDREN_ATT = "visibleChildren";
const QString XML_SUBSECTION_VISIBLE_IN_BASIC_VIEW_ATT = "visibleInBasicView";
const QString XML_VARIABLE_ID_ATT = "id";
const QString XML_VARIABLE_VALUE_NAME_ATT=  "name";
const QString XML_VARIABLE_VALUE_DEFAULT_VALUE_ATT=  "defaultValue";
const QString XML_VARIABLE_VALUE_UNIT_ATT=  "unit";
const QString XML_VARIABLE_COMBOVALUE_NAME_ATT=  "name";
const QString XML_VARIABLE_COMBOVALUE_SELECTABLE_ATT ="isSelectable";
const QString XML_VARIABLE_COMBOVALUE_UNIQUESEL_ATT=  "uniqueSel";
const QString XML_VARIABLE_VALUE_SUBVAR_NAME_ATT=  "subVarNames";


// SPECIFIC MARKER
const QString MARKER_ROTB_NAME = "MARKER_ROTB";
const QString MARKER_PERIODIC_NAME = "MARKER_PERIODIC";
const QString MARKER_INLET_NAME = "MARKER_INLET";
const QString MARKER_OUTLET_NAME=  "MARKER_OUTLET";
const QString BC_FARFIELD_NAME = "BC_FARFIELD";
const QString BC_WALL_NAME = "BC_WALL";

// SPECIFIC VIRTUAL
const QString VIRTUAL_SUBSECTION_NAME="Virtual Frame Definition";
const QString ROTATING_FRAME_SECTION_NAME="Rotating Frame Definition";
const QString VMRF_AXIS_KEYNAME="VMRF_AXIS";
const QString VMRF_INNER_CURVE_KEYNAME="VMRF_INNER_CURVE";
const QString VMRF_OUTER_CURVE_KEYNAME="VMRF_OUTTER_CURVE";
const QString VMRF_GRID_MOVEMENT_KEYNAME="GRID_MOVEMENT";
const QString VMRF_GRID_MOVEMENT_KIND_KEYNAME="GRID_MOVEMENT_KIND";

// SPECIFIC DOMAIN CLIPPING
const QString DOMAIN_CLIPPING_MENU_NAME="Crop Region definition";
const QString DOMAIN_CLIPPING_VARIABLE_NAME="Box domain Clipping";
const QString DOMAIN_CLIPPING_VARIABLE_FILENAME="PARTIAL_OUT";

#endif // SOLVERDEFINES_H
