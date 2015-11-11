# -*- coding: iso-8859-1 -*-

###
### This file is generated automatically by SALOME v7.2.0 with dump python functionality
###

import sys
import salome
########### CHECK FROM HERE
###### THIS NEEDED TO BE CHANGED ACCORDING USER SETTINGS
#_AutomatizationPath="/home/ima/temp/.tools/Automatization/"
_AutomatizationPath="/home/BCAMATH/calonso/ToolDevelopment/workspace/CFD_REPO/build/debug/.tools/Automatization/"
sys.path.append(_AutomatizationPath)
########## TO HERE
from JobManager import *

salome.salome_init()
theStudy = salome.myStudy
theStudyMan = salome.myStudyManager
AllStudiesNames=theStudyMan.GetOpenStudies()

for _StudyIter in range(len(AllStudiesNames)-1):
	_studyID = theStudyMan.GetStudyByName(AllStudiesNames[_StudyIter])
	print "for closign %s" % (_studyID,)
	_studyID.Close()

import salome_notebook
notebook = salome_notebook.NoteBook(theStudy)
sys.path.insert( 0, r'/home/imanol/temp')

####################################################
##       Begin of NoteBook variables section      ##
####################################################


notebook.set("Blade_N1x", 1)
notebook.set("Blade_N1y", 1)
notebook.set("Blade_N2x", 1)
notebook.set("Blade_N2y", 1)
notebook.set("Blade_N3x", 1)
notebook.set("Blade_N3y", 1)
notebook.set("Blade_N4x", 1)
notebook.set("Blade_N4y", 1)
notebook.set("Blade_N5x", 1)
notebook.set("Blade_N5y", 1)
notebook.set("Blade_Lx", 1)
notebook.set("Blade_Ly", 1)
notebook.set("Blade_N1px", 1)
notebook.set("Blade_N1py", 1)
notebook.set("Blade_Sx", 1)
notebook.set("Blade_Sy", 1)
notebook.set("Blade_Ri", 1)
notebook.set("Blade_Ro", 1)
notebook.set("Blade_Thickness", 1)
notebook.set("Halve_Blade_Thickness", 1)
notebook.set("Minus_Blade_Thickness", 1)
notebook.set("Blade_Thickness_ByMinusThree", 1)
notebook.set("Blade_Height", 1)
notebook.set("Blade_LeadingEdge_Cut_Height", 1)
notebook.set("Blade_TrailingEdge_Cut_Height", 1)
notebook.set("Blade_CutNodeY", 1)
notebook.set("Blade_CutNodeX", 1)
notebook.set("Blade_Number", 1)
notebook.set("Vol_Height", 1)
notebook.set("Vol_n6x", 1)
notebook.set("Vol_n6y", 1)
notebook.set("Vol_nstart_x", 1)
notebook.set("Vol_nstart_y", 1)
notebook.set("Vol_n0x_abs", 1)
notebook.set("Vol_n0y_abs", 1)
notebook.set("Vol_n1x", 1)
notebook.set("Vol_n1y", 1)
notebook.set("Vol_n2x", 1)
notebook.set("Vol_n2y", 1)
notebook.set("Vol_n3x", 1)
notebook.set("Vol_n3y", 1)
notebook.set("Vol_n4x", 1)
notebook.set("Vol_n4y", 1)
notebook.set("Vol_n5x", 1)
notebook.set("Vol_n5y", 1)
notebook.set("Rot_in", 1)
notebook.set("Rot_out", 1)
notebook.set("Rot_in", 1)
notebook.set("Rot_out", 1)
notebook.set("Rot_Height", 1)
notebook.set("Rot_Internal_tongeangle", 1) # UNUSEEEEEED
notebook.set("Rot_Internal_and_exit_tongue_separation", 1) # UNUSEEEEEED
notebook.set("Rot_Internal_tongLength", 1) # UNUSEEEEEED
notebook.set("Rot_out_unit_step", 1)
notebook.set("Rot_out_ceiling_fraction", 1)
notebook.set("InletRadius", 1) 
notebook.set("Mesh_VOLUTE_max_area", 1)
notebook.set("Mesh_VOLUTE_min_area", 1)
notebook.set("Mesh_ROTOR_max_area", 1)
notebook.set("Mesh_ROTOR_min_area", 1)
notebook.set("Mesh_INLET_max_area", 1)
notebook.set("Mesh_INLET_min_area", 1)
notebook.set("HugeValue", 1)



####################################################
##        End of NoteBook variables section       ##
####################################################
###
### GEOM component
###

import GEOM
from salome.geom import geomBuilder
import math
import SALOMEDS

# Definitions
print "Computing Geometry"

geompy = geomBuilder.New(theStudy)

O = geompy.MakeVertex(0, 0, 0)
OX = geompy.MakeVectorDXDYDZ(1, 0, 0)
OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)

ShapeTypeFace = geompy.ShapeType["FACE"]

# Blade Creation


geomObj_5 = geompy.MakeVertex(0, 0, 0)
geomObj_11 = geompy.MakeMarker(0, 0, 0, 1, 0, 0, 0, 1, 0)
Blade_N1 = geompy.MakeVertex("Blade_N1x", "Blade_N1y", 0)
Blade_N2 = geompy.MakeVertex("Blade_N2x", "Blade_N2y", 0)
Blade_N3 = geompy.MakeVertex("Blade_N3x", "Blade_N3y", 0)
Blade_N4 = geompy.MakeVertex("Blade_N4x", "Blade_N4y", 0)
Blade_L = geompy.MakeVertex("Blade_Lx", "Blade_Ly", 0)
Blade_N5 = geompy.MakeVertex("Blade_N5x", "Blade_N5y", 0)
Blade_CutNode1 = geompy.MakeVertex("Blade_CutNodeX", "Blade_CutNodeY","Blade_Height")
Blade_CutNode2 = geompy.MakeVertex("Blade_N1x", "Blade_N1y","Blade_LeadingEdge_Cut_Height")
Blade_CutNode3 = geompy.MakeVertex("Blade_N5x", "Blade_N5y","Blade_LeadingEdge_Cut_Height")
Blade_CutNode4 = geompy.MakeVertex("Blade_N2x", "Blade_N2y","Blade_TrailingEdge_Cut_Height")
Blade_CutNode5 = geompy.MakeVertex("Blade_N3x", "Blade_N3y","Blade_TrailingEdge_Cut_Height")
geomObj_12 = geompy.MakeMarker(0, 0, 0, 1, 0, 0, 0, 1, 0)
Blade_sketch = geompy.MakeSketcherOnPlane("Sketcher:F 'Blade_N4x' 'Blade_N4y':TT 'Blade_N3x' 'Blade_N3y':AA 'Blade_N2x' 'Blade_N2y':TT 'Blade_Lx' 'Blade_Ly':AA 'Blade_N1x' 'Blade_N1y':AA 'Blade_N5x' 'Blade_N5y':AA 'Blade_N4x' 'Blade_N4y'", geomObj_5 )
Blade_Base_Surface = geompy.MakeFaceWires([Blade_sketch], 1)
Blade_HeightOnedotTwo = notebook.get("Blade_Height")*1.2
RawBlade = geompy.MakePrismVecH(Blade_Base_Surface, OZ, Blade_HeightOnedotTwo)

LECutPlane = geompy.MakePlaneThreePnt(Blade_CutNode3, Blade_CutNode2, Blade_CutNode1, "HugeValue")
LECutPlaneVector_Normal = geompy.GetNormal(LECutPlane)
LECutPlaneExtrusion = geompy.MakePrismVecH(LECutPlane, LECutPlaneVector_Normal, "HugeValue")

TECutPlane = geompy.MakePlaneThreePnt(Blade_CutNode1, Blade_CutNode4, Blade_CutNode5, "HugeValue")
TECutPlaneVector_Normal = geompy.GetNormal(TECutPlane)
TECutPlaneExtrusion = geompy.MakePrismVecH(TECutPlane, TECutPlaneVector_Normal, "HugeValue")

LECutedBlade = geompy.MakeCut(RawBlade, LECutPlaneExtrusion)

# FILLET
LECutedBladeSectionID = geompy.GetShapesOnPlaneIDs(LECutedBlade, geompy.ShapeType["FACE"], LECutPlaneVector_Normal, GEOM.ST_ON)
BladeFillet_thickness = notebook.get("Blade_Thickness")*0.35
BladeFillet = geompy.MakeFillet(LECutedBlade,BladeFillet_thickness,  geompy.ShapeType["FACE"], LECutedBladeSectionID)

TECutedBlade = geompy.MakeCut(BladeFillet, TECutPlaneExtrusion)
geompy.TranslateVectorDistance(TECutedBlade, OZ, "Rot_out_unit_step", False)

#Multi_Rotation_1 = geompy.MultiRotate1DNbTimes("Rot_out_unit_step", OZ, "Blade_Number")

# ROTOR CREATION
ForSketchPlane = geompy.MakePlane(O, OX, "HugeValue")

Rot_sketch = geompy.MakeSketcherOnPlane("Sketcher:F 'Rot_out_unit_step' 0.000000:TT 'Rot_out_unit_step' 'Rot_out':TT 0.000000 'Rot_out':TT 0.000000 'HugeValue':TT 'Vol_Height' 'HugeValue':TT 'Vol_Height' 'InletRadius':TT 'Vol_n5y' 'InletRadius':TT 'Vol_n5y' 0.000000:WW", ForSketchPlane )
Rot_sketch_face = geompy.MakeFaceWires([Rot_sketch], 1)
#[Edge_15,Edge_16,Edge_17,Edge_18,Edge_19,Edge_20,Edge_21,Edge_22] = geompy.ExtractShapes(Rot_sketch_face, geompy.ShapeType["EDGE"], True)
TECutedFace = geompy.GetShapesOnShapeAsCompound(TECutPlaneExtrusion, TECutedBlade, geompy.ShapeType["FACE"], GEOM.ST_ONIN)
TECutedFaceRevol = geompy.MakeRevolution(TECutedFace, OZ, 349.9999999999999*math.pi/180.0)
PartitionBladeAndRotorFace = geompy.MakePartition([Rot_sketch_face], [TECutedFaceRevol], [], [], geompy.ShapeType["VERTEX"], 0, [], 0)
GluedPartition = geompy.MakeGlueEdges(PartitionBladeAndRotorFace, "Blade_Thickness")
Rot_sketch_face_vertices = geompy.ExtractShapes(Rot_sketch_face, geompy.ShapeType["VERTEX"], True)
GluedPartition_vertices = geompy.ExtractShapes(GluedPartition, geompy.ShapeType["VERTEX"], True)

SharedVertices = []
for _testGlued in GluedPartition_vertices:
	for _testSketch in Rot_sketch_face_vertices:
		_GluedVertexCoor=geompy.PointCoordinates(_testGlued)
		_SketchVertexCoor=geompy.PointCoordinates(_testSketch)
		_same = (_GluedVertexCoor == _SketchVertexCoor)
		if _same:
			SharedVertices.append(_testGlued)
			break
		pass

GluedStandAloneVertices = list(set(GluedPartition_vertices)-set(SharedVertices))

_BladeCeilingy = []

for _threeVertex in GluedStandAloneVertices:
	_BladeCeilingy.append(geompy.PointCoordinates(_threeVertex)[1])

_max  = 0
maxIndex = 4
minIndex = 4
for i in range(len(_BladeCeilingy)):
	if _BladeCeilingy[i] < _max:
		_max = _BladeCeilingy[i]
		maxIndex = i

_min = _max
for j in range(len(_BladeCeilingy)):
	if _BladeCeilingy[j] > _min:
		_min = _BladeCeilingy[j]
		minIndex = j


GluedVertices = []
GluedVertices.append(GluedStandAloneVertices[maxIndex])
GluedVertices.append(GluedStandAloneVertices[minIndex])

GluedVertices_In = GluedVertices[1]
GluedVertices_Out = GluedVertices[0]

GluedVertices_InLocalY = geompy.PointCoordinates(GluedVertices_In)[1] # This is Y coordinate
GluedVertices_InLocalX = geompy.PointCoordinates(GluedVertices_In)[2] * 0.999 # This is z coordinate in global coordinates

GluedVertices_OutLocalY = geompy.PointCoordinates(GluedVertices_Out)[1]# This is Y coordinate
GluedVertices_OutLocalX = geompy.PointCoordinates(GluedVertices_Out)[2] * 0.999 # This is z coordinate in global coordinates

RotorSlope = (GluedVertices_OutLocalX - GluedVertices_InLocalX)/(GluedVertices_InLocalY - GluedVertices_OutLocalY)

_Delta = notebook.get("Rot_out_ceiling_fraction")  * (notebook.get("Blade_Ro") - notebook.get("Blade_Ri"))

ExtendedGluedVertices_OutLocalY = GluedVertices_OutLocalY - _Delta # This is Y coordinate. We need to extend outlet, because trailing edge falls outside of the rotor
ExtendedGluedVertices_OutLocalX = GluedVertices_OutLocalX + _Delta*RotorSlope # This is z coordinate in global coordinates

InletRadiusVerticeLocalY = - notebook.get("InletRadius")
InletRadiusVerticeLocalX = notebook.get("Vol_Height")

notebook.set("GluedVertices_InLocalY",GluedVertices_InLocalY)
notebook.set("GluedVertices_InLocalX",GluedVertices_InLocalX)
notebook.set("ExtendedGluedVertices_OutLocalY",ExtendedGluedVertices_OutLocalY)
notebook.set("ExtendedGluedVertices_OutLocalX",ExtendedGluedVertices_OutLocalX)
notebook.set("InletRadiusVerticeLocalY",InletRadiusVerticeLocalY)
notebook.set("InletRadiusVerticeLocalX",InletRadiusVerticeLocalX)



_Vol_Height = notebook.get("Vol_Height") 
_InletRadius = notebook.get("InletRadius")* (-1)

Rot_Tonge_Vertex = geompy.MakeVertex(0,_InletRadius , _Vol_Height)
LocalCS_1 = geompy.MakeMarker(0, 0, 0, 0, 0, 1, 0, 1, 0)
Minus_Three_Blade_Thickness = notebook.get("Minus_Blade_Thickness") * 3
notebook.set("Minus_Three_Blade_Thickness",Minus_Three_Blade_Thickness )
RotorCutScketchOuter = geompy.MakeSketcherOnPlane("Sketcher:F 'ExtendedGluedVertices_OutLocalX' 'ExtendedGluedVertices_OutLocalY':TT 'GluedVertices_InLocalX' 'GluedVertices_InLocalY':C 'Minus_Three_Blade_Thickness' 90.000000", LocalCS_1 )



RotorSketchInnerEndingPointY = GluedVertices_InLocalY + notebook.get("Rot_Internal_and_exit_tongue_separation") * math.cos(abs(RotorSlope))
RotorSketchInnerEndingPointX = GluedVertices_InLocalX + notebook.get("Rot_Internal_and_exit_tongue_separation") * math.sin(abs(RotorSlope))

notebook.set("RotorSketchInnerEndingPointY",RotorSketchInnerEndingPointY)
notebook.set("RotorSketchInnerEndingPointX",RotorSketchInnerEndingPointX)

RotorSketchInnerStartingPointY = InletRadiusVerticeLocalY - math.cos(notebook.get("Rot_Internal_tongeangle")/360*2*math.pi) * notebook.get("Blade_Height") 
RotorSketchInnerStartingPointX = InletRadiusVerticeLocalX + math.sin(notebook.get("Rot_Internal_tongeangle")/360*2*math.pi) * notebook.get("Blade_Height") 

notebook.set("RotorSketchInnerStartingPointY",RotorSketchInnerStartingPointY)
notebook.set("RotorSketchInnerStartingPointX",RotorSketchInnerStartingPointX)

RotorCutScketchInner = geompy.MakeSketcherOnPlane("Sketcher:F 'RotorSketchInnerStartingPointX' 'RotorSketchInnerStartingPointY': TT 'InletRadiusVerticeLocalX' 'InletRadiusVerticeLocalY':R 0:L 'Rot_Internal_tongLength':AA 'RotorSketchInnerEndingPointX' 'RotorSketchInnerEndingPointY'", LocalCS_1 )

RotorCutScketchInnerExtrusion = geompy.MakePrismVecH2Ways(RotorCutScketchInner, OX, 100)
geompy.Thicken(RotorCutScketchInnerExtrusion, notebook.get("Minus_Blade_Thickness"))


RotorCutScketchOuterExtrusion = geompy.MakePrismVecH2Ways(RotorCutScketchOuter, OX, 100)
geompy.Thicken(RotorCutScketchOuterExtrusion, notebook.get("Minus_Blade_Thickness"))

RotorCutScketchExtrusion = geompy.MakeCompound([RotorCutScketchInnerExtrusion, RotorCutScketchOuterExtrusion])

RotorCutedFace = geompy.MakeCut(Rot_sketch_face, RotorCutScketchExtrusion)
RotorCutedSolid = geompy.MakeRevolution(RotorCutedFace, OZ, 360*math.pi/180.0)
BladeMultiRotation = geompy.MultiRotate1DNbTimes(TECutedBlade, OZ, "Blade_Number")

# VOLUTE CREATION

Volute_sketch = geompy.MakeSketcherOnPlane("Sketcher:F 'Vol_n6x' 'Vol_n6y':TT 'Vol_nstart_x' 'Vol_nstart_y':AA 'Vol_n0x_abs' 'Vol_n0y_abs':AA 'Vol_n1x' 'Vol_n1y':AA 'Vol_n2x' 'Vol_n2y':AA 'Vol_n3x' 'Vol_n3y':AA 'Vol_n4x' 'Vol_n4y':TT 'Vol_n5x' 'Vol_n5y'", geomObj_11 )
Volute_exit_left = geompy.MakeVertex("Vol_n5x", "Vol_n5y", 0)
Vol_exit_tongue = geompy.MakeVertex("Vol_n6x", "Vol_n6y", 0)
Vol_exit_line = geompy.MakeLineTwoPnt(Volute_exit_left, Vol_exit_tongue)
Volute_wire = geompy.MakeWire([Volute_sketch, Vol_exit_line], 1e-07)
Volute_face = geompy.MakeFaceWires([Volute_wire], 1)
VoluteExtrusion = geompy.MakePrismVecH2Ways(Volute_face, OZ, "HugeValue")
CompleteVolute = geompy.MakeCommon(RotorCutedSolid, VoluteExtrusion)


CompleteFan = geompy.MakeCut(CompleteVolute, BladeMultiRotation)

# Mesh refinement of separated tongues

RotorCutScketchOuterRevolution = geompy.MakeRevolution(RotorCutScketchOuter, OZ, 360*math.pi/180.0)
AllRotorCutScketchOuterRevolutionFaces = geompy.ExtractShapes(RotorCutScketchOuterRevolution, geompy.ShapeType["FACE"], True)
#AllMeshRefinementRotorSketchRevolutionFaces = geompy.ExtractShapes(MeshRefinementRotorSketchRevolution, geompy.ShapeType["FACE"], True)


MeshRefinementRotorSketch = geompy.MakeSketcherOnPlane("Sketcher:F 'ExtendedGluedVertices_OutLocalX' 'ExtendedGluedVertices_OutLocalY':TT 'GluedVertices_InLocalX' 'GluedVertices_InLocalY'", LocalCS_1 )
MeshRefinementRotorSketchRevolution = geompy.MakeRevolution(MeshRefinementRotorSketch, OZ, 360*math.pi/180.0)
MeshRefinementRotorSketchRevolutionExtrusion = geompy.MakePrismVecH(MeshRefinementRotorSketchRevolution, OZ, -notebook.get("Blade_Thickness"))
[MeshRefinementRotorSketchRevolutionExtrusionSolid] = geompy.ExtractShapes(MeshRefinementRotorSketchRevolutionExtrusion, geompy.ShapeType["SOLID"], True)
MeshRefinementRotorFaceOpposite = geompy.GetShapesOnShapeAsCompound(MeshRefinementRotorSketchRevolutionExtrusionSolid, RotorCutScketchOuterRevolution, geompy.ShapeType["FACE"], GEOM.ST_ON)
[MeshRefinementRotorFaceOppositeFace] = geompy.ExtractShapes(MeshRefinementRotorFaceOpposite, geompy.ShapeType["FACE"], True)
#MeshRefinementRotorFaceOppositeFaceID = geompy.GetSameIDs(MeshRefinementRotorSketchRevolution, MeshRefinementRotorFaceOppositeFace[0])

MeshRefinementRotorFace=[]
for _face in AllRotorCutScketchOuterRevolutionFaces:
	if MeshRefinementRotorFaceOppositeFace.IsSame(_face):
		continue
	MeshRefinementRotorFace.append(_face)

MeshRefinementRotorFaceExtrusion = geompy.MakePrismVecH(MeshRefinementRotorFace[0], OZ, -notebook.get("Blade_Thickness"))
TongeFaceToRefineCompound = geompy.GetShapesOnShapeAsCompound(MeshRefinementRotorFaceExtrusion, CompleteFan, geompy.ShapeType["FACE"], GEOM.ST_ON)
[TongeFaceToRefine] = geompy.ExtractShapes(TongeFaceToRefineCompound, geompy.ShapeType["FACE"], True)
TongeFaceToRefineID = geompy.GetSameIDs(CompleteFan, TongeFaceToRefine)


# Bourndary Grouping

OutletFace = geompy.MakePrismVecH(Vol_exit_line, OZ, "Vol_Height")
InletDiskHeight = geompy.MakeDiskR("InletRadius", 1)
geompy.TranslateDXDYDZ(InletDiskHeight, 0, 0, "Vol_n5y")
InletDiskExtrusion = geompy.MakePrismVecH(InletDiskHeight, OZ, 1)
InletFaceCommon = geompy.GetShapesOnShapeAsCompound(InletDiskExtrusion, CompleteFan, geompy.ShapeType["FACE"], GEOM.ST_ON)
InletFaces = geompy.SubShapeAll(InletFaceCommon, geompy.ShapeType["FACE"])
listInletSameIDs = geompy.GetSameIDs(CompleteFan, InletFaces[0])
InletFace = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])
geompy.UnionIDs(InletFace, listInletSameIDs)

OutletFaceExtrusion = geompy.MakePrismVecH(OutletFace, OY, 1)
OutletFaceCommon = geompy.GetShapesOnShapeAsCompound(OutletFaceExtrusion, CompleteFan, geompy.ShapeType["FACE"], GEOM.ST_ON)
OutletFaces = geompy.SubShapeAll(OutletFaceCommon, geompy.ShapeType["FACE"])
listOutletSameIDs = geompy.GetSameIDs(CompleteFan, OutletFaces[0])
OutletFace = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])
geompy.UnionIDs(OutletFace, listOutletSameIDs)


		# ALL FAN FACES


AllFanFaces = geompy.SubShapeAll(CompleteFan, geompy.ShapeType["FACE"])
# AllCompleteVoluteFaces = geompy.SubShapeAll(CompleteVolute, geompy.ShapeType["FACE"])
ComonFanAndVoluteFacesCompound= geompy.GetShapesOnShapeAsCompound(CompleteVolute, CompleteFan, geompy.ShapeType["FACE"], GEOM.ST_ON)
ComonFanAndVoluteFaces = geompy.SubShapeAll(ComonFanAndVoluteFacesCompound, geompy.ShapeType["FACE"])

AllFanFacesIDs=[]
for _face in AllFanFaces:
	_ID = geompy.GetSubShapeID(CompleteFan,_face)
	AllFanFacesIDs.append(_ID)
	_ID=-1


		# BLADE FACES

BladeMultiRotatExplosion = geompy.ExtractShapes(BladeMultiRotation, geompy.ShapeType["SOLID"], True)
Common_MultiRotatCompleteFan_FacesCompList = []
for _solid in BladeMultiRotatExplosion:
	_foo = geompy.GetShapesOnShapeAsCompound(_solid, CompleteFan, geompy.ShapeType["FACE"], GEOM.ST_ON)
	Common_MultiRotatCompleteFan_FacesCompList.append(_foo)


BladeFacesIDs = []
for _BladeFacesCompound in Common_MultiRotatCompleteFan_FacesCompList:
	_BladeFaceList = geompy.SubShapeAll(_BladeFacesCompound, geompy.ShapeType["FACE"])
	for _face in _BladeFaceList:
		_ID = geompy.GetSubShapeID(CompleteFan,_face)
		BladeFacesIDs.append(_ID)
		_ID=-1


		# WALL FACES
WallsFaces = list(set(AllFanFacesIDs)-set(BladeFacesIDs)-set(listInletSameIDs)-set(listOutletSameIDs))


		# LEADING EDGE FACES

#BladeLeadingEdge = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])

CutRadious = math.sqrt(notebook.get("Blade_CutNodeY")**2 + notebook.get("Blade_CutNodeX") **2) *1.02
Cylinder_1 = geompy.MakeCylinderRH(CutRadious, "HugeValue")

_InsideCylinder = geompy.GetShapesOnShapeAsCompound(Cylinder_1, CompleteFan, geompy.ShapeType["FACE"], GEOM.ST_IN)
_InsideCylinderFaceList = geompy.SubShapeAll(_InsideCylinder, geompy.ShapeType["FACE"])
_InsideCylinderFaceListIDs = []
for _face in _InsideCylinderFaceList:
	_ID = geompy.GetSubShapeID(CompleteFan,_face)
	_InsideCylinderFaceListIDs.append(_ID)


LEfaceIDs = []
for _faceID in _InsideCylinderFaceListIDs:
	if _faceID in BladeFacesIDs:
		LEfaceIDs.append(_faceID)

Blades = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])
SeparationTonge = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])
BE_Walls = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])
NOEXPORT_LeadingEdge = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"])
InsideCylinder = geompy.CreateGroup(CompleteFan, geompy.ShapeType["FACE"]) ##borrar
geompy.UnionIDs(Blades, BladeFacesIDs)
geompy.UnionIDs(SeparationTonge, TongeFaceToRefineID)
geompy.UnionIDs(BE_Walls, WallsFaces)
geompy.UnionIDs(NOEXPORT_LeadingEdge, LEfaceIDs)
geompy.UnionIDs(InsideCylinder, _InsideCylinderFaceListIDs) ##borrar




		## ADD TO STUDY ##


#geompy.addToStudy( O, 'O' )
#geompy.addToStudy( OX, 'OX' )
#geompy.addToStudy( OY, 'OY' )
#geompy.addToStudy( OZ, 'OZ' )


#geompy.addToStudy( Blade_N1, 'Blade_N1' )
#geompy.addToStudy( Blade_N2, 'Blade_N2' )
#geompy.addToStudy( Blade_N3, 'Blade_N3' )
#geompy.addToStudy( Blade_N4, 'Blade_N4' )
#geompy.addToStudy( Blade_N5, 'Blade_N5' )
#geompy.addToStudy( Blade_CutNode1, 'Blade_CutNode1' )# borrar
#geompy.addToStudy( Blade_CutNode2, 'Blade_CutNode2' )# borrar
#geompy.addToStudy( Blade_CutNode3, 'Blade_CutNode3' )# borrar
#geompy.addToStudy( Blade_CutNode4, 'Blade_CutNode4' )# borrar
#geompy.addToStudy( Blade_CutNode5, 'Blade_CutNode5' )# borrar
#geompy.addToStudy( TECutPlane, 'TECutPlane' )# borrar
#geompy.addToStudy( Blade_L, 'Blade_L' )
#geompy.addToStudy( Blade_sketch, 'Blade_sketch' )
#geompy.addToStudy( Blade_Base_Surface, 'Blade_Base_Surface' )
#geompy.addToStudy( RawBlade, 'RawBlade' )
#geompy.addToStudy( LECutPlane, 'LECutPlane' )
#geompy.addToStudy( LECutPlaneVector_Normal, 'LECutPlaneVector_Normal' )
#geompy.addToStudy( LECutPlaneExtrusion, 'LECutPlaneExtrusion' )
#geompy.addToStudy( LECutedBlade, 'LECutedBlade' )
#geompy.addToStudy( BladeFillet, 'BladeFillet' )
#geompy.addToStudy( TECutPlaneExtrusion, 'TECutPlaneExtrusion' ) # borrar
#geompy.addToStudy( TECutedBlade, 'TECutedBlade' )
#geompy.addToStudy( Rot_sketch, 'Rot_sketch' ) 
#geompy.addToStudy( Rot_sketch_face, 'Rot_sketch_face' ) 
#geompy.addToStudy( RotorRevolution, 'RotorRevolution' ) 
#geompy.addToStudy( TECutedFace, 'TECutedFace' ) 
#geompy.addToStudy( TECutedFaceRevol, 'TECutedFaceRevol' ) 
#geompy.addToStudy( PartitionBladeAndRotorFace, 'PartitionBladeAndRotorFace' ) 
#geompy.addToStudy( GluedPartition, 'GluedPartition' ) 
#geompy.addToStudy( RotorRevolution, 'RotorRevolution' ) 

#geompy.addToStudy( GluedVertices_In, 'GluedVertices_In' )
#geompy.addToStudy( GluedVertices_Out, 'GluedVertices_Out' )
#geompy.addToStudy( Rot_Tonge_Vertex, 'Rot_Tonge_Vertex' )
#geompy.addToStudy( LocalCS_1, 'LocalCS_1' )
#geompy.addToStudy( RotorCutScketchExtrusion, 'RotorCutScketchExtrusion' )
#geompy.addToStudy( RotorCutScketchOuter, 'RotorCutScketchOuter' )
#geompy.addToStudy( RotorCutedFace, 'RotorCutedFace' )
#geompy.addToStudy( RotorCutedSolid, 'RotorCutedSolid' )
#geompy.addToStudy( BladeMultiRotation, 'BladeMultiRotation' )
#geompy.addToStudy( CompleteVolute, 'CompleteVolute' )
#geompy.addToStudy( VoluteExtrusion, 'VoluteExtrusion' )

#geompy.addToStudy( Volute_face, 'Volute_face' )

geompy.addToStudy( CompleteFan, 'CompleteFan' )

#geompy.addToStudy( ComonFanAndVoluteFacesCompound, 'ComonFanAndVoluteFacesCompound' )


#geompy.addToStudy( BladeFaces, 'BladeFaces' )
#geompy.addToStudy( Cylinder_1, 'Cylinder_1' )

geompy.addToStudyInFather( CompleteFan, InletFace, 'InletFace' )
geompy.addToStudyInFather( CompleteFan, OutletFace, 'OutletFace' )
geompy.addToStudyInFather( CompleteFan, Blades, 'Blades' )
geompy.addToStudyInFather( CompleteFan, SeparationTonge, 'SeparationTonge' )
geompy.addToStudyInFather( CompleteFan, BE_Walls, 'BE_Walls' )
geompy.addToStudyInFather( CompleteFan, NOEXPORT_LeadingEdge, 'NOEXPORT_LeadingEdge' )
#geompy.addToStudyInFather( CompleteFan, InsideCylinder, 'InsideCylinder' )



		## MESHING ##

_Rot_Height = notebook.get("Rot_Height")
_Blade_Thickness = notebook.get("Blade_Thickness")


###
### SMESH component
###

import  SMESH, SALOMEDS
from salome.smesh import smeshBuilder

smesh = smeshBuilder.New(theStudy)
from salome.StdMeshers import StdMeshersBuilder
from salome.NETGENPlugin import NETGENPluginBuilder

MyMesh = smesh.Mesh(CompleteFan)

NETGEN_2D = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D)
GEneralFan2DAnd1DHyp = NETGEN_2D.Parameters()
GEneralFan2DAnd1DHyp.SetMaxSize( (_Rot_Height/30)**2 )
GEneralFan2DAnd1DHyp.SetSecondOrder( 0 )
GEneralFan2DAnd1DHyp.SetOptimize( 1 )
GEneralFan2DAnd1DHyp.SetFineness( 2 )
GEneralFan2DAnd1DHyp.SetMinSize( _Blade_Thickness/10 )
GEneralFan2DAnd1DHyp.SetQuadAllowed( 0 )


NETGEN_3D = MyMesh.Tetrahedron()
GeneralFan3DHyp = NETGEN_3D.Parameters()
GeneralFan3DHyp.SetMaxSize( (_Rot_Height/10)**3 )
GeneralFan3DHyp.SetSecondOrder( 40 )
GeneralFan3DHyp.SetOptimize( 1 )
GeneralFan3DHyp.SetFineness( 2 )
GeneralFan3DHyp.SetMinSize( (_Blade_Thickness/10)**3  )


NETGEN_2D_1 = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D,geom=Blades)
Blades2Dand1DHyp = NETGEN_2D_1.Parameters()
Blades2Dand1DHyp.SetMaxSize( (_Blade_Thickness /3)**2  )
Blades2Dand1DHyp.SetSecondOrder( 0 )
Blades2Dand1DHyp.SetOptimize( 1 )
Blades2Dand1DHyp.SetFineness( 2 )
Blades2Dand1DHyp.SetMinSize( (_Blade_Thickness /4)**2  )
Blades2Dand1DHyp.SetQuadAllowed( 0 )

NETGEN_2D_3 = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D,geom=SeparationTonge)
SeparationTonge2Dand1DHyp = NETGEN_2D_3.Parameters()
SeparationTonge2Dand1DHyp.SetMaxSize( (_Blade_Thickness /3)**2  )
SeparationTonge2Dand1DHyp.SetSecondOrder( 0 )
SeparationTonge2Dand1DHyp.SetOptimize( 1 )
SeparationTonge2Dand1DHyp.SetFineness( 2 )
SeparationTonge2Dand1DHyp.SetMinSize( (_Blade_Thickness /4)**2  )
SeparationTonge2Dand1DHyp.SetQuadAllowed( 0 )


NETGEN_2D_2 = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D,geom=NOEXPORT_LeadingEdge)
LE2Dand1DHyp = NETGEN_2D_2.Parameters()
LE2Dand1DHyp.SetMaxSize((_Blade_Thickness * 0.3)**2  )
LE2Dand1DHyp.SetSecondOrder( 0 )
LE2Dand1DHyp.SetOptimize( 1 )
LE2Dand1DHyp.SetFineness( 2 )
LE2Dand1DHyp.SetMinSize( (_Blade_Thickness /5)**2  )
LE2Dand1DHyp.SetQuadAllowed( 0 )
Blades2dSub = NETGEN_2D_1.GetSubMesh()
LE2dSub = NETGEN_2D_2.GetSubMesh()
SeparationTonge2dSub = NETGEN_2D_2.GetSubMesh()

print "Computing Mesh"

isDone = MyMesh.SetMeshOrder( [ [LE2dSub, Blades2dSub, SeparationTonge2dSub] ])
isDone = MyMesh.Compute()
InletFace = MyMesh.GroupOnGeom(InletFace,'InletFace',SMESH.EDGE)
OutletFace = MyMesh.GroupOnGeom(OutletFace,'OutletFace',SMESH.EDGE)
Blades = MyMesh.GroupOnGeom(Blades,'Blades',SMESH.EDGE)
SeparationTonge = MyMesh.GroupOnGeom(SeparationTonge,'SeparationTonge',SMESH.EDGE)
BE_Walls = MyMesh.GroupOnGeom(BE_Walls,'BE_Walls',SMESH.EDGE)
NOEXPORT_LeadingEdge = MyMesh.GroupOnGeom(NOEXPORT_LeadingEdge,'NOEXPORT_LeadingEdge',SMESH.EDGE)

## set object names
smesh.SetName(MyMesh.GetMesh(), 'MyMesh')
smesh.SetName(NETGEN_2D.GetAlgorithm(), 'NETGEN_2D')
smesh.SetName(GEneralFan2DAnd1DHyp, 'GEneralFan2DAnd1DHyp')
smesh.SetName(NETGEN_3D.GetAlgorithm(), 'NETGEN_3D')
smesh.SetName(GeneralFan3DHyp, 'GeneralFan3DHyp')
smesh.SetName(Blades2Dand1DHyp, 'Blades2Dand1DHyp')
smesh.SetName(LE2Dand1DHyp, 'LE2Dand1DHyp')
smesh.SetName(Blades2dSub, 'Blades2dSub')
smesh.SetName(SeparationTonge2dSub, 'SeparationTonge2dSub')
smesh.SetName(InletFace, 'InletFace')
smesh.SetName(LE2dSub, 'LE2dSub')
smesh.SetName(OutletFace, 'OutletFace')
smesh.SetName(Blades, 'Blades')
smesh.SetName(BE_Walls, 'BE_Walls')
smesh.SetName(NOEXPORT_LeadingEdge, 'NOEXPORT_LeadingEdge')


# Exporting the mesh

dirname = '/home/BCAMATH/igarcia/Programs/BBiped/BBipedWorkingDirectory/CodeBlocks/SU2_CFD/Automatization'

print "Exporting Mesh"

EXPORTMesh(False, dirname, MyMesh, SMESH, SALOMEDS, smeshBuilder,smesh) # if no Multizone, then first argument = False




## SOME DEFINITIONS ##

_update = salome.sg.updateObjBrowser

global _name_iter

_name_iter = 0

def _add(_name):
	global _name_iter
	_name_iter = _name_iter +1
	_name_iter_str = str(_name_iter)
	geompy.addToStudy( _name, _name_iter_str )


if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser(1)
