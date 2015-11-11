from __future__ import division
from math import *
import numpy as np
import scipy
import matplotlib

# USER INPUT
def UserValues(VariablesToModify):
	# Blade Geometry Definition
	_DefinedValues_dict = dict()

	_DefinedValues_dict["Blade_F"]=0.85 # Fraction of contraflexure point from Blade initial point  respect to whole blade span
	_DefinedValues_dict["Blade_F1"]=0.2 
	_DefinedValues_dict["Blade_F2"]=0.2 
	_DefinedValues_dict["Blade_gamma"]=45 # Blade stagger angle
	_DefinedValues_dict["Blade_B1"]=70 # angle of attack (negative value is forward blade)
	_DefinedValues_dict["Blade_B2"]=30 # exit angle respect to Radiar vector
	# _DefinedValues_dict["Blade_theta"]=14 # Angle of contraflexure respect to stagger angle # I changed Blade definion. This vector is no longer used
	_DefinedValues_dict["Blade_D1_adim"]=0.3 # camber of at least 2%, the camber being defined as the ratio between the height of the neutral axis relative to the span and the length of the span. D1_adim/H*Blade_Ro
	_DefinedValues_dict["Blade_D2_adim"]=0.01 # * Blade_Ro
	_DefinedValues_dict["Blade_Ri"]=0.3 # blade leading edge radius
	_DefinedValues_dict["Blade_Ro"]=0.5 # b()())lade trailing edge radius
	_DefinedValues_dict["Blade_Thickness"]=0.005 # 
	_DefinedValues_dict["Blade_Number"]=13 # 
		
	# Volute Geometry Definition
	_DefinedValues_dict["Vol_beta"] = 10 # This is the angle at which the volute tongue starts to create the seashell respect to the tangent of the origin centered circle crossing that point.
	_DefinedValues_dict["Vol_omega"] = 40 # The user will introduce an angle like 40, but we work with 40+180
	_DefinedValues_dict["Vol_gamma"] = 63.8 # The user will introduce an angle like 40, but we work with 40+180
	_DefinedValues_dict["Vol_F_adim"] = 0.3 # This is the free space distance from the rotor outlet to the N0 vertex, in fraction of Rotor outlet Radius (Vol_F_adim = 1 = Rot_out distance)
	_DefinedValues_dict["Vol_Rc_adim"] = 0.1 # Radius of the volute tonge / fraction of Rot_out length
	_DefinedValues_dict["Vol_W_adim"] = 1 # Distance from Rot_out radius until the outest volute x coordinate (N4x), in fraction of Rot_out length (Vol_W_adim = 1 = Rot_out)
	_DefinedValues_dict["Vol_N1x_adim"] = 0.0 # in fraction of Rot_out length (Vol_N1x = 1 = Rot_out)
	_DefinedValues_dict["Vol_N2y_adim"] = 0.0 # in fraction of Rot_out length (Vol_N2y = 1 = Rot_out)
	_DefinedValues_dict["Vol_N3x_adim"] = 0.0 # in fraction of Rot_out length (Vol_N3x = 1 = Rot_out)
	_DefinedValues_dict["Vol_N4y_adim"] = 0.0 # in fraction of Rot_out length (Vol_N4y = 1 = Rot_out)
	_DefinedValues_dict["Vol_difussor_length_adim"] = 0.3

	# Rotor definition
	_DefinedValues_dict["Rot_in"] = 0.27 # 
	_DefinedValues_dict["Rot_out"] = 0.58 # 
	_DefinedValues_dict["TwoD_inlet_adim"] = 0.7 # This is inlet circle * Rot_in
	
	# Mesh fineness definition
	_DefinedValues_dict["Mesh_VOLUTE_max_area"] = 0.05
	_DefinedValues_dict["Mesh_VOLUTE_min_area"] = 0.01
	_DefinedValues_dict["Mesh_ROTOR_max_area"] = 0.03
	_DefinedValues_dict["Mesh_ROTOR_min_area"] = 0.00291902
	_DefinedValues_dict["Mesh_INLET_max_area"] = 0.015
	_DefinedValues_dict["Mesh_INLET_min_area"] = 0.001
	
	
	for i in range(len(VariablesToModify)):
		if VariablesToModify[i] in _DefinedValues_dict:
			if type(VariablesToModify[i+1]) is not str:
				_DefinedValues_dict[VariablesToModify[i]]=VariablesToModify[i+1]
			else: raise RuntimeError, "After variable %s there isn't a number % dicc[VariablesToModify[i]]"
	
	return _DefinedValues_dict

def ComputeGeometry(ModifiedVariables):
	
	# Definitions 
	_returnDicc = dict()
	Ri = ModifiedVariables["Blade_Ri"]
	Ro = ModifiedVariables["Blade_Ro"]
	F = ModifiedVariables["Blade_F"]
	F1 = ModifiedVariables["Blade_F1"]
	F2 = ModifiedVariables["Blade_F2"] # I changed Blade definion. This vector is no longer used
	gamma = ModifiedVariables["Blade_gamma"]
	B1 = ModifiedVariables["Blade_B1"]
	B2 = ModifiedVariables["Blade_B2"]
	# theta = ModifiedVariables["Blade_theta"] # Angle of contraflexure respect to stagger angle # I changed Blade definion. This vector is no longer used
	D1 = ModifiedVariables["Blade_D1_adim"]*(Ro-Ri)
	D2 = ModifiedVariables["Blade_D2_adim"]*(Ro-Ri)

	Blade_Thickness =ModifiedVariables["Blade_Thickness"]
	Blade_Number =ModifiedVariables["Blade_Number"]
	
	Rot_in = ModifiedVariables["Rot_in"]
	Rot_out = ModifiedVariables["Rot_out"]
	TwoD_inlet = ModifiedVariables["TwoD_inlet_adim"]*Rot_in
	
	
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
	Vol_difussor_length = ModifiedVariables["Vol_difussor_length_adim"]*Rot_out
	
	
	Mesh_VOLUTE_max_area = ModifiedVariables["Mesh_VOLUTE_max_area"]
	Mesh_VOLUTE_min_area = ModifiedVariables["Mesh_VOLUTE_min_area"]
	Mesh_ROTOR_max_area = ModifiedVariables["Mesh_ROTOR_max_area"]
	Mesh_ROTOR_min_area = ModifiedVariables["Mesh_ROTOR_min_area"]
	Mesh_INLET_max_area = ModifiedVariables["Mesh_INLET_max_area"]
	Mesh_INLET_min_area = ModifiedVariables["Mesh_INLET_min_area"]
	

	# CALCULATIONS OF BLADE

	gamma = gamma/360*2*pi
	# theta = theta/360*2*pi
	B1 = B1/360*2*pi
	B2 = B2/360*2*pi
	
	# Angle of attack
	uy=sin(pi/2+gamma)
	ux=cos(pi/2+gamma)
	
	N1px = ux
	N1py = uy

	# Node 0
	N0x = 0
	N0y = Ri
	N0px = cos(pi/2+B1)
	N0py = sin(pi/2+B1)

	# Node 4
	a=-2*Ri*N1py
	b=sqrt(4*Ri**2*N1py**2+4*(Ro**2-Ri**2))
	H=(a+b)/2
	N4x = H*ux
	N4y = H*uy + Ri
	N4px = cos(pi/2+gamma+B2)
	N4py = sin(pi/2+gamma+B2)

	# Node 2
	N2x = ux*H*F
	N2y = uy*H*F+Ri
	# N2px = cos(pi/2+gamma-theta) I changed Blade definion. This vector is no longer used
	# N2py = sin(pi/2+gamma-theta) I changed Blade definion. This vector is no longer used
	
	# Node 1
	N1x = ux * H*F*F1 - uy * D1
	N1y = uy * H*F*F1 + ux * D2 + Ri 
	
	# Node 3
	
	N3x = ux*(H*F+H*(1-F)*F2) + uy*H*D2
	N3y = uy*(H*F+H*(1-F)*F2) - ux*H*D2 + Ri
	N3px = ux
	N3py = uy
	
	
	# RETURN BLADES
	
	_returnDicc["Blade_N0x"] = N0x
	_returnDicc["Blade_N0y"] = N0y
	_returnDicc["Blade_N1x"] = N1x
	_returnDicc["Blade_N1y"] = N1y
	_returnDicc["Blade_N2x"] = N2x
	_returnDicc["Blade_N2y"] = N2y
	_returnDicc["Blade_N3x"] = N3x
	_returnDicc["Blade_N3y"] = N3y
	_returnDicc["Blade_N4x"] = N4x
	_returnDicc["Blade_N4y"] = N4y
	_returnDicc["Blade_N0px"] = N0px
	_returnDicc["Blade_N0py"] = N0py
	_returnDicc["Blade_N4px"] = N4px
	_returnDicc["Blade_N4py"] = N4py
	_returnDicc["Blade_Ri"] = Ri
	_returnDicc["Blade_Ro"] = Ro
	_returnDicc["Blade_Thickness"] = Blade_Thickness
	_returnDicc["Blade_Number"] = Blade_Number
	
	# CALCULATION OF ROTOR
	
	_returnDicc["Rot_in"] = Rot_in
	_returnDicc["Rot_out"] = Rot_out
	_returnDicc["TwoD_inlet"] = TwoD_inlet
	
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
	_returnDicc["Mesh_VOLUTE_max_area"] = Mesh_VOLUTE_max_area
	_returnDicc["Mesh_VOLUTE_min_area"] = Mesh_VOLUTE_min_area
	_returnDicc["Mesh_ROTOR_max_area"] = Mesh_ROTOR_max_area
	_returnDicc["Mesh_ROTOR_min_area"] = Mesh_ROTOR_min_area
	_returnDicc["Mesh_INLET_max_area"] = Mesh_INLET_max_area
	_returnDicc["Mesh_INLET_min_area"] = Mesh_INLET_min_area
	
	return _returnDicc
