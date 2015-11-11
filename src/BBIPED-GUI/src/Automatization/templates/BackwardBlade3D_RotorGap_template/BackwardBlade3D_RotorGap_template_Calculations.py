from __future__ import division
from math import *
import numpy as np
import scipy
import matplotlib

# USER INPUT
def UserValues(VariablesToModify):
	# Blade Geometry Definition
	_DefinedValues_dict = dict()

	_DefinedValues_dict["Blade_gamma"]=45 # Blade stagger angle
	_DefinedValues_dict["Blade_B1"]=15 # angle of attack (negative value is forward blade)
	_DefinedValues_dict["Blade_B2"]=15 # exit angle respect to Radiar vector
	_DefinedValues_dict["Blade_Ri"]=300 # blade leading edge radius
	_DefinedValues_dict["Blade_Ro"]=500 # blade trailing edge radius
	_DefinedValues_dict["Blade_Thickness"]=5 # 
	_DefinedValues_dict["Blade_Number"]=3 #
	_DefinedValues_dict["Blade_Height"]=148 #
	_DefinedValues_dict["Blade_LeadingEdge_Cut_Height_frac"]=0.4 #
	_DefinedValues_dict["Blade_TrailingEdge_Cut_Height_fac"]=0.9 #
	_DefinedValues_dict["Blade_Cut_RotorSpanfract"]=0.2 #
	
	
	# Volute Geometry Definition
	_DefinedValues_dict["Vol_beta"] = 8 # This is the angle at which the volute tongue starts to create the seashell respect to the tangent of the origin centered circle crossing that point.
	_DefinedValues_dict["Vol_omega"] = 40 # The user will introduce an angle like 40, but we work with 40+180
	_DefinedValues_dict["Vol_gamma"] = 63.8 # The user will introduce an angle like 40, but we work with 40+180
	_DefinedValues_dict["Vol_F_adim"] = 0.18 # This is the free space distance from the rotor outlet to the N0 vertex, in fraction of Rotor outlet Radius (Vol_F_adim = 1 = Rot_out distance)
	_DefinedValues_dict["Vol_Rc_adim"] = 0.045 # Radius of the volute tonge / Rot_out length
	_DefinedValues_dict["Vol_W_adim"] = 0.65 # Distance from Rot_out radius until the outest volute x coordinate (N4x), in fraction of Rot_out length (Vol_W = 1 = Rot_out)
	_DefinedValues_dict["Vol_N1x_adim"] = 0.0 # in fraction of Rot_out length (Vol_N1x = 1 = Rot_out)
	_DefinedValues_dict["Vol_N2y_adim"] = 0.0 # in fraction of Rot_out length (Vol_N2y = 1 = Rot_out)
	_DefinedValues_dict["Vol_N3x_adim"] = 0.0 # in fraction of Rot_out length (Vol_N3x = 1 = Rot_out)
	_DefinedValues_dict["Vol_N4y_adim"] = 0.0 # in fraction of Rot_out length (Vol_N4y = 1 = Rot_out)
	_DefinedValues_dict["Vol_difussor_length_adim"] = 1
	_DefinedValues_dict["Vol_Height"] = 444 

	# Rotor definition
	_DefinedValues_dict["Rot_in"] = 270 # 
	_DefinedValues_dict["Rot_out"] = 580 # 
	_DefinedValues_dict["Rot_out_unit_step_fraction"] = 0.05 # Relative to Rot_out
	_DefinedValues_dict["Rot_Height"] = 190
	_DefinedValues_dict["Rot_out_ceiling_fraction"] = 0.05 # Relative to (Blade outlet - Blade inlet)
	_DefinedValues_dict["Rot_Internal_tongeangle"] = 45
	_DefinedValues_dict["Rot_Internal_and_exit_tongue_separation_adim"] = 3 # Relative to Blade thickness 
	_DefinedValues_dict["Rot_Internal_tongLength_adim"] = 0.6 # Relative to Blade inlet Radius 
	
	
	_DefinedValues_dict["InletRadius_adim"] = 1.35 # Relative to Blade inlet Radius
	
	# only for 2D
	#_DefinedValues_dict["TwoD_inlet_adim"] = 0.7 # This is inlet circle * Rot_in
	
	# Mesh fineness definition
	_DefinedValues_dict["Mesh_scale_factor"] = 1
	_DefinedValues_dict["Mesh_VOLUTE_max_area"] = 50
	_DefinedValues_dict["Mesh_VOLUTE_min_area"] = 10
	_DefinedValues_dict["Mesh_ROTOR_max_area"] = 30
	_DefinedValues_dict["Mesh_ROTOR_min_area"] = 2
	_DefinedValues_dict["Mesh_INLET_max_area"] = 15
	_DefinedValues_dict["Mesh_INLET_min_area"] = 10
	
	
	for i in range(len(VariablesToModify)):
		if VariablesToModify[i] in _DefinedValues_dict:
			if type(VariablesToModify[i+1]) is not str:
				_DefinedValues_dict[VariablesToModify[i]]=VariablesToModify[i+1]
			else: raise RuntimeError, "After variable %s there isn't a number % dicc[VariablesToModify[i]]"
	
	return _DefinedValues_dict

def ComputeGeometry(ModifiedVariables):
	# For modifying a variable in the template from the user defined values (function UserValues). First import the definition with "Var = ModifiedVariables[...]" and then export it with "_returnDicc[...] = nbr "
	
	
	# Definitions 
	_returnDicc = dict()
	Ri = ModifiedVariables["Blade_Ri"]
	Ro = ModifiedVariables["Blade_Ro"]
	gamma = ModifiedVariables["Blade_gamma"]
	B1 = ModifiedVariables["Blade_B1"]
	B2 = ModifiedVariables["Blade_B2"]
	Blade_Cut_RotorSpanfract = ModifiedVariables["Blade_Cut_RotorSpanfract"]
	Blade_Thickness =ModifiedVariables["Blade_Thickness"]
	Blade_LeadingEdge_Cut_Height_frac =ModifiedVariables["Blade_LeadingEdge_Cut_Height_frac"]
	Blade_Height = ModifiedVariables["Blade_Height"]
	Blade_TrailingEdge_Cut_Height =ModifiedVariables["Blade_TrailingEdge_Cut_Height_fac"]*Blade_Height
	Blade_Number =ModifiedVariables["Blade_Number"]
	
	Rot_in = ModifiedVariables["Rot_in"]
	Rot_out = ModifiedVariables["Rot_out"]
	Rot_out_unit_step_fraction = ModifiedVariables["Rot_out_unit_step_fraction"] 
	Rot_Height = ModifiedVariables["Rot_Height"] 
	Rot_out_ceiling_fraction = ModifiedVariables["Rot_out_ceiling_fraction"] 
	
	Rot_Internal_tongeangle = ModifiedVariables["Rot_Internal_tongeangle"] 
	Rot_Internal_and_exit_tongue_separation = ModifiedVariables["Rot_Internal_and_exit_tongue_separation_adim"]  * Blade_Thickness
	Rot_Internal_tongLength = ModifiedVariables["Rot_Internal_tongLength_adim"] * Ri
	
	InletRadius_adim = ModifiedVariables["InletRadius_adim"] 
	InletRadius = InletRadius_adim * Ri
	
	
	Vol_beta = ModifiedVariables["Vol_beta"] # This is the angle at which the volute tongue starts to create the seashell respect to the tangent of the origin centered circle crossing that point.
	Vol_omega = ModifiedVariables["Vol_omega"]  #
	Vol_gamma = ModifiedVariables["Vol_gamma"]  # The user will introduce an angle like 40, but we work with 40+180
	Vol_F = ModifiedVariables["Vol_F_adim"]*Rot_out  # Distance between Rotor outlet and Vol_n0 divided by Rot_out
	Vol_Rc = ModifiedVariables["Vol_Rc_adim"]*Rot_out  # Radius of the volute tonge
	Vol_W = ModifiedVariables["Vol_W_adim"] *Rot_out
	Vol_n1x = ModifiedVariables["Vol_N1x_adim"] *Rot_out
	Vol_n2y = ModifiedVariables["Vol_N2y_adim"] *Rot_out
	Vol_n3x = ModifiedVariables["Vol_N3x_adim"] *Rot_out
	Vol_n4y = ModifiedVariables["Vol_N4y_adim"]*Rot_out
	Vol_Height = ModifiedVariables["Vol_Height"]
	Vol_difussor_length = ModifiedVariables["Vol_difussor_length_adim"]*Rot_out
	
	
	Mesh_VOLUTE_max_area = ModifiedVariables["Mesh_VOLUTE_max_area"]
	Mesh_VOLUTE_min_area = ModifiedVariables["Mesh_VOLUTE_min_area"]
	Mesh_ROTOR_max_area = ModifiedVariables["Mesh_ROTOR_max_area"]
	Mesh_ROTOR_min_area = ModifiedVariables["Mesh_ROTOR_min_area"]
	Mesh_INLET_max_area = ModifiedVariables["Mesh_INLET_max_area"]
	Mesh_INLET_min_area = ModifiedVariables["Mesh_INLET_min_area"]
	
	HugeValue = 20 * Rot_out
	

	# CALCULATIONS OF BLADE

	gamma = gamma/360*2*pi
	# theta = theta/360*2*pi
	B1 = B1/360*2*pi
	B2 = B2/360*2*pi
	
	"""
	# Angle of attack
	uy=sin(pi/2+gamma)
	ux=cos(pi/2+gamma)
	
	N1px = ux
	N1py = uy
	"""
	
	N1x = 0
	N1y = Ri
	
	Ux = -sin(B1)
	Uy = cos(B1)
	
	Vx = -Uy
	Vy = Ux
	
	# Converting from gamma to omega
	T = (-2*cos(gamma)*Ri+sqrt(4*cos(gamma)**2*Ri**2 + 4* (Ro**2-Ri**2))) / (2) 
	N2x = -sin(gamma)*T
	N2y = Ri + cos(gamma) *T
	omega = acos(N2y/Ro)
	
	Theta = gamma + B2
	
	Sx = - cos(pi/2-Theta)
	Sy = sin(pi/2-Theta)
	
	CLx = -Sy
	CLy = Sx
	
	
	# Solve location geometry constrains
	
	
	Crod=(N1y*Sx-N2y*Sx-N1x*Sy+N2x*Sy)/(CLy*Sx-CLx*Sy+Sy*Vx-Sx*Vy)
	H=-(CLy*N1x-CLx*N1y-CLy*N2x+CLx*N2y+N1y*Vx-N2y*Vx-N1x*Vy+N2x*Vy)/(CLy*Sx-CLx*Sy+Sy*Vx-Sx*Vy)	
	Cx=N1x+Vx*Crod
	Cy=N1y+Vy*Crod
	Lx=Cx-CLx*Crod
	Ly=Cy-CLy*Crod
	N3x=N2x-Sy*Blade_Thickness
	N3y=N2y+Sx*Blade_Thickness
	N4x=Lx+CLx*Blade_Thickness
	N4y=Ly+CLy*Blade_Thickness
	N5x=N1x+Vx*Blade_Thickness
	N5y=N1y+Vy*Blade_Thickness
	
	# For 3D, calculate Blade N1yup, and the box (face perpendicular to [N1px,N1py] is [N1py, -N1px]) that will cut the Blade. 
	
	Blade_N1zup = N1y + Blade_Height
	Blade_N5zup = N5y + Blade_Height
	_BladeChord = sqrt((N2y-N1y)**2+(N2x-N1x)**2)
	Blade_CutNodeY = N1y + _BladeChord * Blade_Cut_RotorSpanfract * cos(gamma)
	Blade_CutNodeX = N1x - _BladeChord * Blade_Cut_RotorSpanfract * sin(gamma)
	
	
	
	# RETURN BLADES
	
	_returnDicc["Blade_N1x"] = N1x
	_returnDicc["Blade_N1y"] = N1y
	_returnDicc["Blade_N2x"] = N2x
	_returnDicc["Blade_N2y"] = N2y
	_returnDicc["Blade_N3x"] = N3x
	_returnDicc["Blade_N3y"] = N3y
	_returnDicc["Blade_N4x"] = N4x
	_returnDicc["Blade_N4y"] = N4y
	_returnDicc["Blade_N5x"] = N5x
	_returnDicc["Blade_N5y"] = N5y
	_returnDicc["Blade_Lx"] = Lx
	_returnDicc["Blade_Ly"] = Ly
	_returnDicc["Blade_N1px"] = Ux
	_returnDicc["Blade_N1py"] = Uy
	_returnDicc["Blade_Sx"] = Sx
	_returnDicc["Blade_Sy"] = Sy
	_returnDicc["Blade_Ri"] = Ri
	_returnDicc["Blade_Ro"] = Ro
	_returnDicc["Blade_Thickness"] = Blade_Thickness
	_returnDicc["Blade_Thickness_ByMinusThree"] = - Blade_Thickness*3
	_returnDicc["Halve_Blade_Thickness"] = Blade_Thickness/2
	_returnDicc["Minus_Blade_Thickness"] = -Blade_Thickness
	_returnDicc["Blade_Height"] = Blade_Height
	_returnDicc["Blade_Number"] = Blade_Number
	_returnDicc["Blade_CutNodeY"] = Blade_CutNodeY
	_returnDicc["Blade_CutNodeX"] = Blade_CutNodeX
	_returnDicc["Blade_LeadingEdge_Cut_Height"] = Blade_LeadingEdge_Cut_Height_frac * Blade_Height
	_returnDicc["Blade_TrailingEdge_Cut_Height"] = Blade_TrailingEdge_Cut_Height
	
	# CALCULATION OF ROTOR
	
	_returnDicc["Rot_in"] = Rot_in
	_returnDicc["Rot_out"] = Rot_out
	_returnDicc["Rot_Height"] = Rot_Height
	_returnDicc["InletRadius"] = InletRadius
	_returnDicc["Rot_out_unit_step"] = Rot_out_unit_step_fraction * Rot_out
	_returnDicc["Rot_out_ceiling_fraction"] = Rot_out_ceiling_fraction
	_returnDicc["Rot_Internal_tongeangle"] = Rot_Internal_tongeangle 
	_returnDicc["Rot_Internal_and_exit_tongue_separation"] = Rot_Internal_and_exit_tongue_separation
	_returnDicc["Rot_Internal_tongLength"] = Rot_Internal_tongLength
	
	
	# CALCULATIONS OF VOLUTE
	# Convert grad to rad
	
	Vol_gamma = Vol_gamma/360*2*pi
	Vol_omega = Vol_omega/360*2*pi
	Vol_beta = Vol_beta/360*2*pi
	
	# Start Calculations

	Vol_vx = -sin(Vol_omega)
	Vol_vy = -cos(Vol_omega)
	
	Vol_ux = Vol_vy
	Vol_uy = Vol_vx
	
	Vol_nstart_x = -cos(Vol_gamma)*(Rot_out+Vol_F+Vol_Rc)
	Vol_nstart_y = sin(Vol_gamma)*(Rot_out+Vol_F+Vol_Rc)
	
	Vol_cx = Vol_nstart_x + Vol_ux * Vol_Rc
	Vol_cy = Vol_nstart_y + Vol_uy * Vol_Rc
	
	# start iteration to find N0
	
	Vol_gamma_des = Vol_gamma
	
	for i in range(7):
		Vol_n0x_rel = Vol_Rc*cos(3*pi/2 + Vol_gamma_des + Vol_beta)
		Vol_n0y_rel = Vol_Rc*sin(3*pi/2 + Vol_gamma_des + Vol_beta)
		
		Vol_nstart_x_rel = Vol_Rc*cos(pi-Vol_omega)
		Vol_nstart_y_rel = Vol_Rc*sin(pi-Vol_omega)
		
		Vol_Ax = Vol_n0x_rel - Vol_nstart_x_rel
		Vol_Ay = Vol_n0y_rel - Vol_nstart_y_rel
		
		Vol_n0x_abs = Vol_nstart_x + Vol_Ax
		Vol_n0y_abs = Vol_nstart_y + Vol_Ay
		
		Vol_gamma_des = pi/2-atan(-Vol_n0y_abs/Vol_n0x_abs)
	
	Vol_R0 = sqrt(Vol_n0x_abs**2+Vol_n0y_abs**2)
	Vol_n4x = -Vol_R0 - Vol_W
	Vol_n1y = Vol_R0+Vol_Rc
	Vol_AR = (-Vol_n4x - Vol_n1y)/3
	Vol_n2x = Vol_n1y + Vol_AR	
	Vol_n3y = - (Vol_n1y + Vol_AR*2)
	Vol_n5x = Vol_n4x
	Vol_n5y = Vol_nstart_y + Vol_difussor_length
	Vol_outlet_tonge_length = (Vol_n5y-Vol_nstart_y)/cos(Vol_omega)
	Vol_n6x = Vol_nstart_x + sin(Vol_omega)*Vol_outlet_tonge_length
	Vol_n6y = Vol_n5y
	
	
	_returnDicc["Vol_Height"] = Vol_Height
	_returnDicc["Vol_nstart_x"] = Vol_nstart_x
	_returnDicc["Vol_nstart_y"] = Vol_nstart_y
	_returnDicc["Vol_n0x_abs"] = Vol_n0x_abs
	_returnDicc["Vol_n0y_abs"] = Vol_n0y_abs
	_returnDicc["Vol_n1x"] = Vol_n1x
	_returnDicc["Vol_n1y"] = Vol_n1y
	_returnDicc["Vol_n2x"] = Vol_n2x
	_returnDicc["Vol_n2y"] = Vol_n2y
	_returnDicc["Vol_n3x"] = Vol_n3x
	_returnDicc["Vol_n3y"] = Vol_n3y
	_returnDicc["Vol_n4x"] = Vol_n4x
	_returnDicc["Vol_n4y"] = Vol_n4y
	_returnDicc["Vol_n5x"] = Vol_n5x
	_returnDicc["Vol_n5y"] = Vol_n5y
	_returnDicc["Vol_n6x"] = Vol_n6x
	_returnDicc["Vol_n6y"] = Vol_n6y
	
	# Return Meshing 
        _returnDicc["Mesh_VOLUTE_max_area"] = Mesh_VOLUTE_max_area*1
        _returnDicc["Mesh_VOLUTE_min_area"] = Mesh_VOLUTE_min_area*1
        _returnDicc["Mesh_ROTOR_max_area"] = Mesh_ROTOR_max_area*1
        _returnDicc["Mesh_ROTOR_min_area"] = Mesh_ROTOR_min_area*1
        _returnDicc["Mesh_INLET_max_area"] = Mesh_INLET_max_area*1
        _returnDicc["Mesh_INLET_min_area"] = Mesh_INLET_min_area*1
	
	# Return Others
	_returnDicc["HugeValue"] = HugeValue
	
	return _returnDicc
