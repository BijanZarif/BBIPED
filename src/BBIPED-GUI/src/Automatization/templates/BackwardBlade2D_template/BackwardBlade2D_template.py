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
	print "closign previos Salome Studies %s" % (_studyID,)
	_studyID.Close()

import salome_notebook
notebook = salome_notebook.NoteBook(theStudy)
sys.path.insert( 0, r'/home/imanol/temp')

####################################################
##       Begin of NoteBook variables section      ##
####################################################


notebook.set("Blade_N1x", 0.000000)
notebook.set("Blade_N1y", 0.300000)
notebook.set("Blade_N2x", -0.353553)
notebook.set("Blade_N2y", 0.353553)
notebook.set("Blade_N3x", -0.353379)
notebook.set("Blade_N3y", 0.348556)
notebook.set("Blade_N4x", -0.042873)
notebook.set("Blade_N4y", 0.337701)
notebook.set("Blade_N5x", -0.004997)
notebook.set("Blade_N5y", 0.299826)
notebook.set("Blade_Lx", -0.042698)
notebook.set("Blade_Ly", 0.342698)
notebook.set("Blade_N1px", -0.9397)
notebook.set("Blade_N1py", 0.342)
notebook.set("Blade_Sx", -0.9397)
notebook.set("Blade_Sy", 0.342)
notebook.set("Blade_Ri", 0.300000)
notebook.set("Blade_Ro", 0.500000)
notebook.set("Blade_Thickness", 0.005000)
notebook.set("Halve_Blade_Thickness", 0.002500)
notebook.set("Minus_Blade_Thickness", -0.005000)
notebook.set("Blade_Number", 13)
notebook.set("Vol_n6x", -0.11106)
notebook.set("Vol_n6y", 0.872318)
notebook.set("Vol_nstart_x", -0.32)
notebook.set("Vol_nstart_y", 0.6275)
notebook.set("Vol_n0x_abs", -0.287601)
notebook.set("Vol_n0y_abs", 0.590229)
notebook.set("Vol_n1x", 0)
notebook.set("Vol_n1y", 0.687035)
notebook.set("Vol_n2x", 0.80746)
notebook.set("Vol_n2y", 0)
notebook.set("Vol_n3x", 0)
notebook.set("Vol_n3y", -0.949634)
notebook.set("Vol_n4x", -1.03966)
notebook.set("Vol_n4y", 0)
notebook.set("Vol_n5x", -1.04106)
notebook.set("Vol_n5y", 0.870695)
notebook.set("Rot_in", 0.270000)
notebook.set("Rot_out", 0.580000)
notebook.set("TwoD_inlet", 0.15)
notebook.set("Mesh_VOLUTE_max_area", 0.020)
notebook.set("Mesh_VOLUTE_min_area", 0.010)
notebook.set("Mesh_ROTOR_max_area", 0.00800)
notebook.set("Mesh_ROTOR_min_area", 0.0006)
notebook.set("Mesh_INLET_max_area", 0.015000)
notebook.set("Mesh_INLET_min_area", 0.001000)
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

print "Computing Geometry"

geompy = geomBuilder.New(theStudy)

O = geompy.MakeVertex(0, 0, 0)
OX = geompy.MakeVectorDXDYDZ(1, 0, 0)
OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
geomObj_1 = geompy.MakeVertex(0, 0, 0)
geomObj_2 = geompy.MakeVectorDXDYDZ(1, 0, 0)
geomObj_3 = geompy.MakeVectorDXDYDZ(0, 1, 0)
geomObj_4 = geompy.MakeVectorDXDYDZ(0, 0, 1)
geomObj_5 = geompy.MakeVertex(0, 0, 0)
geomObj_6 = geompy.MakeVectorDXDYDZ(1, 0, 0)
geomObj_7 = geompy.MakeVectorDXDYDZ(0, 1, 0)
geomObj_8 = geompy.MakeVectorDXDYDZ(0, 0, 1)
geomObj_9 = geompy.MakeMarker(0, 0, 0, 1, 0, 0, 0, 1, 0)
geomObj_10 = geompy.MakeMarker(0, 0, 0, 1, 0, 0, 0, 1, 0)
geomObj_11 = geompy.MakeMarker(0, 0, 0, 1, 0, 0, 0, 1, 0)
Volute_sketch = geompy.MakeSketcherOnPlane("Sketcher:F 'Vol_n6x' 'Vol_n6y':TT 'Vol_nstart_x' 'Vol_nstart_y':AA 'Vol_n0x_abs' 'Vol_n0y_abs':AA 'Vol_n1x' 'Vol_n1y':AA 'Vol_n2x' 'Vol_n2y':AA 'Vol_n3x' 'Vol_n3y':AA 'Vol_n4x' 'Vol_n4y':TT 'Vol_n5x' 'Vol_n5y'", geomObj_11 )
Volute_exit_left = geompy.MakeVertex("Vol_n5x", "Vol_n5y", 0)
Vol_exit_tongue = geompy.MakeVertex("Vol_n6x", "Vol_n6y", 0)
Vol_exit_line = geompy.MakeLineTwoPnt(Volute_exit_left, Vol_exit_tongue)
Volute_wire = geompy.MakeWire([Volute_sketch, Vol_exit_line], 1e-07)
Volute_face = geompy.MakeFaceWires([Volute_wire], 1)

Blade_N1 = geompy.MakeVertex("Blade_N1x", "Blade_N1y", 0)
Blade_N2 = geompy.MakeVertex("Blade_N2x", "Blade_N2y", 0)
Blade_N3 = geompy.MakeVertex("Blade_N3x", "Blade_N3y", 0)
Blade_N4 = geompy.MakeVertex("Blade_N4x", "Blade_N4y", 0)
Blade_L = geompy.MakeVertex("Blade_Lx", "Blade_Ly", 0)
Blade_N5 = geompy.MakeVertex("Blade_N5x", "Blade_N5y", 0)

geomObj_12 = geompy.MakeMarker(0, 0, 0, 1, 0, 0, 0, 1, 0)
Sketch_1 = geompy.MakeSketcherOnPlane("Sketcher:F 'Blade_N4x' 'Blade_N4y':TT 'Blade_N3x' 'Blade_N3y':AA 'Blade_N2x' 'Blade_N2y':TT 'Blade_Lx' 'Blade_Ly':AA 'Blade_N1x' 'Blade_N1y':AA 'Blade_N5x' 'Blade_N5y':AA 'Blade_N4x' 'Blade_N4y'", geomObj_5 )
Blade_Base_Surface = geompy.MakeFaceWires([Sketch_1], 1)

Multi_Rotation_1 = geompy.MultiRotate1DNbTimes(Blade_Base_Surface, OZ, "Blade_Number")
Rotor_outer_disc_for_booleanCut = geompy.MakeDiskR("Rot_out", 1)
Rotor_inner_disc_for_booleanCut = geompy.MakeDiskR("Rot_in", 1)
Volute_face_to_compound = geompy.MakeCut(Volute_face, Rotor_outer_disc_for_booleanCut)
Rotor_face_to_ExtractBlades_And_compound = geompy.MakeCut(Rotor_outer_disc_for_booleanCut, Rotor_inner_disc_for_booleanCut)
Rotor_face_to_compound = geompy.MakeCut(Rotor_face_to_ExtractBlades_And_compound, Multi_Rotation_1)
Inner2DInlet_disk_for_booleanCut = geompy.MakeDiskR("TwoD_inlet", 1)
Rotor2DInlet_face_to_compound = geompy.MakeCut(Rotor_inner_disc_for_booleanCut, Inner2DInlet_disk_for_booleanCut)
a2DFanCompound = geompy.MakeCompound([Volute_face_to_compound, Rotor_face_to_compound, Rotor2DInlet_face_to_compound])
a2DFanGlued = geompy.MakeGlueEdges(a2DFanCompound, 1e-07)
[volute_face_explosion,Inlet_face_explosion,Rotor_face_explosion] = geompy.ExtractShapes(a2DFanGlued, geompy.ShapeType["FACE"], True)
listSubShapeIDs = geompy.SubShapeAllIDs(volute_face_explosion, geompy.ShapeType["EDGE"])

[Shared_Edge_1] = geompy.GetSharedShapesMulti([volute_face_explosion, Rotor_face_explosion], geompy.ShapeType["EDGE"])
listSubShapeIDs = geompy.SubShapeAllIDs(Rotor_face_explosion, geompy.ShapeType["EDGE"])
[Shared_Edge_2] = geompy.GetSharedShapesMulti([Rotor_face_explosion, Inlet_face_explosion], geompy.ShapeType["EDGE"])

# Blades group (Rotor without interfaces). We create the _Blade_Walls group
_RotorWallsID = geompy.SubShapeAllIDs(Rotor_face_explosion,geompy.ShapeType["EDGE"])
_RotorInnerInterface_shape = geompy.GetSame(Rotor_face_explosion,Shared_Edge_2)
_RotorOuterInterface_shape = geompy.GetSame(Rotor_face_explosion,Shared_Edge_1)
_RotorInnerInterface_ID = _RotorInnerInterface_shape.GetSubShapeIndices()
_RotorOuterInterface_ID = _RotorOuterInterface_shape.GetSubShapeIndices()
_BladesWallsID = _RotorWallsID
_BladesWallsID.remove(_RotorInnerInterface_ID[0])
_BladesWallsID.remove(_RotorOuterInterface_ID[0])
BE_Blade_Walls = geompy.CreateGroup(Rotor_face_explosion, geompy.ShapeType["EDGE"])
geompy.UnionIDs(BE_Blade_Walls, _BladesWallsID)

# Volute group 
_volute_walls_ID = geompy.SubShapeAllIDs(volute_face_explosion,geompy.ShapeType["EDGE"])
_Exit_shape=geompy.GetSame(volute_face_explosion,Vol_exit_line)
_Exit_ID = _Exit_shape.GetSubShapeIndices()
_Volute_interface_shape = geompy.GetSame(volute_face_explosion,Shared_Edge_1)
_Volute_interface_ID = _Volute_interface_shape.GetSubShapeIndices()
_volute_walls_ID.remove(_Exit_ID[0])
_volute_walls_ID.remove(_Volute_interface_ID[0])
BE_volute_walls = geompy.CreateGroup(volute_face_explosion, geompy.ShapeType["EDGE"])
geompy.UnionIDs(BE_volute_walls, _volute_walls_ID)

# Inlet group. This group is empty, since there is only BE_inlet and interface
_Inlet_ID = geompy.SubShapeAllIDs(Inlet_face_explosion,geompy.ShapeType["EDGE"])
_Inlet_Outer_Edge_shape = geompy.GetSame(Inlet_face_explosion,Shared_Edge_2)
_Inlet_Outer_Edge_ID = _Inlet_Outer_Edge_shape.GetSubShapeIndices()
_Inlet_ID.remove(_Inlet_Outer_Edge_ID[0])

# BE groups
BE_inlet = geompy.CreateGroup(Inlet_face_explosion, geompy.ShapeType["EDGE"])
geompy.UnionIDs(BE_inlet, _Inlet_ID)

BE_outlet = geompy.CreateGroup(volute_face_explosion, geompy.ShapeType["EDGE"])
geompy.UnionIDs(BE_outlet, _Exit_ID)

BE_walls = geompy.CreateGroup(a2DFanGlued, geompy.ShapeType["EDGE"])

geompy.UnionIDs(BE_walls, _volute_walls_ID + _volute_walls_ID )




geompy.addToStudy( OX, 'OX' )
geompy.addToStudy( OY, 'OY' )
geompy.addToStudy( OZ, 'OZ' )
geompy.addToStudy( Volute_sketch, 'Volute_sketch' )
geompy.addToStudy( Volute_exit_left, 'Volute_exit_left' )
geompy.addToStudy( Vol_exit_tongue, 'Vol_exit_tongue' )
geompy.addToStudy( Vol_exit_line, 'Vol_exit_line' )
geompy.addToStudy( Volute_wire, 'Volute_wire' )
geompy.addToStudy( Volute_face, 'Volute_face' )

geompy.addToStudy( O, 'O' )
geompy.addToStudy( OX, 'OX' )
geompy.addToStudy( OY, 'OY' )
geompy.addToStudy( OZ, 'OZ' )
geompy.addToStudy( Blade_N1, 'Blade_N1' )
geompy.addToStudy( Blade_N2, 'Blade_N2' )
geompy.addToStudy( Blade_N3, 'Blade_N3' )
geompy.addToStudy( Blade_N4, 'Blade_N4' )
geompy.addToStudy( Blade_N5, 'Blade_N5' )
geompy.addToStudy( Blade_L, 'Blade_L' )
geompy.addToStudy( Sketch_1, 'Sketch_1' )
geompy.addToStudy( Blade_Base_Surface, 'Blade_Base_Surface' )
geompy.addToStudy( Multi_Rotation_1, 'Multi_Rotation_1' )
geompy.addToStudy( Rotor_outer_disc_for_booleanCut, 'Rotor_outer_disc_for_booleanCut' )
geompy.addToStudy( Rotor_inner_disc_for_booleanCut, 'Rotor_inner_disc_for_booleanCut' )
geompy.addToStudy( Volute_face_to_compound, 'Volute_face_to_compound' )
geompy.addToStudy( Rotor_face_to_ExtractBlades_And_compound, 'Rotor_face_to_ExtractBlades_And_compound' )
geompy.addToStudy( Rotor_face_to_compound, 'Rotor_face_to_compound' )
geompy.addToStudy( Inner2DInlet_disk_for_booleanCut, 'Inner2DInlet_disk_for_booleanCut' )
geompy.addToStudy( Rotor2DInlet_face_to_compound, 'Rotor2DInlet_face_to_compound' )
geompy.addToStudy( a2DFanCompound, '2DFanCompound' )
geompy.addToStudy( a2DFanGlued, '2DFanGlued' )
geompy.addToStudyInFather( a2DFanGlued, volute_face_explosion, 'volute_face_explosion' )
geompy.addToStudyInFather( a2DFanGlued, Rotor_face_explosion, 'Rotor_face_explosion' )
geompy.addToStudyInFather( a2DFanGlued, Inlet_face_explosion, 'Inlet_face_explosion' )
# geompy.addToStudyInFather( volute_face_explosion, Shared_Edge_1, 'Shared_Edge_1' )
# geompy.addToStudyInFather( Rotor_face_explosion, Shared_Edge_2, 'Shared_Edge_2' )
geompy.addToStudyInFather( a2DFanGlued, BE_outlet, 'BE_outlet' )
geompy.addToStudyInFather( a2DFanGlued, BE_inlet, 'BE_inlet' )
geompy.addToStudyInFather( a2DFanGlued, BE_Blade_Walls, 'BE_Blade_Walls' )
geompy.addToStudyInFather( a2DFanGlued, BE_volute_walls, 'BE_volute_walls' )
# geompy.addToStudyInFather( a2DFanGlued, BE_walls, 'BE_walls' )


###
### SMESH component
###

import  SMESH, SALOMEDS
from salome.smesh import smeshBuilder

smesh = smeshBuilder.New(theStudy)
aFilterManager = smesh.CreateFilterManager()

from salome.StdMeshers import StdMeshersBuilder
from salome.NETGENPlugin import NETGENPluginBuilder
aCriteria = []
aCriterion = smesh.GetCriterion(SMESH.FACE,SMESH.FT_ElemGeomType,SMESH.FT_EqualTo,SMESH.Geom_TRIANGLE)
aCriteria.append(aCriterion)
aCriteria = []
aCriterion = smesh.GetCriterion(SMESH.FACE,SMESH.FT_ElemGeomType,SMESH.FT_EqualTo,SMESH.Geom_QUADRANGLE)
aCriteria.append(aCriterion)
aCriteria = []
aCriterion = smesh.GetCriterion(SMESH.VOLUME,SMESH.FT_ElemGeomType,SMESH.FT_EqualTo,SMESH.Geom_TETRA)
aCriteria.append(aCriterion)
aCriteria = []
aCriterion = smesh.GetCriterion(SMESH.VOLUME,SMESH.FT_ElemGeomType,SMESH.FT_EqualTo,SMESH.Geom_HEXA)
aCriteria.append(aCriterion)
aCriteria = []
aCriterion = smesh.GetCriterion(SMESH.VOLUME,SMESH.FT_ElemGeomType,SMESH.FT_EqualTo,SMESH.Geom_PYRAMID)
aCriteria.append(aCriterion)
aCriteria = []
aCriterion = smesh.GetCriterion(SMESH.VOLUME,SMESH.FT_ElemGeomType,SMESH.FT_EqualTo,SMESH.Geom_PENTA)
aCriteria.append(aCriterion)
MyMesh = smesh.Mesh(a2DFanGlued)
NETGEN_2D = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D,geom=volute_face_explosion)
VoluteSub = NETGEN_2D.GetSubMesh()
Volute = NETGEN_2D.Parameters()
Volute.SetSecondOrder( 0 )
Volute.SetOptimize( 1 )
Volute.SetFineness( 2 )
Volute.SetQuadAllowed( 0 )
Volute.SetMaxSize( "Mesh_VOLUTE_max_area" )
Volute.SetMinSize( "Mesh_VOLUTE_min_area" )
NETGEN_2D_1 = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D,geom=Rotor_face_explosion)
RotorSub = NETGEN_2D_1.GetSubMesh()
Rotor_1 = NETGEN_2D_1.Parameters()
Rotor_1.SetMaxSize( "Mesh_ROTOR_max_area" )
Rotor_1.SetSecondOrder( 0 )
Rotor_1.SetOptimize( 1 )
Rotor_1.SetMinSize( "Mesh_ROTOR_min_area" )
Rotor_1.SetQuadAllowed( 0 )
Rotor_1.SetFineness( 3 )
NETGEN_2D_2 = MyMesh.Triangle(algo=smeshBuilder.NETGEN_1D2D,geom=Inlet_face_explosion)
InletSub = NETGEN_2D_2.GetSubMesh()
Inletsub = NETGEN_2D_2.Parameters()
Inletsub.SetSecondOrder( 0 )
Inletsub.SetOptimize( 1 )
Inletsub.SetFineness( 2 )
Inletsub.SetQuadAllowed( 0 )
Inletsub.SetMinSize( "Mesh_INLET_min_area" )
Inletsub.SetMaxSize( "Mesh_INLET_max_area" )
Regular_1D = MyMesh.Segment(geom=Shared_Edge_2)
InletInterface = Regular_1D.NumberOfSegments(200,[],[  ])
InletInterface.SetDistrType( 0 )
isDone = MyMesh.SetMeshOrder( [ [RotorSub, VoluteSub] ])
isDone = MyMesh.Compute()
BE_inlet_1 = MyMesh.GroupOnGeom(BE_inlet,'BE_inlet',SMESH.EDGE)
BE_outlet_1 = MyMesh.GroupOnGeom(BE_outlet,'BE_outlet',SMESH.EDGE)
BE_Blade_Walls_1 = MyMesh.GroupOnGeom(BE_Blade_Walls,'BE_Blade_Walls',SMESH.EDGE)
BE_Volute_Walls_1 = MyMesh.GroupOnGeom(BE_volute_walls,'BE_Volute_Walls',SMESH.EDGE)
InletInterfaceSub = Regular_1D.GetSubMesh()

## set object names
smesh.SetName(MyMesh.GetMesh(), 'MyMesh')
smesh.SetName(NETGEN_2D.GetAlgorithm(), 'NETGEN_2D')
smesh.SetName(VoluteSub, 'VoluteSub')
smesh.SetName(Volute, 'Volute')
smesh.SetName(RotorSub, 'RotorSub')
smesh.SetName(Rotor_1, 'Rotor')
smesh.SetName(Inletsub, 'Inletsub')
smesh.SetName(Regular_1D.GetAlgorithm(), 'Regular_1D')
smesh.SetName(InletInterface, 'InletInterface')
smesh.SetName(BE_inlet_1, 'BE_inlet')
smesh.SetName(BE_outlet_1, 'BE_outlet')
smesh.SetName(BE_Blade_Walls_1, 'BE_Blade_Walls')
smesh.SetName(BE_Volute_Walls_1, 'BE_Volute_Walls')
smesh.SetName(InletInterfaceSub, 'InletInterfaceSub')
smesh.SetName(Inletsub, 'InletSub')

# Exporting the mesh

dirname = "/home/BCAMATH/igarcia/Dropbox/PhD/CFD/meshes/salome/Exported" # this line is where BB mesh will be exported . DO NOT MODIFY THIS LINE!!! affects JobManager.py

EXPORTMesh(False, dirname, MyMesh, SMESH, SALOMEDS, smeshBuilder,smesh)


if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser(1)
