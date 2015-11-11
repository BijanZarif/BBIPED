"""
This automatization script includes most of the functions that where originally developed, and they are writen how should they have been used. They are old versioned and probably don't work any more.
Contains an example of how most of the JobManager functions work. It is given as an example of possibilities, not to be used.
"""
import os
import sys
_BBipedPath = os.environ["BB_HOME"]
_AutomatizationPath=_BBipedPath+"/trunk/Automatization/"
sys.path.append(_AutomatizationPath)
import JobManager

CurrentPath = os.getcwd()

##### BEGIN USER EDITABLE SECTION ######

Geometry_Template = "SBlade2D_template"

# Geometry variables possible to be modified
"""


	_DefinedValues_dict = dict()

"lade_F 			= 0.85 # Fraction of contraflexure point from Blade initial point  respect to whole blade span
Blade_F1 			= 0.2 
Blade_F2			=0.2
Blade_gamma			=30 # Blade stagger angle
Blade_B1			=70 # angle of attack (negative value is forward blade)
Blade_B2			=30 # exit angle respect to Radiar vector
Blade_theta			=14 # Angle of contraflexure respect to stagger angle
Blade_D1_adim			=0.3 # camber of at least 2%, the camber being defined as the ratio between the height ofthe neutral axis relative to the span and the length of the span. D1_adim/H*Blade_Ro
Blade_D2_adim			=0.01 # * Blade_Ro
Blade_Ri			=0.3 # blade leading edge radius
Blade_Ro			=0.5 # b()())lade trailing edge radius
Blade_Thickness			=0.005 # 
Blade_Number			=13 # 
		
	# Volute Geometry Definition
Vol_beta			= 10 # This is the angle at which the volute tongue starts to create the seashell respect to the tangent of the origin centered circle crossing that point.
Vol_omega			= 40 # The user will introduce an angle like 40, but we work with 40+180
Vol_gamma			= 63.8 # The user will introduce an angle like 40, but we work with 40+180
Vol_F_adim			= 0.3 # This is the free space distance from the rotor outlet to the N0 vertex, in fraction of Rotor outlet Radius (Vol_F_adim = 1 = Rot_out distance)
Vol_Rc_adiTwoD_inlet_adim m		 = 0.1 # Radius of the volute tonge / fraction of Rot_out length
Vol_W_adim			= 1 # Distance from Rot_out radius until the outest volute x coordinate (N4x), in fraction of Rot_out length (Vol_W = 1 = Rot_out)
Vol_N1x_adim			= 0.0 # in fraction of Rot_out length (Vol_N1x = 1 = Rot_out)
Vol_N2y_adim			= 0.0 # in fraction of Rot_out length (Vol_N2y = 1 = Rot_out)
Vol_N3x_adim			= 0.0 # in fraction of Rot_out length (Vol_N3x = 1 = Rot_out)
Vol_N4y_adim			= 0.0 # in fraction of Rot_out length (Vol_N4y = 1 = Rot_out)
Vol_difussor_length_adim	= 0.3

	# Rotor definition
Rot_in				= 0.27 # 
Rot_out				= 0.58 # 
TwoD_inlet_adim			= 0.7 # This is inlet circle * Rot_in
	
	# Mesh fineness definition
Mesh_VOLUTE_max_area		= 0.05
Mesh_VOLUTE_min_area		= 0.01
Mesh_ROTOR_max_area		= 0.03
Mesh_ROTOR_min_area		= 0.00291902
Mesh_INLET_max_area		= 0.015
Mesh_INLET_min_area		= 0.001


"""

name = "MyReport" # Remember to delete all previous reports with the same name
Pres = [100000,120000, 140000]
Blade_Num =[12,13,14]
for p in Pres:
	for Blade in Blade_Num:

		old_Configuration_File = CurrentPath+"/"+"Original2DExternalDrag.cfg"
		newConfigFile = "newCfgBlade%iPres%i.cfg" % (Blade,p)

		JobManager.Compute_Mesh(CurrentPath,Geometry_Template, "Blade_Number", Blade )
		JobManager.ModifyCfg(CurrentPath,old_Configuration_File,newConfigFile, "MARKER_OUTLET","= ( BE_outlet, %i )" % (p))
		JobManager.Compute_BB(newConfigFile)
		JobManager.Report(name,CurrentPath,"CDrag","CLift")
		JobManager.CopyMesh(CurrentPath,"newMesh%iPres%i" % (Blade, p))
		#JobManager.CopyCfgFile(CurrentPath,Configuration_File,"newCfgBlade%iPres%i" % Blade, p)


