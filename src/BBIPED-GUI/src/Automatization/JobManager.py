from subprocess import call
import os
import sys
import shutil
from array import array
import imp
import pdb
import os.path


_salomePath = "/home/BCAMATH/calonso/salome/appli_V7_2_0/runSalomeScript"
#_salomePath = "/home/ima/programs/installationDir/appli_V7_3_0/runSalomeScript"
#_BBipedPath = os.environ["BB_HOME"]
#_BBipedPath = "/home/BCAMATH/igarcia/Programs/BBiped/BBipedWorkingDirectory"
_BBipedExecPath ="./BBipedxSU2"
#_BBipedExecPath = _BBipedPath
#_AutomatizationPath="/home/ima/temp/.tools/Automatization/"
_AutomatizationPath="/home/BCAMATH/calonso/ToolDevelopment/workspace/CFD_REPO/build/debug/.tools/Automatization/"


sys.path.append(_AutomatizationPath+'/templates')

_meshScaleFactor = 0.001 # This variable converts mm into m


def Compute_Mesh(CallerPath,TemplateFile, *argv): 
	""" In this function we create the SU2 mesh as .case_mesh.py in the callers path. We first modify the mesh template for our needs Using "TemplateFile_Calculations". Then we compute the mesh which will always be called AutomaticMesh.su2 (Name is actually given in JobManager.EXPORTMesh function)"""
	VariablesToComp = argv
	print "\n\nJobManager > ComputingMesh"
	print "Compute Mesh for Template File: %s" % (TemplateFile,)
	print "Local working directory: %s" % (CallerPath,)
	ModifyMeshTemplate(CallerPath,TemplateFile,VariablesToComp) # Modifies template and writes .case_mesh.py in callers path
	_MeshToCompute = CallerPath+"/case_mesh.py" # writes out the case dump script which salome will read
	
	print "Imanol : _salomePath",_salomePath
	print "Imanol : _MeshToCompute",_MeshToCompute
	
	try:
		pro=call([_salomePath, _MeshToCompute]) # return mesh called AutomaticMesh.su2 
	except IOError:
		raise RuntimeError, "check path _salomePath: %s is correct in JobManager" % (_salomePath,)
		raise RuntimeError, "check path _MeshToCompute: %s is correct in JobManager" % (_MeshToCompute,)
		
def ModifyMeshTemplate(CallerPath,TemplateFile,VariablesToChangeFromTemplate):
	""" Computes the ..._Calculations.py and copies the TemplateFile into the Caller working directory, substituting template default values by the optionals set by the user call ModifyMeshTemplate(CallerPath,TemplateFile,VariablesToChangeFromTemplate). VariablesToChangeFromTemplate has to be of the structure ModifyMeshTemplate(CallerPath,TemplateFile, par1, par1_value, par2, par2_value,...) """
	_template_Geometry_Modifier=_AutomatizationPath+"templates/"+TemplateFile+"/"+TemplateFile+"_Calculations.py"
	
	print "\n\nJob Manager > ModifyMeshTemplate"
	
	_template_fullpath=_AutomatizationPath+"templates/"+TemplateFile+"/"+TemplateFile+".py"
	try:
		_template_calc = imp.load_source('_foo', _template_Geometry_Modifier) # The precious lines defines the template _Calculation file and lods it in _template_calc
	except IOError:
		raise RuntimeError, "check path _template_Geometry_Modifier exists (maybe template doesn't exist?): %s " % (_template_Geometry_Modifier,)
	
	
	GeometryVariablesToComp=_template_calc.UserValues(VariablesToChangeFromTemplate) # Loads _Calculation default values, and if there is any new value from the Task_to_perform call, it sets it to the case. Returns a dictionary of all the variables and values. It will pass the key parameter and the value: If we want to pass 3 new geometry values, there will be 6 parameters in the function (key1, value1, key 2...)
	
	SalomeVariablesToComp=_template_calc.ComputeGeometry(GeometryVariablesToComp)
	
	SalomeVariablesToComp_str=SalomeVariablesToComp.keys()
	
	fout= open(CallerPath+'/case_mesh.py', 'w') # Next loop reads the salome template file, and copies it to the case_mesh.py file. But writes out the SalomeVariablesToComp dictionary values in notebook, instead of template ones. 
	with open(_template_fullpath) as _template:
		for line in _template:
			_break = 0
			for keyword in SalomeVariablesToComp_str:
				key_temp = 'notebook.set("%s"'% keyword
				if key_temp in line: # write here all geometry modifications
					if key_temp not in 'notebook.set("Blade_Number"':
						_strToWrite = 'notebook.set("%s", %f)\n' % (keyword, SalomeVariablesToComp[keyword])
						fout.write(_strToWrite)
					else:
						_strToWrite = 'notebook.set("%s", %i)\n' % (keyword, SalomeVariablesToComp[keyword])
						fout.write(_strToWrite)
					_break = 1
					
			if _break == 0:
				if "this line is where BB mesh will be exported" in line:
					_foo=str("dirname = '" + CallerPath + "'\n")
					fout.write(_foo)
				else:
					fout.write(line)
	fout.close()

def ModifyCfg(CurrentPath,old_Configuration_File,new_Config_File,*argv):
	""" When called reads a provided configuration file, and rewrites it changing the options provided by user: structure has to be ModifyCfg(CurrentPath,old_Configuration_File,new_Config_File, par1, par1_value, par2, par2_value,...) """
	
	VariablesToModify = ParseCfg(argv)  # Creates a dictionary by the user provided Markers and values.
	VariablesToModify_str=VariablesToModify.keys()
	fout= open(CurrentPath+'/'+new_Config_File, 'w')
	with open(old_Configuration_File) as _OldConfigFile:
		for line in _OldConfigFile:
			_break = 0
			for keyword in VariablesToModify_str:
				if keyword in line:
					_strToWrite = "%s%s\n" % (keyword, VariablesToModify[keyword])
					fout.write(_strToWrite)
					_break=1
					
			if _break == 0:
				fout.write(line)
	fout.close()
		
def ParseCfg(arguments):
	MarkersDict = dict()
	for _iter in range(len(arguments)/2):
		_iter = _iter * 2
		MarkersDict[arguments[_iter]]=arguments[_iter+1]
	return MarkersDict

def Compute_BB(ConfigFile):
	""" Simply runs the executable by command call([_BBipedExecPath, _cfgToCompute]) """
	_cfgToCompute=ConfigFile
	print "\n\nJobManager > Comput_BB"
	
	try:
		pro=call([_BBipedExecPath, _cfgToCompute])
	except IOError:
		raise RuntimeError, "check path _BBipedExecPath: %s is correct in JobManager" % (_BBipedExecPath,)
		raise RuntimeError, "check path _cfgToCompute: %s is correct in JobManager" % (_cfgToCompute,)
		

def Report(ReportName, ConfigFileID,CallerPath,*argv):
	""" It reads the history of ConfigFileID CONV_FILENAME file, which. For the user given variables in the (Report call), creates a column based report of those values. Its line represents an independent simulations. I still didn't add the Report Header line to identify the variable reported. However, this matches with the user call variables.'"""
	
	#### HERE WE CHECK THE HISTORY FILE NAME FOR A CONFIG FILE, OPEN IT AND SEARCH FOR THE INDICATED VARIABLES
	
	
	_newconf = CallerPath+"/"+ConfigFileID
	try:
		with open(_newconf) as NewConfigName:
			for _line in NewConfigName:
				#if len(_line)<4:
				#	_line=""
				#	continue
				_parsed = _line.split("=")
				_parsed[0] = _parsed[0].translate(None,' ') # remove strange characters
				if _parsed[0] == "CONV_FILENAME": # % Output file convergence history (w/o extension) in configuration file
					HistoryFileName = _parsed[1].replace(".csv","")
					#HistoryFileName = _parsed[1].replace(".cfg","")
					HistoryFileName = HistoryFileName.replace("\n","")
					HistoryFileName = HistoryFileName.replace("\r","")
					if HistoryFileName[0:1] == " ":
						HistoryFileName = HistoryFileName [1:]
					HistoryFileName = HistoryFileName+".csv"
					print "history file, %s" % (HistoryFileName,)
					break
			NewConfigName.close()
	except IOError:
		raise RuntimeError, "check path _newconf: %s is correct in JobManager" % (_newconf,)
	_HistoryCall = CallerPath + "/"+ HistoryFileName
	try: 
		with open(_HistoryCall) as History: 
			_line = History.readline() # We read first line (variable headers) and We split it in an array
			_line = _line.split(",") # 
			_ReportIndexPointer=dict() # This dictionary will point for the user asked variables the entry index in the previously created array.
			__ReportDict=dict() # Initialize dictionary for variables values.
			for _var in argv: # For each variable reported by user, we find the index and store it in dictionary
				_var=str(_var)
				try:
					_ReportIndexPointer['"'+_var+'"'] = _line.index('"'+_var+'"')
				except ValueError:
					pass
					
			for _line in History: 
				_line = _line.split(",")
				for _key, _value in _ReportIndexPointer.iteritems(): # For each variable reported by user, we find the value and store it in dictionary at each history line. When last line is arrived, the bucle stops and the values of the dictionary will be last history line values
					__ReportDict[_key] = _line[_value]
	except IOError:
		raise RuntimeError, "check path _HistoryCall: %s is correct in JobManager" % (_HistoryCall,)
	
	
	#### HERE WE CHECK THE FILE NAMED Efficiency.csv, OPEN IT AND SEARCH FOR THE INDICATED VARIABLES
	
	_HistoryCall = "Efficiency.csv"
	try: 
		with open(_HistoryCall) as History: 
			_line = History.readline() # We read first line (variable headers) and We split it in an array
			_line = _line.split(",") # 
			_ReportIndexPointerEff=dict() # This dictionary will point for the user asked variables the entry index in the previously created array.
			_ReportDictEff=dict() # Initialize dictionary for variables values.
			for _var in argv: # For each variable reported by user, we find the index and store it in dictionary
				_var=str(_var)
				try:
					_ReportIndexPointerEff[_var] = _line.index(_var)
				except ValueError:
					pass
				
			for _line in History: 
				_line = _line.split(",")
				for _key, _value in _ReportIndexPointerEff.iteritems(): # For each variable reported by user, we find the value and store it in dictionary at each history line. When last line is arrived, the bucle stops and the values of the dictionary will be last history line values
					_ReportDictEff[_key] = _line[_value]
	except IOError:
		pass
	
	### MERGE BOTH DICTIONARIES IN __ReportDict ####
	
	for _key, _value in _ReportDictEff.iteritems():
		__ReportDict[_key] = _ReportDictEff[_key] # Copy dictionary
	
	### HERE STARTS THE WRITE OUT OF REPORT FILE ####
	
	_textTobeWriten = ConfigFileID # Name of the simulated configuration file will be writen in first column. helps to identify the line in the report with a simulation
	for _key, _value in __ReportDict.iteritems():
		_textTobeWriten = _textTobeWriten + "  ," + str(_value)  # Write out the variables values
	_textTobeWriten = _textTobeWriten.replace("\n","")
	existRep = os.path.isfile(ReportName)
	if existRep == False:
		with open(ReportName, "a") as myfile: # Append title
			mystr = "config file name, "
			
			for _key, _value in __ReportDict.iteritems():
				mystr = mystr + str(_key) + "  ,  "
			mystr = mystr[:-4]
			myfile.write(mystr)
			myfile.write("\n")
	with open(ReportName, "a") as myfile: # Append a jump of line
			myfile.write(_textTobeWriten)
			myfile.write("\n")
			
		



def CopyMesh(CallerPath,newMeshName):
	""" Optionall function that copies the automatically created mesh to a unique name, in order to save it """
	MeshPath = CallerPath + "/AutomaticMesh.su2"
	shutil.copyfile(MeshPath,CallerPath+"/"+newMeshName)
	
#def CopyCfgFile(CallerPath,OldCfgFilePath,newMeshName):   ##### BORRAR
#	shutil.copyfile(OldCfgFilePath,CallerPath+"/"+newMeshName)


def EXPORTMesh(Multizone, dirname, AutomaticMesh, SMESH, SALOMEDS, smeshBuilder, smesh): # Multizone = true or false
	""" Export mesh exports salome mesh into SU2 (and if manually uncomented in this function, also to STL mesh). It is called directly from the case_mesh.py. It currently doesn't support Multizone. """
	globMultiZoneActive = Multizone # THERE IS NO TK IN THIS VERISION!! # From tk we will change to true if we click on MultiZone export
# VARIABLE DEFINITIONS
	globMesh = [] #global Mesh list variable, if the value is 1 it means we selected to export the corresponding mesh
	# i/o definitions
	old_stdout=sys.stdout # We copy Salome stdout to return from file write to console output

#def PrintToConsole(old_stdout):
#	stdout.close()
#	sys.stdout = old_stdout 

#if globMesh[MeshIter]==1: # If we checked we want to export this mesh
	OutMeshName = dirname + "/" + "AutomaticMesh" # we extend the path to the main directory. We will turn this to file or folder depending if we export mesh or submeshes
	FilePath = OutMeshName + ".su2" # Set the path with file .su2 extension
	wrt=Write_No_Multizone(AutomaticMesh,FilePath, SMESH, SALOMEDS, smeshBuilder, smesh)
	sys.stdout = old_stdout  # We return the stdout to the salome console

	# AutomaticMesh.ExportSTL(dirname +"/TUIMesh.stl") # 

#                  ********************************                                   \       
#                  *         Export Mesh
#                  ********************************                                   \  
#                                                                                     \     
#vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\






#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\
#                                                                                     \  
#                        *********************************                            \        
#                        *        Export Mesh                                            
#                        *********************************                            \  

class Write_No_Multizone:
	""" If the Multizone export is not activated (globMultiZoneActive variable is false) we export meshes with this Class. If Multizone is activated, but the mesh to be exported doesn't have anu submesh, it is exported wich this Class as well """
	def __init__(self,Mesh, FilePath, SMESH, SALOMEDS, smeshBuilder, smesh):
		sys.stdout = open (FilePath, "w")
		self.filters(Mesh,SMESH, SALOMEDS, smeshBuilder, smesh)
		#self.ClassMesh = Mesh
		self.MeshDimen = Mesh.MeshDimension()
		self.Write(Mesh,SMESH, SALOMEDS, smeshBuilder, smesh)
	
	def filters(self,Mesh,SMESH, SALOMEDS, smeshBuilder, smesh):
		self.filter_tri = smesh.GetFilter(smeshBuilder.FACE, smeshBuilder.FT_ElemGeomType, smeshBuilder.Geom_TRIANGLE)
		self.filter_qua = smesh.GetFilter(smeshBuilder.FACE, smeshBuilder.FT_ElemGeomType, smeshBuilder.Geom_QUADRANGLE)
		self.filter_tet = smesh.GetFilter(smeshBuilder.VOLUME, smeshBuilder.FT_ElemGeomType, smeshBuilder.Geom_TETRA)
		self.filter_hex = smesh.GetFilter(smeshBuilder.VOLUME, smeshBuilder.FT_ElemGeomType, smeshBuilder.Geom_HEXA)
		self.filter_pyr = smesh.GetFilter(smeshBuilder.VOLUME, smeshBuilder.FT_ElemGeomType, smeshBuilder.Geom_PYRAMID)
		self.filter_pen = smesh.GetFilter(smeshBuilder.VOLUME, smeshBuilder.FT_ElemGeomType, smeshBuilder.Geom_PENTA)
		self.ids_tri = Mesh.GetIdsFromFilter(self.filter_tri)
		self.ids_qua = Mesh.GetIdsFromFilter(self.filter_qua)
		self.ids_hex = Mesh.GetIdsFromFilter(self.filter_hex)
		self.ids_tet = Mesh.GetIdsFromFilter(self.filter_tet)
		self.ids_pyr = Mesh.GetIdsFromFilter(self.filter_pyr)
		self.ids_pen = Mesh.GetIdsFromFilter(self.filter_pen)
	
	def MeshInfo(self):
		print "%"
		print "% Problem dimension"
		print "%"
		# Define cell type filter, find IDs and write to file
		print "NDIME=", self.MeshDimen
		print "% Number of triangles:", len(self.ids_tri)
		print "% Number of quadrangles:", len(self.ids_qua)
		if self.MeshDimen == 3:
			print "% Number of hexahedrons:", len(self.ids_hex)
			print "% Number of tetrahedrons:", len(self.ids_tet)
			print "% Number of pyramids:", len(self.ids_pyr)
			print "% Number of prisms:", len(self.ids_pen)
			self.NBMeshCells = len(self.ids_hex)+len(self.ids_tet)+len(self.ids_pyr)+len(self.ids_pen)
		self.NBSurCells = len(self.ids_tri)+len(self.ids_qua)
		print "%"
		print "% Total Surface Cells: ", self.NBSurCells 
		if self.MeshDimen == 3:
			print "% Total Volumen Cells: ", self.NBMeshCells
			print "%"
			################################################################
	def Write(self,Mesh,SMESH, SALOMEDS, smeshBuilder, smesh):
		""" By this function we start the write off process for 2D meshes.  """
		self.MeshInfo()
		print "%"
		print "% Inner element connectivity"
		print "%"
		# Find number of nodes
		self.NBnodesMesh = Mesh.NbNodes() 
		self.CheckNodesNB(self.NBnodesMesh) # Check is not empty
		if self.MeshDimen == 3:
			self.CheckVolNB(self.NBMeshCells) # Check is not empty
		else:
			self.CheckVolNB(self.NBSurCells) # Check is not empty
		self.WriteVolCells(Mesh) # Cell connectivity
		self.WriteNodes(Mesh)
		self.WriteBoundar(Mesh) #BOUNDARY ELEMENTS 2-D/3-D
		self.WriteCellInfo()
		
	def CheckNodesNB(self, Nodes):
		if Nodes == 0:
			raise RuntimeError, "ERROR!!! The number of NODES is equal 0!!!"
			
	def CheckVolNB(self,vols):
		if vols == 0:
			raise RuntimeError, "ERROR!!! The number of CELLS is equal 0!!!"
		print "NELEM=", vols
		
	def WriteVolCells(self,Mesh):
		counter = 0
		if self.MeshDimen == 2:
			for ind in self.ids_qua:
				nnode1=Mesh.GetElemNode(ind,0)-1
				nnode2=Mesh.GetElemNode(ind,1)-1
				nnode3=Mesh.GetElemNode(ind,2)-1
				nnode4=Mesh.GetElemNode(ind,3)-1
				print "9", nnode1,nnode2,nnode3,nnode4,counter
				counter=counter+1
			for ind in self.ids_tri:
				nnode1=Mesh.GetElemNode(ind,0)-1
				nnode2=Mesh.GetElemNode(ind,1)-1
				nnode3=Mesh.GetElemNode(ind,2)-1
				print "5", nnode1,nnode2,nnode3,counter
				counter=counter+1
		else:
			for ind in self.ids_pen:    
				nnode1=Mesh.GetElemNode(ind,0)-1
				nnode2=Mesh.GetElemNode(ind,1)-1
				nnode3=Mesh.GetElemNode(ind,2)-1
				nnode4=Mesh.GetElemNode(ind,3)-1
				nnode5=Mesh.GetElemNode(ind,4)-1
				nnode6=Mesh.GetElemNode(ind,5)-1
				print "13", nnode1, nnode2, nnode3, nnode4, nnode5, nnode6, counter
				counter=counter+1
			for ind in self.ids_tet:
				nnode1=Mesh.GetElemNode(ind,0)-1
				nnode2=Mesh.GetElemNode(ind,1)-1
				nnode3=Mesh.GetElemNode(ind,2)-1
				nnode4=Mesh.GetElemNode(ind,3)-1
				print "10", nnode1,nnode2,nnode3,nnode4,counter
				counter=counter+1
			for ind in self.ids_hex:
				nnode1=Mesh.GetElemNode(ind,0)-1
				nnode2=Mesh.GetElemNode(ind,1)-1
				nnode3=Mesh.GetElemNode(ind,2)-1
				nnode4=Mesh.GetElemNode(ind,3)-1
				nnode5=Mesh.GetElemNode(ind,4)-1
				nnode6=Mesh.GetElemNode(ind,5)-1
				nnode7=Mesh.GetElemNode(ind,6)-1
				nnode8=Mesh.GetElemNode(ind,7)-1
				print "12", nnode1,nnode2,nnode3,nnode4,nnode5,nnode6,nnode7,nnode8,counter
				counter=counter+1
			for ind in self.ids_pyr:
				nnode1=Mesh.GetElemNode(ind,0)-1
				nnode2=Mesh.GetElemNode(ind,1)-1
				nnode3=Mesh.GetElemNode(ind,2)-1
				nnode4=Mesh.GetElemNode(ind,3)-1
				nnode5=Mesh.GetElemNode(ind,4)-1
				print "14", nnode1,nnode2,nnode3,nnode4,nnode5,counter
				counter=counter+1
			
	def WriteNodes(self, Mesh):
		print "%"
		print "% Node coordinates"
		print "%"
		self.nodes = Mesh.GetNodesId()
		print "NPOIN= ", len(self.nodes)
		if self.MeshDimen == 2:
			for ind in self.nodes:       
				coord=Mesh.GetNodeXYZ(ind)
				# print "\t", '%.16f' % coord[0], " \t",'%.16f' %  coord[1], " \t", '%.16f' %  coord[2], " \t", ind-1
				print '%.16f' % (coord[0]*_meshScaleFactor), '%.16f' %  (coord[1]*_meshScaleFactor), ind-1
		else:
			for ind in self.nodes:       
				coord=Mesh.GetNodeXYZ(ind)
				# print "\t", '%.16f' % coord[0], " \t",'%.16f' %  coord[1], " \t", '%.16f' %  coord[2], " \t", ind-1
				print '%.16f' % (coord[0]*_meshScaleFactor), '%.16f' %  (coord[1]*_meshScaleFactor), '%.16f' %  (coord[2]*_meshScaleFactor), ind-1
				
	def WriteBoundar(self, Mesh):
		print "%"
		print "% Boundary elements"
		print "%"
		# Get the number of groups
		self.nbGroups = 0 # group initialization
		self.groups = Mesh.GetGroups()# Get the groups of the Mesh
		self.GroupNameList=[] # accepted grup initialization
		for group in self.groups:# For each group of the main Mesh
			self.GroupName = group.GetName()# Print the group name
			self.groupElementIDs = group.GetListOfID()# Get the element IDs for the iteration group
			if not self.groupElementIDs:
				continue
			if self.GroupName[0:9] == "NOEXPORT_":  # This will be useful to skip mesh groups we don't want to be exported
				continue
			self.nbGroups = self.nbGroups + 1 # Increase by one the number of accepted groups for the submesh
			self.GroupNameList.append(self.GroupName) # Append to accepted submesh groups names
			vars()[self.GroupName] = self.groupElementIDs # we create a new var of arrays with the group name. It contains the group elemID
		print "NMARK=",self.nbGroups 
		for groupIter in range(self.nbGroups):# For each of them...
			print "MARKER_TAG=",self.GroupNameList[groupIter] # Print the group name
			self.GroupVarName = vars()[self.GroupNameList[groupIter]]
			print "MARKER_ELEMS=",len(self.GroupVarName)# Print the number of elements 
			for groupElementID in self.GroupVarName:# For each of them...
				self.groupElementNodes = Mesh.GetElemNodes(groupElementID)# Get the node IDs
				if len(self.groupElementNodes) == 3:
					groupElementNode1 = self.groupElementNodes[0]-1
					groupElementNode2 = self.groupElementNodes[1]-1
					groupElementNode3 = self.groupElementNodes[2]-1
					print "5", groupElementNode1,  groupElementNode2, groupElementNode3
				if len(self.groupElementNodes) == 4:
					groupElementNode1 = self.groupElementNodes[0]-1
					groupElementNode2 = self.groupElementNodes[1]-1
					groupElementNode3 = self.groupElementNodes[2]-1
					groupElementNode4 = self.groupElementNodes[3]-1
					print "9", groupElementNode1,  groupElementNode2, groupElementNode3, groupElementNode4
				if len(self.groupElementNodes) == 2:
					groupElementNode1 = self.groupElementNodes[0]-1
					groupElementNode2 = self.groupElementNodes[1]-1
					print "3", groupElementNode1,  groupElementNode2
					
		
	def WriteCellInfo(self):
		print "% Boundary Tags summary:"
		for BoundaryNames in self.GroupNameList:
			print "  % ", BoundaryNames	
		print "% Element Type      Identifier"
		print "% Line                  3"
		print "% Triangle              5"
		print "% Rectangle             9"
		print "% Tetrahedral           10"
		print "% Hexahedral            12"
		print "% Wedge                 13"
		print "% Pyramid               14"


